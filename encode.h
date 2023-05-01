#ifndef __ENCODE__
#define __ENCODE__

#include "dictkey.h"

// Codifica mensagem
void encodeMessage(int mflag, char message[], DICT dictKeys[], char out[]);

// Pega chave aleatória de uma determinada letra do dicionário
int getRandomKey(DICT dictKeys[], int position);

#endif