// GRR20190397 Eduardo Henrique dos Santos Cerolli

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "lib_mosaico.h"


int main (int argc, char **argv) {

    DIR *diretorio;
    struct dirent **dir_pastilhas;
    struct dirent *dir_imagem;
    char pastilha_atual[100];
    char *pasta = "tiles";
    char *imagem_entrada = NULL;
    char *imagem_saida = NULL;
    FILE *arquivo;
    int quant_pastilhas;
    int atual = 0;
    int i;

    // confere os argumentos usados na chamada do programa
    if (argc > 1) {
        for (i = 1; i < argc; i++) {
            if (strcmp (argv[i], "-i") == 0) {
                imagem_entrada = argv[i+1];
                i++;
            }
            else if (strcmp (argv[i], "-o") == 0) {
                imagem_saida = argv[i+1];
                i++;
            }
            else if (strcmp (argv[i], "-p") == 0) {
                pasta = argv [i+1];
                i++;
            }
            else if ((strcmp (argv[i], "-h") == 0) || (strcmp (argv[i], "-help") == 0)) {
                fprintf (stderr, "O programa recebe uma imagem grande e converte ela em um fotomosaico com varias pastilhas pequenas.\n\n");
                fprintf (stderr, "Para informar a imagem que sera usada para criar o fotomosaico use o argumento \"-i\" quando for iniciar o programa. (caso nao informado sera lido pela entrada padrao)\n\n");
                fprintf (stderr, "Para informar o diretorio com as pastilhas use o argumento \"-p\" quando for iniciar o programa. (caso nao informado sera utilizado a pasta \"./tiles\")\n\n");
                fprintf (stderr, "Para informar a imagem de saida que recebera o fotomosaico use o argumento \"-o\" quando for iniciar o programa. (caso nao informado sera escrito na entrada padrao)\n\n");
                fprintf (stderr, "O argumento \"-h\" ou \"-help\" gera essa mensagem de ajuda.\n");
                exit (0);
            }
        }

    }

    //abre a pasta das pastilahas
    diretorio = opendir (pasta);
    

    // cria o vetor para armazenar as apstilhas e aloca ele
    pastilhas *pastilha;
    pastilha = (pastilhas*) malloc (sizeof (pastilhas));
    pastilha->tam = 0;

    // le todas as as pastilhas
    quant_pastilhas = scandir (pasta, &dir_pastilhas, NULL, alphasort);
    aloca_pastilhas (pastilha, quant_pastilhas);


    fprintf (stderr, "Lendo as apstilhas da pasta: %s\n", pasta);

    // guarda todas as pastilhas em memoria
    for (i = 0; i < quant_pastilhas; i++) {
        if (dir_pastilhas[i]->d_type == DT_REG) {

            strcat (pastilha_atual, pasta);
            strcat (pastilha_atual, "/");
            strcat (pastilha_atual, dir_pastilhas[i]->d_name);
            
            arquivo = fopen (pastilha_atual, "r");
            
            le_imagem (pastilha->vetor[atual], arquivo);

            fclose (arquivo);

            memset (pastilha_atual, 0, sizeof (pastilha_atual));

            atual++;
        }
        free (dir_pastilhas[i]);
    }
    free (dir_pastilhas);

    fprintf (stderr, "Total de pastilhas: %d\n", pastilha->tam);
    fprintf (stderr, "Tamanho das pastilhas: %dx%d\n", pastilha->vetor[0]->largura, pastilha->vetor[0]->altura);
    fprintf (stderr, "Calculando as medias de cores das pastilhas\n");

    // calcula as medias de todas as pastilhas
    for (i = 0; i < quant_pastilhas-2; i++) {
        calcula_media (pastilha->vetor[i], 0, 0, pastilha->vetor[i]->altura, pastilha->vetor[i]->largura);
    }


    (void) closedir (diretorio);


    diretorio = opendir ("./");
    dir_imagem = readdir (diretorio);

    while (dir_imagem->d_type == DT_DIR) {
        dir_imagem = readdir (diretorio);
    }


    imagem *img;
    img = (imagem*) malloc (sizeof (imagem));

    fprintf (stderr, "Lendo a imagem de entrada\n");

    if (imagem_entrada != NULL) {
        arquivo = fopen (imagem_entrada, "r");
        le_imagem (img, arquivo);
    }
    else {
        le_imagem_padrao (img);
    }

    fprintf (stderr, "A imagem é um %s, com tamanho %dx%d\n", img->tipo, img->largura, img->altura);
    fprintf (stderr, "Criando o fotomosaico\n");

    cria_mosaico (img, pastilha);

    fprintf (stderr, "Escrevendo o fotomosaico\n");

    if (imagem_saida != NULL) {
        arquivo = fopen (imagem_saida, "w");
        imprimir_imagem (img, arquivo);
    }
    else {
        imprimir_imagem_padrao (img);
    }

    (void) closedir (diretorio);
    
    destroi_pastilhas (pastilha);
    destroi_matriz (img);
    free (img);
    free (pastilha);


    return 0;
}