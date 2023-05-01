#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "dictkey.h"
#include "decode.h"
#include "encode.h"
#include "filehandler.h"
#include <string.h>

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