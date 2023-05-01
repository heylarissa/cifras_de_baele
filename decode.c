#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictkey.h"
#include "filehandler.h"
#include "decode.h"

void loadKeyFile(DICT dictKeys[], char filename[])
{
    FILE *keyFile;
    char word[LINESIZE + 1];

    keyFile = fopen(filename, "r");
    checkFileOpening(keyFile);

    int letter, key;
    while (fscanf(keyFile, "%s", word) != EOF)
    {
        if (strchr(word, ':') != NULL) // se possui : é uma letra
        {
            letter = word[0];
            dictKeys[letter].p = letter; // insere na posição do dicionario correspondente ao caracter
        }
        else
        {
            key = atoi(word);
            int tam = getListSize(dictKeys[letter].keysList);
            if (tam == 1 && dictKeys[letter].keysList->key == 0)
                dictKeys[letter].keysList->key = key; // remover 0 extra da lista
            else
                dictKeys[letter].keysList = createNode(key, dictKeys[letter].keysList);
        }
    }
    fclose(keyFile);
}

void decodeOriginalMessage(DICT dictKeys[], int iflag, char message[], char decoded[])
{
    if (!iflag)
    {
        fprintf(stderr, "ERRO: É necessário uma mensagem codificada, em formato de string ou arquivo para execução do programa\n\n");
        exit(EXIT_FAILURE);
    }

    int key;
    char aux[2], output[LINESIZE + 1], word[LINESIZE + 1];
    memset(output, 0, LINESIZE + 1);
    memset(word, 0, LINESIZE + 1);

    FILE *f;

    f = fopen(message, "r");
    checkFileOpening(f);

    while (!feof(f))
    {

        if (fscanf(f, "%s", word) != EOF)
        {
            key = atoi(word);

            aux[0] = searchKey(dictKeys, key);
            if (aux[0] == -1) // tratamento de erro para chave inválida
                fprintf(stderr, "Não existe nenhuma letra que corresponda a chave %d\n\n", key);
            aux[1] = '\0';

            strcat(output, aux);
        }
    }
    strcpy(decoded, output);
}
