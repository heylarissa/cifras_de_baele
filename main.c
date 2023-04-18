#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define TRUE 1
#define FALSE 0
#define LINESIZE 1024
int main(int argc, char *argv[])
{
    // codificar
    //./beale  -e  -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves

    // decodificar
    //./beale  -d  -i MensagemCodificada  -c ArquivoDeChaves  -o MensagemDecodificada
    //./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada

    FILE *livroCifra;

    int option;
    int encode = FALSE;
    char word[LINESIZE + 1];

    while ((option = getopt(argc, argv, "edb:c:m:o:i:")) != -1)
    {
        switch (option)
        {
        case 'e':
            // encode
            encode = TRUE;

            break;

        case 'd':
            // decode
            encode = FALSE;
            break;

        case 'b':
            // livro cifra

            // gera as chaves a partir do livro cifra

            livroCifra = fopen(optarg, "r");
            if (!livroCifra)
            {
                perror("Erro ao abrir arquivo");
                exit(1); // encerra o programa com status 1
            }

            while (!feof(livroCifra))
            {
                fscanf(livroCifra, "%s[^\n]", word);
            }
            fclose(livroCifra);

            break;

        case 'm':
            printf("%s", optarg);

            // mensagem original;
            // a partir das chaves criadas codifica a mensagem
            
            break;

        case 'o':
            // mensagem codificada
            printf("%s", optarg);

            break;

        case 'c':
            // gera arquivo de chaves se encode == TRUE
            // senao, usa arquivo de chaves para decodificar uma mensagem
            break;

        case 'i':
            // mensagem codificada
            break;

        case '?':
            // mensagem de erro
            break;
        }
    }
}