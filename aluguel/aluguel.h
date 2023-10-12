/* TAD: aluguel */

/* Dependência de módulo */
// #include "../cliente/cliente.h"
#include "../carro/carro.h"

/* Tipo exportado */
typedef struct aluguel Aluguel;

/* Funções exportadas */

/* Função aluguel_cria
    Cria um novo elemento na lista de aluguel, 
    inserindo-o de forma ordenada
*/
Aluguel *aluguel_cria(Aluguel* aluguel, Carro* carro, char *data, int duracao, int status);

/* Função aluguel_libera
    libera a memória alocada para a lista de aluguel
*/
void aluguel_libera(Aluguel *aluguel);

/* Função aluguel_imprime
    exibe as informações contidas no aluguel
*/
void aluguel_imprime(Aluguel *aluguel);

/* Função aluguel_imprime_historico
    exibe as informações de todos os aluguéis
    armazenados no histórico do cliente
*/
void aluguel_imprime_historico(Aluguel *aluguel, int *historico);

/* Função aluguel_atualiza_historico
    responsável por adicionar as novas informações ao histórico
    do cliente referentes aos alugueis feitos por ele
*/
void aluguel_atualiza_historico(Aluguel *aluguel, FILE *fl);

/* Função aluguel_ordena
    retorna a posição ordenada do novo aluguel,
    na ordem cronológica
*/
Aluguel *aluguel_ordena(Aluguel *aluguel, char *data_inicio);

/* Função aluguel_data_fim
    retorna o ultimo dia do aluguel ativo
*/
char *aluguel_data_fim(Aluguel *aluguel);

/* Função aluguel_finaliza
    atualiza a situação dos status de aluguel 
    e do carro
*/
void aluguel_finaliza(Aluguel *aluguel);
