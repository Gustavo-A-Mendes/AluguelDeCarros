/* TAD: aluguel */

/* Dependência de módulo */
// #include "../cliente/cliente.h"
#include "../carro/carro.h"

/* Tipo exportado */
typedef struct aluguel Aluguel;

/* Funções exportadas */

/* Função aluguel_cria

*/
Aluguel *aluguel_cria(Aluguel* aluguel, Carro* carro, char *data, int duracao);

/* Função aluguel_libera

*/
void aluguel_libera(Aluguel *aluguel);

/* Função aluguel_imprime

*/
void aluguel_imprime(Aluguel *aluguel);

/* Função aluguel_historico

*/
void aluguel_atualiza_historico(Aluguel *aluguel, FILE *fl);

/* Função aluguel_ordena

*/
Aluguel *aluguel_ordena(Aluguel *aluguel, char *data_inicio);

/* Função aluguel_data_fim

*/
char *aluguel_data_fim(Aluguel *aluguel);

/* Função aluguel_finaliza

*/
void aluguel_finaliza(Aluguel *aluguel);
