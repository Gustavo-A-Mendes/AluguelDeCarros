#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "./carro/carro.c"
// #include "./cliente/cliente.c"
#include "./geral/geral.c"
// #include "carro.h" // pensei em nao ser possivel modificar nada do carro, ento so o cliente.c ja seria suficiente 
// #include "cliente.c"


int main(void) {

    Cliente *clientes = NULL;
    Carro *carros = NULL;
    int op1 = 0, op2 = 0, op3 = 0;
    
    /* Recuperando dados salvos */
    registro_leia(&clientes, &carros);
    delay(ATRASO);     /* atraso para verificar resposta */

    // Menu principal:
    do {
        op1 = menu_principal(clientes);
        if (op1 == '1')
            clientes = menu_cliente(clientes, carros);
        else if (op1 == '2')
            carros = menu_carro(clientes, carros);
        
    } while (op1 != '3');
    
    cliente_libera(clientes);
    carro_libera(carros);
    
    printf("\ntchau");

    return 0;
}
