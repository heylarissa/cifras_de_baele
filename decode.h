#ifndef __DECODE__
#define __DECODE__

#include "dictkey.h"

// Carrega arquivo de chaves na memória
void loadKeyFile(DICT dictKeys[], char filename[]);

// Decodifica a mensagem a partir do arquivo de entrada
void decodeOriginalMessage(DICT dictKeys[], int iflag, char message[], char decoded[]);

#endif