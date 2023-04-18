#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define TRUE 1
#define FALSE 0
#define LINESIZE 1024

typedef struct keys
{
    /* Armazena as chaves */
    int value;
    struct keys *next;
} KEY;

typedef struct keysDict
{
    /* Armazema o relacionamento da letra com as chaves possíveis */
    char letter;
    KEY *keys;
    struct keysDict *next;
} DICT;

void checkFileOpening(FILE *file)
{
    if (!file)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
}

void initDictNode(DICT *dictionary)
{
    dictionary = malloc(sizeof(DICT));
    dictionary->keys = malloc(sizeof(KEY));
}

// Adiciona uma nova chave à lista
KEY *appendKey(KEY *keys, int newKey)
{
    KEY *newNode;
    newNode = malloc(sizeof(KEY));

    newNode->value = newKey;
    newNode->next = keys;

    return newNode;
}

DICT *createNode(char letter, int key, DICT *dictionary)
{
    DICT *newDict, *aux;

    newDict = malloc(sizeof(DICT));
    newDict->keys = malloc(sizeof(KEY));

    aux = dictionary;

    // TESTE criar novo nodo

    while (aux && aux->letter != letter)
        aux = aux->next;

    /*
    Procura letra no dicionário
    Se encontrar appendKey
    Senão cria novo nodo
    */

    if (aux)
    {
        printf("%d %c\n", key, letter);

        // aux->keys = appendKey(aux->keys);
    }
    else
    {
        newDict->keys->value = key;
        newDict->letter = key;
        newDict->next = dictionary;
    }

    return newDict;
}

// Lê o arquivo e carrega as chaves na memória
DICT *loadKeys()
{
    DICT *dictionary;
    dictionary = malloc(sizeof(DICT));
    dictionary->keys = malloc(sizeof(KEY));

    FILE *livroCifra;

    char word[LINESIZE + 1];

    livroCifra = fopen(optarg, "r");
    checkFileOpening(livroCifra);

    int key = 0;
    while (!feof(livroCifra))
    {
        fscanf(livroCifra, "%s[^\n]", word);
        dictionary = createNode(word[0], key, dictionary);

        key++;
    }

    fclose(livroCifra);

    return dictionary;
}

int main(int argc, char *argv[])
{
    // codificar
    //./beale  -e  -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves

    // decodificar
    //./beale  -d  -i MensagemCodificada  -c ArquivoDeChaves  -o MensagemDecodificada
    //./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada

    DICT *dictionary;

    int option;
    int encode = FALSE;

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
            dictionary = loadKeys();

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