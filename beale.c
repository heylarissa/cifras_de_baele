#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define TRUE 1
#define FALSE 0
#define LINESIZE 1024

typedef struct value {
    /* lista com valores referentes a letra*/
    int value;                  // Código correspondente à letra
    struct values *next;
} VALUE;

typedef struct cifra {
    /* lista que contem a cifra completa */
    char key;                   // letra
    VALUE *codes;               // lista de códigos possíveis
    struct cifra *next;
} CIFRA;

int main(int argc, char *argv[])
{
    // codificar
    //./beale  -e  -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves

    // decodificar
    //./beale  -d  -i MensagemCodificada  -c ArquivoDeChaves  -o MensagemDecodificada
    //./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada

    FILE *livroCifra;

    CIFRA *cifra;
    cifra = malloc(sizeof(CIFRA));

    int option;
    int encode = FALSE;
    char word[LINESIZE + 1];

    while ((option = getopt(argc, argv, "edbc:m:o:i:")) != -1)
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
            if (encode == TRUE) // tirar
            {
                livroCifra = fopen(optarg, "r");
                if (!livroCifra)
                {
                    perror("Erro ao abrir arquivo x");
                    exit(1); // encerra o programa com status 1
                }

                // lê as 10 primeiras linhas do arquivo
                int i = 0;
                while (!feof(livroCifra))
                {
                    fscanf(livroCifra, "%s[^\n]", word);
                    printf("%d: %c\n", i, word[0]);
                    i++;
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