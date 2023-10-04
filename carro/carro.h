/* TAD carro (codigo, disponibilidade, modelo, cliente) */

/* Dependência de módulo */
#include <stdio.h>
// #include "../cliente/cliente.h"

/* Tipo exportado */
typedef struct carro Carro;

/* Funções Exportadas */

/* Função carro_cadastra
    
*/
Carro *carro_cadastra(Carro *carro, char *modelo, char *placa, float preco);

/* Função carro_busca

*/
Carro *carro_busca(Carro *carro, char *modelo);

/* Função carro_libera

*/
void carro_libera(Carro *carro);

/* Função carro_aluga

*/
// Carro *carro_aluga(Carro *carro, Cliente *cliente);

/* Função carro_lista

*/
void carro_lista(Carro *carro);

/* Função carro_disponivel

*/
void carro_disponivel(Carro *carro);

/* Função carro_alugado

*/
void carro_alugado(Carro *carro);

/* Função carro_busca

*/
Carro *carro_busca(Carro *carro, char *placa);

/* Função carro_ordena

*/
Carro *carro_ordena(Carro *carro, char *modelo);

/* Funçaõ carro_leia

*/
Carro *carro_leia(Carro *carro);

