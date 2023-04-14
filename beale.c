#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define TRUE 1
#define FALSE 0
#define LINESIZE 1024

typedef struct value
{
    /* lista com valores referentes a letra*/
    int value; // Código correspondente à letra
    struct value *next;
} VALUE;

typedef struct dict
{
    /* lista que contem a cifra completa */
    char key;     // letra
    VALUE *codes; // lista de códigos possíveis
    struct dict *next;
} DICT;


DICT *createNode(int code, char key, DICT *cifra){
    // verifica se já existe a key no dicionário
    // se existe:
        // insere valor no final da lista de códigos
    // se nao:
        // cria novo nó na lista de cifras

    DICT *newNode = (DICT *)malloc(sizeof(DICT));
    newNode->codes = malloc(sizeof(VALUE));
    newNode->key = key;
    newNode->codes->value = code;
    newNode->next = cifra;

    return newNode;
}

int main(int argc, char *argv[])
{
    // codificar
    //./beale  -e  -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves

    // decodificar
    //./beale  -d  -i MensagemCodificada  -c ArquivoDeChaves  -o MensagemDecodificada
    //./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada

    FILE *livroCifra;

    DICT *cifra;
    cifra = malloc(sizeof(DICT));
    cifra->codes = malloc(sizeof(VALUE));

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

            livroCifra = fopen(optarg, "r");
            if (!livroCifra)
            {
                perror("Erro ao abrir arquivo");
                exit(1); // encerra o programa com status 1
            }

            int i = 0;
            while (!feof(livroCifra))
            {
                fscanf(livroCifra, "%s[^\n]", word);
                // Criar nodo da lista cifra
                // Enquanto cria nodo da lista cifra, criar nodo da lista de values
                cifra = createNode(i, word[0], cifra);
                i++;
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
    int i = 0;

    while (cifra->next!= NULL){
        printf("%d: %c\n", i, cifra->key);
        cifra = cifra->next;
        i++;
    }
}