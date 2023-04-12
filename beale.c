#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[])
{
    // codificar
    //./beale  -e  -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves
    // decodificar
    //./beale  -d  -i MensagemCodificada  -c ArquivoDeChaves  -o MensagemDecodificada
    //./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada

    FILE *f;

    int option;
    int encode = FALSE;

    while ((option = getopt(argc, argv, "e:b:m:o:c:d:i")) != -1)
    {
        switch (option)
        {
        case 'e':
            // encode
            printf("%s", optarg);
            break;
        case 'd':
            // decode
            printf("%s", optarg);
            break;
        case 'b':
            printf("%s", optarg);

            // livro cifra
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
        }
    }
}