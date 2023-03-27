#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[]){

    int option;

    while ((option = getopt(argc, argv, "e:b:m:o:c:d:i"))!=-1){
        switch (option){
            case 'e':
            // enconde
                printf("ok");
                break;
            case 'd':
            //decode
                break;
            case 'b':
            // livro cifra
                break;
            case 'm':
            // mensagem original;
                break;
            case 'o':
            // mensagem codificada
                break;
            case 'c':
            //decode
                break;
            case 'i':
            //decode
                break;

        }
    }

}