#ifndef __DICTKEY__
#define __DICTKEY__

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

#define QTTCARACTERES 256

// Completa dicionário com caracteres especiais da tabela ASCII
void completeDictKeys(DICT dictKeys[]);

// Cria dicionário de chaves a partir de livro cifra
void createDictKeys(int bflag, char *livroFilename, DICT dictKeys[]);

// Retorna letra correspondente a chave ou -1 caso não encontre
char searchKey(DICT dictKeys[], int key);

// Libera memória do dicionário
void freeDict(DICT dict[]);

// Aloca espaço no dicionario
void initDictKeys(DICT dictKeys[]);

// Cria nodo na lista de chaves
KEYS *createNode(int key, KEYS *keyList);

// Insere um novo caractere ou chave no dicionário
void insertKey(DICT dictKeys[], int key, char letter);

// Retorna o tamanho da lista
int getListSize(KEYS *list);

// Busca um determinado caracter no dicionario. Retorna -1 caso não encontre, ou a posição no dicionario caso encontre.
int searchChar(char letter, DICT dict[]);

#endif