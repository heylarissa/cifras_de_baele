#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <stdint.h>
#include <locale.h>

#define TRUE 1
#define FALSE 0
#define LINESIZE 1024
#define QTTCARACTERES 256

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

// verifica abertura de arquivo
void checkFileOpening(FILE *file)
{
    if (!file)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
}

// aloca espaço no dicionario
void initDictKeys(DICT dictKeys[])
{
    for (int i = 0; i <= QTTCARACTERES; i++)
    {
        dictKeys[i].p = 0;
        dictKeys[i].keysList = malloc(sizeof(KEYS));
    }
}

// escreve arquivo de chaves
void writeFile(int cflag, DICT dictKeys[], char *optarg)
{
    if (cflag) // gera arquivo de chaves somente se houver opção -c
    {
        fprintf(stdout, "\nWriting keys file: %s\n", optarg);
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
    else
    {
        fprintf(stderr, "\nATENÇÃO: Argumento -c não consta.\nArquivo com chaves não será gerado\n");
    }
}

// Busca um determinado caracter no dicionario. Retorna -1 caso não encontre, ou a posição no dicionario caso encontre.
int searchChar(char letter, DICT dict[])
{
    for (int i = 0; i <= QTTCARACTERES; i++)
    {
        if (dict[i].p == toupper(letter) || dict[i].p == tolower(letter))
        {
            return i;
        }
    }
    return -1;
}

// Cria nodo na lista de chaves
KEYS *createNode(int key, KEYS *keyList)
{
    KEYS *aux;
    aux = malloc(sizeof(KEYS));
    aux->key = key;
    aux->next = keyList;
    return aux;
}

// Insere um novo caracter no dicionário
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
    {
        dictKeys[position].keysList = createNode(key, dictKeys[position].keysList);
    }
}

// Retorna o tamanho da lista
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

// Pega chave aleatória de uma determinada letra do dicionário
int getRandomKey(DICT dictKeys[], int position)
{

    KEYS *list;
    list = dictKeys[position].keysList;
    int key, tam = getListSize(list);

    position = rand() % tam;

    for (int i = 0; i <= position; i++)
    {
        if (i == position)
        {
            key = list->key;
        }
        list = list->next;
    }

    return key;
}

// Codifica mensagem
char *encodeMessage(int mflag, char message[], DICT dictKeys[])
{
    char *output = malloc(sizeof(char) * LINESIZE);

    if (mflag && message != NULL)
    {
        int position;
        int key;

        for (int i = 0; i < strlen(message); i++)
        {
            if (isspace(message[i]))
            {
                strcat(output, "-1 ");
            }
            else
            {
                position = searchChar(message[i], dictKeys);
                key = getRandomKey(dictKeys, position);

                char str[10];
                sprintf(str, "%d", key); // copia inteiro para string
                strcat(output, str);
                strcat(output, " ");
            }
        }
    }
    else
    {
        fprintf(stderr, "\nATENÇÃO: Argumento -m não consta.\nNão será possível codificar mensagem\n");
        exit(EXIT_FAILURE);
    }

    return output;
}

// Escreve arquivo de saída
void generateOutputFile(int oflag, char filename[], char encodedMessage[])
{
    if (oflag)
    {
        FILE *output;
        output = fopen(filename, "w+");
        checkFileOpening(output);
        fputs(encodedMessage, output);

        fclose(output);
    }
    else
    {
        fprintf(stderr, "\nATENÇÃO: Argumento -o não consta.\nArquivo com mensagem codificada não será gerado\n");
    }
}

// NÃO IMPLEMENTADA Carrega arquivo de chaves na memória
void loadKeyFile(DICT dictKeys[], char filename[])
{
    FILE *keyFile;
    char word[LINESIZE + 1];

    keyFile = fopen(filename, "r");
    checkFileOpening(keyFile);

    int key = 0;
    while (!feof(keyFile))
    {
        fscanf(keyFile, "%s[^\n]", word);
        printf("%s\n", word);
        // insertKey(dictKeys, key, word[0]); // arrumar
        key++;
    }
    fclose(keyFile);
    // printDictKeys(dictKeys);
}

void createDictKeys(int bflag, char *livroFilename, DICT dictKeys[])
{

    if (bflag)
    {
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
    }
    else
    {
        fprintf(stderr, "ERRO: Não existe argumento -b indicando arquivo do livro cifra.\n");
        exit(EXIT_FAILURE);
    }
}

void completeDictKeys(DICT dictKeys[])
{
    char toComplete[QTTCARACTERES];
    int key = -2;
    for (int i = 33; i < 127; i++)
    {
        char c[2];
        sprintf(c, "%d", i);
        printf("s: %s ", c);
        insertKey(dictKeys, key, i);
        
        key--;
    }
    printf("\n\n");
}

int main(int argc, char *argv[])
{
    // codificar
    //./beale  -e  -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves

    // decodificar
    //./beale  -d  -i MensagemCodificada  -c ArquivoDeChaves  -o MensagemDecodificada
    //./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada

    int option,
        encode = FALSE,
        cflag = FALSE,
        oflag = FALSE,
        bflag = FALSE,
        mflag = FALSE,
        iflag = FALSE;

    char output[LINESIZE],
        originalMsg[LINESIZE],
        outputFile[LINESIZE + 1],
        keysFile[LINESIZE + 1],
        livroFilename[LINESIZE + 1];

    while ((option = getopt(argc, argv, "edb:c:m:o:i:")) != -1)
    {
        switch (option)
        {
        case 'e': // encode
            encode = TRUE;
            break;

        case 'd': // decode
            encode = FALSE;
            break;

        case 'b': // recebe livro cifra
            bflag = TRUE;
            strcpy(livroFilename, optarg);
            break;

        case 'm': // mensagem original
            mflag = TRUE;
            strcpy(originalMsg, optarg);
            break;

        case 'o': // cria arquivo de saída
            oflag = TRUE;
            strcpy(outputFile, optarg);
            break;

        case 'c': // escreve arquivo de chaves
            cflag = TRUE;
            strcpy(keysFile, optarg);
            break;

        case 'i': // mensagem codificada
            iflag = TRUE;
            break;

        case '?': // mensagem de erro
            break;
        }
    }

    DICT dictKeys[QTTCARACTERES + 1];
    initDictKeys(dictKeys);

    if (encode)
    {
        createDictKeys(bflag, livroFilename, dictKeys); // gera dicionário de chaves
        completeDictKeys(dictKeys);                     // completa dicionário com caracteres inexistentes no livro cifra
        
        
        strcpy(output, encodeMessage(mflag, originalMsg, dictKeys)); // codifica mensagem
        fprintf(stdout, "%s\n", output);

        generateOutputFile(oflag, outputFile, output); // gera arquivo com mensagem codificada

        writeFile(cflag, dictKeys, keysFile);
    }
    else
    {
        if (cflag)
        {
            loadKeyFile(dictKeys, keysFile);
        }
        else
        {
            createDictKeys(bflag, livroFilename, dictKeys);
        }
    }
}