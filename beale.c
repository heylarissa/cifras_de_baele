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
    char line[LINESIZE + 1];
    while ((option = getopt(argc, argv, "edb:m:o:c:i:")) != -1)
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
            if (encode == TRUE)
            {
                livroCifra = fopen(optarg, "r");
                if (!livroCifra)
                {
                    perror("Erro ao abrir arquivo x");
                    exit(1); // encerra o programa com status 1
                }

                // lê as 10 primeiras linhas do arquivo
                for (int i = 0; i < 10; i++)
                {
                    fscanf(livroCifra, "%s[^\n]", line);
                    printf("%d: %s\n", i, line);
                }
            }
            fclose(livroCifra);

            break;
        case 'm':
            printf("%s", optarg);

            // mensagem original;
            break;
        case 'o':
            // mensagem codificada
            break;
        case 'c':
            // decode
            break;
        case 'i':
            // decode
            break;

        case '?':
            // mensagem de erro
            break;
        }
    }
}