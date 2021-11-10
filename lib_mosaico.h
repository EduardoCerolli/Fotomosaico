// GRR20190397 Eduardo Henrique dos Santos Cerolli

// struct para armazenar a pastilha ou a imagem grande
typedef struct imagem {
    char tipo[2];
    int altura, largura, cor_max;
    float red_m, green_m, blue_m;
    int **pixels;
} imagem;

// struct que contem um vetor de structs imagens para guardar todas as pastilhas
typedef struct pastilhas {
    int tam;
    imagem **vetor;
} pastilhas;

void pula_comentario (FILE *arquivo);
void aloca_matriz (imagem *img);
void aloca_pastilhas (pastilhas *img, int tam);
void le_imagem (imagem *img, FILE *arquivo);
void le_imagem_padrao (imagem *img);
void calcula_media (imagem *img, int ini_alt, int ini_lar, int altura, int largura);
float red_mean (imagem *img, imagem *pastilha_atual);
void troca_imagens (imagem *img, pastilhas *pastilha, int ini_alt, int ini_lar);
void cria_mosaico (imagem *img, pastilhas *pastilha);
void imprimir_imagem (imagem *img, FILE *arquivo);
void imprimir_imagem_padrao (imagem *img);
void destroi_matriz (imagem *img);
void destroi_pastilhas (pastilhas *pastilha);