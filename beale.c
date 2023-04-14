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

void insertCode(char key, DICT *cifra, int code){
    DICT *temp;
    temp = cifra;
    VALUE *codesTemp;
    codesTemp = malloc(sizeof(VALUE));
    while (temp->next!=NULL){
        if (key == temp->key) {
            // insere codigo na lista de códigos da chave
            codesTemp = temp->codes;
            while (codesTemp->next!= NULL){
                codesTemp = codesTemp->next;
            }
            codesTemp->value = code;
            return;
        }
        else {

        }
    }
    return;
}

DICT *createNode(int code, char key, DICT *cifra){
    DICT *temp;
    temp = malloc(sizeof(DICT));
    temp = NULL;
    temp->codes = malloc(sizeof(VALUE));

    temp = cifra;

    insertCode(key, temp, code);

    temp->key = key;
    temp->codes->value = code;
    printf("%d: %c\n", temp->codes->value, temp->key);
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
}