#include <stdio.h>
#include <string.h>

int main(void){

    int a, b, c;


    a = strncmp("Nada", "Nada", 4);
    b = strncmp("NADA", "nada", 4);
    c = strncmp("nada", "NADA", 4);

    printf("%d %d %d", a, b, c);

    return 0;
}