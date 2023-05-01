#ifndef __FILEHANDLER__
#define __FILEHANDLER__

#define LINESIZE 1024

#define TRUE 1
#define FALSE 0

#include "dictkey.h"
#include <stdio.h>

// verifica abertura de arquivo
void checkFileOpening(FILE *file);

// Escreve arquivo de saída
void generateOutputFile(int oflag, int encode, char filename[], char encodedMessage[]);

// Escreve arquivo de chaves
void writeFile(int cflag, DICT dictKeys[], char *optarg);

#endif