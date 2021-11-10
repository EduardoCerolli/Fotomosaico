// GRR20190397 Eduardo Henrique dos Santos Cerolli

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lib_mosaico.h"

// ignora as linhas de comentarios
void pula_comentario (FILE *arquivo) {
    char c;
    
    c = getc (arquivo);
    while ((c == '#') || (c == '\n')) {
        while (c != '\n') {
            c = getc (arquivo);
        }
        c = getc (arquivo);
    }
    ungetc (c, arquivo);

    return;
}

void aloca_matriz (imagem *img) {
    int i;

    img->pixels = (int**) malloc (img->altura * sizeof (int*));

    for (i = 0; i < img->altura; i++) {
        img->pixels[i] = (int*) malloc ((img->largura * 3) * sizeof (int));
    }

    return;
}

void aloca_pastilhas (pastilhas *pastilha, int tam) {
    int i;

    pastilha->vetor = (imagem**) malloc (sizeof (imagem*) * (tam-2));
    for (i = 0; i < tam-2; i++) {
        pastilha->vetor[i] = (imagem*) malloc (sizeof (imagem));
    }
    pastilha->tam = tam-2;

    return;
}

// le a imagem e salva em memoria, salvando os pixeis de cor em uma matriz
void le_imagem (imagem *img, FILE *arquivo) {
    int i,j;

    fscanf (arquivo, "%s", img->tipo);
    pula_comentario (arquivo);
    fscanf (arquivo, "%d %d", &img->largura, &img->altura);
    pula_comentario (arquivo);
    fscanf (arquivo, "%d", &img->cor_max);

    aloca_matriz (img);

    // verifica se o tipo for P3
    if (strcmp (img->tipo, "P3") == 0) {
        for (i = 0; i < img->altura; i++) {
            for (j = 0; j < (img->largura * 3); j++) {
            fscanf (arquivo, "%d", &img->pixels[i][j]);
            }
        }
    }
    else {
        unsigned char pixel;
        fscanf (arquivo, "%c", &pixel);
        for (i = 0; i < img->altura; i++) {
            for (j = 0; j < (img->largura * 3); j++) {
            fscanf (arquivo, "%c", &pixel);
            img->pixels[i][j] = pixel;
            }
        }
    }

    return;
}

void le_imagem_padrao (imagem *img) {
    int i,j;

    scanf ("%s", img->tipo);
    scanf ("%d %d", &img->largura, &img->altura);
    scanf ("%d", &img->cor_max);

    aloca_matriz (img);

    // verifica se o tipo for P3
    if (strcmp (img->tipo, "P3") == 0) {
        for (i = 0; i < img->altura; i++) {
            for (j = 0; j < (img->largura * 3); j++) {
            scanf ("%d", &img->pixels[i][j]);
            }
        }
    }
    else {
        unsigned char pixel;
        scanf ("%c", &pixel);
        for (i = 0; i < img->altura; i++) {
            for (j = 0; j < (img->largura * 3); j++) {
            scanf ("%c", &pixel);
            img->pixels[i][j] = pixel;
            }
        }
    }

    return;
}

// calcula as medias das cores e salva na propria struct da pastilha
void calcula_media (imagem *img, int ini_alt, int ini_lar, int altura, int largura) {
    int i,j;
    unsigned long int red_m = 0;
    unsigned long int green_m = 0;
    unsigned long int blue_m = 0;


    for (i = ini_alt; i < (altura + ini_alt); i++) {
        for (j = ini_lar * 3; j < (((largura + ini_lar) * 3)); j+=3) {
            red_m = red_m + img->pixels[i][j];
            green_m = green_m + img->pixels[i][j+1];
            blue_m = blue_m + img->pixels[i][j+2];
        }
    }

    img->red_m = (red_m / (altura * largura));
    img->green_m = (green_m / (altura * largura));
    img->blue_m = (blue_m / (altura * largura));
    
    return;
} 

// retorna a distancia de cor entre as duas imagens
float red_mean (imagem *img, imagem *pastilha_atual) {
    float delta_r, delta_g, delta_b;
    float r_linha;
    float distancia = 0;

    delta_r = img->red_m - pastilha_atual->red_m;
    delta_g = img->green_m - pastilha_atual->green_m;
    delta_b = img->blue_m - pastilha_atual->blue_m;

    r_linha = (img->red_m + pastilha_atual->red_m) / 2;

    delta_r = delta_r * delta_r;
    delta_g = delta_g * delta_g;
    delta_b = delta_b * delta_b;

    distancia = ((2 + (r_linha / 256)) * delta_r) + (4 * delta_g) + ((2 + ((255 - r_linha) / 256) * delta_b));
    distancia = sqrt (distancia);

    return distancia;
}

// troca o pedaço da imagem pela pastilha com cor parecida
void troca_imagens (imagem *img, pastilhas *pastilha, int ini_alt, int ini_lar) {
    int i,j;
    float mais_prox = 1000;
    float aux;
    int indice = 0;
    int i_alt = 0;
    int i_lar = 0;

    // acha a apstilha certa
    calcula_media (img, ini_alt, ini_lar, pastilha->vetor[0]->altura, pastilha->vetor[0]->largura);
    for (i = 0; i < pastilha->tam; i++) {
        aux = red_mean (img, pastilha->vetor[i]);
        if (aux < mais_prox) {
            mais_prox = aux;
            indice = i;
        }
    }

    // faz a troca dos pixeis
    for (i = ini_alt; i < (pastilha->vetor[0]->altura + ini_alt); i++) {
        for (j = ini_lar * 3; j < (((pastilha->vetor[0]->largura  + ini_lar) * 3)) ; j++) {
            img->pixels[i][j] = pastilha->vetor[indice]->pixels[i_alt][i_lar];
            i_lar++;
        }
        i_lar = 0;
        i_alt++;
    }
}

// corta a imagem grande e substitui pelas pastilhas
void cria_mosaico (imagem *img, pastilhas *pastilha) {
    int altura = pastilha->vetor[0]->altura;
    int largura = pastilha->vetor[0]->largura;
    int max_altura = img->altura;
    int max_largura = img->largura;
    int i,j;
    int i_total, j_total;


    for (i = 0; i <= (max_altura - altura); i+= altura) {
        for (j = 0; j <= (max_largura - largura); j+= largura) {
            troca_imagens (img, pastilha, i, j);
        }
    }

    i_total = i;
    j_total = j;

    // se faltou pixeis embaixo ele copia os ultimos pixeis de cima
    if (i_total < max_altura) {
        int inicio;
        inicio = i_total;
        for (i = inicio; i < max_altura; i++) {
            for (j = 0; j < max_largura * 3; j+=3) {
                img->pixels[i][j] = img->pixels[i - altura][j];
                img->pixels[i][j+1] = img->pixels[i - altura ][j+1];
                img->pixels[i][j+2] = img->pixels[i - altura ][j+2];
            }
        }
    }

    // se faltou pixeis na direita ele copia os ultimos pixeis da esquerda
    if (j_total < max_largura) {
        int inicio;
        inicio = j_total * 3;
        for (i = 0; i < max_altura; i++) {
            for (j = inicio; j < max_largura * 3; j+=3) {
                img->pixels[i][j] = img->pixels[i][j - (largura * 3) - 3];
                img->pixels[i][j+1] = img->pixels[i][j - (largura * 3) - 2];
                img->pixels[i][j+2] = img->pixels[i][j - (largura * 3) - 1];
            }
        }
    }

    return;
}

void imprimir_imagem (imagem *img, FILE *arquivo) {
    int i,j;

    fprintf (arquivo, "%s\n", img->tipo);
    fprintf (arquivo, "%d %d\n", img->largura, img->altura);
    fprintf (arquivo, "%d\n", img->cor_max);

    if (strcmp (img->tipo, "P3") == 0) {
        for (i = 0; i < img->altura; i++) {
            for (j = 0; j < (img->largura * 3); j++) {
            fprintf (arquivo, "%d ", img->pixels[i][j]);
            }
        fprintf (arquivo, "\n");
        }
    }
    else {
        for (i = 0; i < img->altura; i++) {
            for (j = 0; j < (img->largura * 3); j++) {
            fprintf (arquivo, "%c", img->pixels[i][j]);
            }
        }
    }

    return;
}

void imprimir_imagem_padrao (imagem *img) {
    int i,j;

    printf ("%s\n", img->tipo);
    printf ("%d %d\n", img->largura, img->altura);
    printf ("%d\n", img->cor_max);

    if (strcmp (img->tipo, "P3") == 0) {
        for (i = 0; i < img->altura; i++) {
            for (j = 0; j < (img->largura * 3); j++) {
            printf ("%d ", img->pixels[i][j]);
            }
        printf ("\n");
        }
    }
    else {
        for (i = 0; i < img->altura; i++) {
            for (j = 0; j < (img->largura * 3); j++) {
            printf ("%c", img->pixels[i][j]);
            }
        }
    }

    return;
}

void destroi_matriz (imagem *img) {
    int i;


    for (i = 0; i < img->altura; i++) {
        free (img->pixels[i]);
    }

    free (img->pixels);

    return;
}

void destroi_pastilhas (pastilhas *pastilha) {
    int i;

    for (i = 0; i < pastilha->tam; i++) {
        destroi_matriz (pastilha->vetor[i]);
        free (pastilha->vetor[i]);
    }

    return;
}