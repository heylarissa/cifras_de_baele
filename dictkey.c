#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictkey.h"
#include "filehandler.h"

void completeDictKeys(DICT dictKeys[])
{
    int key = -1;
    for (int i = 32; i < 127; i++) // caracteres especiais da tabela ASCII
    {
        insertKey(dictKeys, key, i);
        key--;
    }
}

void createDictKeys(int bflag, char *livroFilename, DICT dictKeys[])
{
    if (!bflag)
    {
        fprintf(stderr, "ERRO: Não existe argumento -b indicando arquivo do livro cifra.\n");
        exit(EXIT_FAILURE);
    }
    FILE *livroCifra;

    char word[LINESIZE + 1];
    int key = 0;

    livroCifra = fopen(livroFilename, "r");
    checkFileOpening(livroCifra);

    while (!feof(livroCifra))
    {
        fscanf(livroCifra, "%s[^\n]", word);
        insertKey(dictKeys, key, word[0]);
        key++;
    }

    fclose(livroCifra);
    completeDictKeys(dictKeys);
}

char searchKey(DICT dictKeys[], int key)
{
    char p;
    KEYS *aux;

    for (int i = 0; i < QTTCARACTERES; i++)
    {
        aux = dictKeys[i].keysList;
        p = dictKeys[i].p;

        while (aux)
        {
            if (key == aux->key)
                return p;
            aux = aux->next;
        }
    }

    return -1;
}

void freeDict(DICT dict[])
{
    for (int i = 0; i < QTTCARACTERES; i++)
        free(dict[i].keysList);
}

void initDictKeys(DICT dictKeys[])
{
    for (int i = 0; i <= QTTCARACTERES; i++)
    {
        dictKeys[i].p = 0;
        dictKeys[i].keysList = malloc(sizeof(KEYS));
    }
}

void createNode(int key, KEYS *keyList)
{
    KEYS *aux;
    aux = malloc(sizeof(KEYS));
    aux->key = key;
    aux->next = keyList;
    keyList = aux;
    free(aux);
}

void insertKey(DICT dictKeys[], int key, char letter)
{
    int position = searchChar(letter, dictKeys);

    if (position == -1) // não encontrou a letra no dicionario, preenche uma nova posição no vetor
    {
        int l = tolower(letter); // insere na posicao correspondente ao código ASCII da letra minúscula, por isso já fica ordenado
        dictKeys[l].p = l;
        dictKeys[l].keysList->key = key;
    }
    else if (key >= 0)
        createNode(key, dictKeys[position].keysList);
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

int searchChar(char letter, DICT dict[])
{
    for (int i = 0; i < QTTCARACTERES; i++)
        if (dict[i].p == toupper(letter) || dict[i].p == tolower(letter))
            return i;
    return -1;
}
