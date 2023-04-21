#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define LINESIZE 1024
#define QTTCARACTERES 255

typedef struct key
{
    int key;
    struct key *next;
} KEYS;

typedef struct dict
{
    char p;
    KEYS *keysList;
} DICT;
void checkFileOpening(FILE *file)
{
    if (!file)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
}

void initDictKeys(DICT dictKeys[])
{
    for (int i = 0; i <= QTTCARACTERES; i++)
    {
        dictKeys[i].p = 0;
        dictKeys[i].keysList = malloc(sizeof(KEYS));
        ;
    }
}

int searchKey(char letter, DICT dict[])
{
    for (int i = 0; i <= QTTCARACTERES; i++)
    {
        if (dict[i].p == toupper(letter) || dict[i].p == tolower(letter))
        {
            return i;
        }
    }
    return -1;
}

int getDictTam(DICT dict[])
{
    int i = 0;
    char aux = dict[0].p;
    while (aux != 0 && i < QTTCARACTERES)
    {
        i++;
        aux = dict[i].p;
    }

    return i;
}

KEYS *createNode (int key, KEYS *keyList){
    KEYS *aux;
    aux = malloc (sizeof(KEYS));
    aux->key = key;
    aux->next = keyList;

    return aux;

}

void insertKey(DICT dictKeys[], int key, char letter)
{
    int position = searchKey(letter, dictKeys);

    if (position == -1) // não encontrou a chave no dicionario, preenche uma nova posição no vetor
    {
        int tam = getDictTam(dictKeys);
        dictKeys[tam].p = letter;
        dictKeys[tam].keysList->key = key;
    }
    else
    {
        printf("\nadiciona chave %d na posicao %d\n", key, position);

        dictKeys[position].keysList = createNode(key, dictKeys[position].keysList);
    }
}

int main(int argc, char *argv[])
{
    // codificar
    //./beale  -e  -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves

    // decodificar
    //./beale  -d  -i MensagemCodificada  -c ArquivoDeChaves  -o MensagemDecodificada
    //./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada

    FILE *livroCifra;
    DICT dictKeys[QTTCARACTERES + 1];
    int dictTam;
    initDictKeys(dictKeys);

    printf("\n\n");
    int option;
    int encode = FALSE;
    char word[LINESIZE + 1];

    while ((option = getopt(argc, argv, "edb:c:m:o:i:")) != -1)
    {
        switch (option)
        {
        case 'e': // encode
            encode = TRUE;

            break;

        case 'd': // decode
            encode = FALSE;
            break;

        case 'b': // recebe livro cifra

            livroCifra = fopen(optarg, "r");
            checkFileOpening(livroCifra);

            int key = 0;
            while (!feof(livroCifra))
            {
                fscanf(livroCifra, "%s[^\n]", word);
                insertKey(dictKeys, key, word[0]);
                key++;
            }
            fclose(livroCifra);

            printf("\nDicionario criado\n");
            for (int i = 0; i <= QTTCARACTERES; i++)
            {
                printf("%c: ", dictKeys[i].p);
                KEYS *list = dictKeys[i].keysList;
                while (list!= NULL){
                    printf("%d ", list->key);
                    list = list->next;
                }
                printf("\n");
            }

            break;

        case 'm': // mensagem original;
            printf("%s", optarg);

            break;
        case 'o': // mensagem codificada
            break;

        case 'c': // decode
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