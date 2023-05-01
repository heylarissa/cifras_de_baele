#include "filehandler.h"
#include "dictkey.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkFileOpening(FILE *file)
{
    if (!file)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
}

void generateOutputFile(int oflag, int encode, char filename[], char encodedMessage[])
{
    if (!oflag && encode) // encode
    {
        fprintf(stderr, "\nATENÇÃO: Argumento -o não consta.\nArquivo com mensagem codificada não será gerado\n");
    }
    else if (!oflag && !encode) // decode
    {
        fprintf(stderr, "\nATENÇÃO: Argumento -o não consta.\nArquivo com mensagem decodificada não será gerado\n");
        exit(EXIT_FAILURE);
    }

    FILE *output;
    output = fopen(filename, "w+");
    checkFileOpening(output);
    fputs(encodedMessage, output);
    fclose(output);
}

void writeFile(int cflag, DICT dictKeys[], char *optarg)
{
    if (!cflag)
        fprintf(stderr, "\nATENÇÃO: Argumento -c não consta.\nArquivo com chaves não será gerado\n");

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
