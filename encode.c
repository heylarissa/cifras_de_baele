#include "dictkey.h"
#include "encode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filehandler.h"

char *encodeMessage(int mflag, char message[], DICT dictKeys[])
{
    if (!mflag)
    {
        fprintf(stderr, "\nATENÇÃO: Argumento -m não consta.\nNão será possível codificar mensagem\n");
        exit(EXIT_FAILURE);
    }

    int position, key;
    FILE *msg;
    msg = fopen(message, "r");
    checkFileOpening(msg);
    char c = fgetc(msg), *output = malloc(sizeof(char) * (LINESIZE + 1));

    while (c != EOF)
    {
        position = searchChar(c, dictKeys);
        key = getRandomKey(dictKeys, position);
        char str[10];
        sprintf(str, "%d", key); // copia inteiro para string
        strcat(output, str);
        strcat(output, " ");
        c = fgetc(msg);
    }

    fclose(msg);

    return output;
}

int getRandomKey(DICT dictKeys[], int position)
{
    KEYS *list;
    list = dictKeys[position].keysList;
    int key, tam = getListSize(list);

    position = rand() % tam;

    for (int i = 0; i <= position; i++)
    {
        if (i == position)
            key = list->key;

        list = list->next;
    }

    return key;
}