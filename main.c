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
        if (dict[i].p == toupper(letter) || dict[i].p == tolower(letter)) // considerar maiscula e minuscula
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

void insertKey(DICT dictKeys[], int key, char letter)
{
    // busca chave no vetor
    // se existir retorna posição da letra
    // insere key na lista
    // printf("%c ", letter);

    int position = searchKey(letter, dictKeys);

    if (position == -1) // não encontrou a chave no dicionario, preenche uma nova posição no vetor
    {
        int tam = getDictTam(dictKeys);
        dictKeys[tam].p = letter;
        dictKeys[tam].keysList->key = key;
        printf("ADICIONEI em %d\n", tam + 1);
    }
    else
    {
        printf("\nadiciona na posicao %d\n", position);
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
                printf("%c ", dictKeys[i].p);
            }

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