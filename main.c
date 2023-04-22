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
void tostring(char str[], int num)
{
    for (int i = 0; i < 10; i++)
    {
        str[i] = num + '0';
    }
}

void writeFile(DICT dictKeys[], char *optarg)
{
    fprintf(stdout, "Writing keys file: %s\n", optarg);
    FILE *keysFile;
    keysFile = fopen(optarg, "w+");

    // int fputc   (int c, FILE* stream)
    for (int i = 0; i <= QTTCARACTERES; i++)
    {
        if (dictKeys[i].p != 0)
        {
            fputc(dictKeys[i].p, keysFile);
            fputs(": ", keysFile);

            KEYS *list = dictKeys[i].keysList;
            while (list != NULL)
            {

                char str[10];
                sprintf(str, "%d", list->key); // copia inteiro para string
                fputs(str, keysFile);
                fputs(" ", keysFile);
                list = list->next;
            }
            fputs("\n", keysFile);
        }
    }

    fclose(keysFile);
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

KEYS *createNode(int key, KEYS *keyList)
{
    KEYS *aux;
    aux = malloc(sizeof(KEYS));
    aux->key = key;
    aux->next = keyList;
    return aux;
}

void insertKey(DICT dictKeys[], int key, char letter)
{
    int position = searchKey(letter, dictKeys);

    if (position == -1) // não encontrou a chave no dicionario, preenche uma nova posição no vetor
    {
        int l = tolower(letter); // insere na posicao correspondente ao código ASCI da letra minúscula, por isso já fica ordenado
        dictKeys[l].p = l;
        dictKeys[l].keysList->key = key;
    }
    else
    {
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



            break;

        case 'm': // mensagem original;
            printf("%s", optarg);

            break;
        case 'o': // mensagem codificada
            break;

        case 'c': // escreve arquivo de chaves
            if (encode)
            {
                writeFile(dictKeys, optarg);
            }
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