#include <stdlib.h>
#include <stdio.h>

int main (){

    FILE *file;

    file = fopen("ISOteste.txt", "r");

    while (!feof(file)){
        char c;
        c = fgetc(file);
        if (c != '\n' && c != 32) printf("%d ", c);
    }
}