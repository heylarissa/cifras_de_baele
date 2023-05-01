#ifndef __ENCODE__
#define __ENCODE__

#include "dictkey.h"

// Codifica mensagem
char *encodeMessage(int mflag, char message[], DICT dictKeys[]);

// Pega chave aleatória de uma determinada letra do dicionário
int getRandomKey(DICT dictKeys[], int position);

#endif