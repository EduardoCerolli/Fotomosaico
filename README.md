# Fotomosaico

O programa recebe uma imagem grande e gera um fotomosaico a apartir dessa imagem usando varias pastilhas (amostras de imagens bem pequenas).

Para fazer isso ele corta a imagem grande em varias no tamanho da pastilha, e calcula as cores para substituir esse pedaço pela pastilha com cor mais parecida.

# Arquivos

O arquivo principal é o "mosaico.c", os arquivos lib sao a biblioteca de funções do programa.

# Como usar

Para rodar o programa você tem algumas opções de incialização:

    "-h" ou "-help" te mostra uma mensagem de ajuda explicando como usar o programa;
    "-i" serve para passar a imagem de entrada;
    "-o" serve para passar a imagem de saida;
    "-p" serve para indicar o diretorio com as pastilhas;