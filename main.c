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
    }
}

void writeFile(DICT dictKeys[], char *optarg)
{
    fprintf(stdout, "Writing keys file: %s\n", optarg);
    FILE *keysFile;
    keysFile = fopen(optarg, "w+");

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

int searchChar(char letter, DICT dict[])
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
    int position = searchChar(letter, dictKeys);

    if (position == -1) // não encontrou a chave no dicionario, preenche uma nova posição no vetor
    {
        int l = tolower(letter); // insere na posicao correspondente ao código ASCII da letra minúscula, por isso já fica ordenado
        dictKeys[l].p = l;
        dictKeys[l].keysList->key = key;
    }
    else
    {
        dictKeys[position].keysList = createNode(key, dictKeys[position].keysList);
    }
}

int getListSize(KEYS *list)
{
    int tam = 0;
    KEYS *aux;
    aux = list;

    while (aux != NULL)
    {
        aux = aux->next;
        tam++;
    }

    return tam;
}

// pega chave aleatória de uma determinada letra do dicionário
int getRandomKey(DICT dictKeys[], int position)
{

    KEYS *list;
    list = dictKeys[position].keysList;
    int key;
    int tam = getListSize(list);

    position = rand() % tam;

    for (int i = 0; i <= position; i++)
    {
        if (i == position)
        {
            key = list->key;
        }
        list = list->next;
    }
    printf("key escolhida: %d\n", key);
    return key;
}

char *encodeMessage(char message[], DICT dictKeys[])
{
    int position;
    int key;
    char output[LINESIZE];

    for (int i = 0; i < strlen(message); i++)
    {

        if (isspace(message[i]))
        {
            strcat(output, "-1 ");
        }
        else
        {
            position = searchChar(message[i], dictKeys);
            key = getRandomKey(dictKeys, position);

            char str[10];
            sprintf(str, "%d", key); // copia inteiro para string
            strcat(output, str);
            strcat(output, " ");
        }
    }

    return strdup(output);
}

void generateOutputFile(char optarg[], char encodedMessage[])
{
    FILE *output;
    output = fopen(optarg, "w+");

    fputs(encodedMessage, output);

    fclose(output);
}

void loadKeyFile(DICT dictKeys[], char optarg[])
{
    FILE *keyFile;
    char word[LINESIZE + 1];

    keyFile = fopen(optarg, "r");
    checkFileOpening(keyFile);

    int key = 0;
    while (!feof(keyFile))
    {
        fscanf(keyFile, "%s[^\n]", word);
        insertKey(dictKeys, key, word[0]);
        key++;
    }
    fclose(keyFile);
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

    initDictKeys(dictKeys);

    int option;
    int encode = FALSE;
    char word[LINESIZE + 1];

    char output[LINESIZE];

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
        printf("%s\n", optarg);
            strcpy(output, encodeMessage(optarg, dictKeys));
            fprintf(stdout, "%s\n", output);
            break;

        case 'o': // cria arquivo de saída
            generateOutputFile(optarg, output);
            break;

        case 'c': // escreve arquivo de chaves
            if (encode)
            {
                writeFile(dictKeys, optarg);
            }
            else
            {
                loadKeyFile(dictKeys, optarg);
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