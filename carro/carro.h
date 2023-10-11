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
Carro *carro_busca(Carro *carro, char *modelo, int tipo);

/* Função carro_libera

*/
void carro_libera(Carro *carro);

/* Função carro_lista

*/
Carro *carro_lista(Carro *carro);

/* Função carro_aluga

*/
// Carro *carro_aluga(Carro *carro, Cliente *cliente);

/* Função carro_lista

*/
Carro *carro_lista_disponivel(Carro *carro);

/* Função carro_disponivel

*/
void carro_disponivel(Carro *carro);

/* Função carro_alugado

*/
void carro_alugado(Carro *carro);

/* Função carro_ordena

*/
Carro *carro_ordena(Carro *carro, char *modelo);

/* Funçaõ carro_leia

*/
Carro *carro_leia(Carro *carro);

/* Função carro_imprime

*/
void carro_imprime(Carro *carro, int *qtd_carros);

/* Função carro_consulta

*/
int carro_consulta(Carro *carro, Carro *carro_consultado);

/* Função carro_edita

*/
carro_edita(carro, carro_consultado);