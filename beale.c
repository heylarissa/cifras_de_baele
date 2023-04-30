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

// Cerifica abertura de arquivo
void checkFileOpening(FILE *file)
{
    if (!file)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
}

// Aloca espaço no dicionario
void initDictKeys(DICT dictKeys[])
{
    for (int i = 0; i <= QTTCARACTERES; i++)
    {
        dictKeys[i].p = 0;
        dictKeys[i].keysList = malloc(sizeof(KEYS));
    }
}

// Escreve arquivo de chaves
void writeFile(int cflag, DICT dictKeys[], char *optarg)
{
    if (!cflag) // gera arquivo de chaves somente se houver opção -c
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
                // fwrite(str, sizeof(char), sizeof(DICT)*QTTCARACTERES, keysFile);
                fputs(str, keysFile);
                fputs(" ", keysFile);
                list = list->next;
            }
            fputs("\n", keysFile);
        }
    }

    fclose(keysFile);
}

// Busca um determinado caracter no dicionario. Retorna -1 caso não encontre, ou a posição no dicionario caso encontre.
int searchChar(char letter, DICT dict[])
{
    for (int i = 0; i < QTTCARACTERES; i++)
        if (dict[i].p == toupper(letter) || dict[i].p == tolower(letter))
            return i;
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

// Insere um novo caractere ou chave no dicionário
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
        dictKeys[position].keysList = createNode(key, dictKeys[position].keysList);
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
            key = list->key;

        list = list->next;
    }

    return key;
}

// Codifica mensagem
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
    char c = fgetc(msg), *output = malloc(sizeof(char) * LINESIZE);

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

// Escreve arquivo de saída
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

// Completa dicionário com caracteres especiais da tabela ASCII
void completeDictKeys(DICT dictKeys[])
{
    int key = -1;
    for (int i = 32; i < 127; i++) // caracteres especiais da tabela ASCII
    {
        insertKey(dictKeys, key, i);
        key--;
    }
}

// Carrega arquivo de chaves na memória
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

// Cria dicionário de chaves a partir de livro cifra
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

// Retorna letra correspondente a chave ou -1 caso não encontre
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

// Decodifica a mensagem a partir do arquivo de entrada
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

// Libera memória do dicionário
void freeDict(DICT dict[])
{
    for (int i = 0; i < QTTCARACTERES; i++)
        free(dict[i].keysList);
}

int main(int argc, char *argv[])
{

    int option, encode = FALSE,
        cflag = FALSE, oflag = FALSE,
        bflag = FALSE, mflag = FALSE, iflag = FALSE;

    char output[LINESIZE],
        originalMsg[LINESIZE], outputFile[LINESIZE + 1],
        keysFile[LINESIZE + 1], livroFilename[LINESIZE + 1];

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
            if (encode)
            {
                mflag = TRUE;
                strcpy(originalMsg, optarg);
            }
            else
                fprintf(stderr, "ERRO: argumento -m só é usado com o parâmetro -e (encode)");
    
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
            if (!encode)
            {
                iflag = TRUE;
                strcpy(originalMsg, optarg);
            }
            else
            {
                fprintf(stderr, "ERRO: Argumento -i só é usado com o parâmetro -d (decode)");
            }
            break;

        default: // mensagem de erro
            fprintf(stderr, "\nCodificação:\n-e\n\n-b: livro cifra;\n-c: arquivo de chaves a ser escrito;\n-o: arquivo de saída para mensagem codificada; \n-m: mensagem original\n\nDecodificação:\n-d\n-b: livro cifra;\n-c: arquivo de chaves;\n-o: saída da mensagem decodificada;\n-i: entrada da mensagem codificada\n");
            exit(EXIT_FAILURE);

            break;
        }
    }

    DICT dictKeys[QTTCARACTERES + 1];
    initDictKeys(dictKeys);

    if (encode)
    {
        createDictKeys(bflag, livroFilename, dictKeys); // gera dicionário de chaves

        strcpy(output, encodeMessage(mflag, originalMsg, dictKeys)); // codifica mensagem
        fprintf(stdout, "Output: %s\n", output);
        writeFile(cflag, dictKeys, keysFile);
        generateOutputFile(oflag, encode, outputFile, output); // gera arquivo com mensagem codificada
    }
    else
    {
        if (cflag)
        {
            loadKeyFile(dictKeys, keysFile);
            completeDictKeys(dictKeys);
        }
        else
            createDictKeys(bflag, livroFilename, dictKeys);

        decodeOriginalMessage(dictKeys, iflag, originalMsg, output);
        fprintf(stdout, "Output: %s\n", output);
        generateOutputFile(oflag, encode, outputFile, output); // gera arquivo com mensagem decodificada
    }
    freeDict(dictKeys);
}