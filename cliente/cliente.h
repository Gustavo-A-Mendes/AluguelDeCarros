/* TAD cliente (nome, data_aluguel, duracao, carro, prox_cliente) */

/* Dependência de módulo */
#include <stdio.h>
#include "../aluguel/aluguel.h"

/* Tipo exportado */
typedef struct cliente Cliente;

/* Funções Exportadas */

/* Função cliente_cadastra
    cria um novo cadastro de cliente, preenchendo com seus dados pessoais
*/
Cliente *cliente_cadastra(int tag, Cliente *cli, char *nome, char *doc, char *tel);

/* Função cliente_exclui
    exclui o cadastro do cliente
*/
Cliente *cliente_exclui(Cliente *cli, char *dado);

/* Função cliente_doc
    retorna o valor de documento do cliente
*/
char *cliente_doc(Cliente *cli);

/* Função cliente_aluga
    inicia o processo de alguel de carro
*/
void cliente_aluga(Cliente *cli, Carro* carro);

/* Função cliente_busca_filtro
    a partir de um dado de busca, retorna lista de
    elementos que o contenham parcialmente ou por completo
*/
Cliente *cliente_busca(Cliente *cli, char* dado_busca);

/* Função cliente_consulta
    verifica os detalhes de registro do cliente (dados pessoais e aluguel)
*/
void cliente_consulta(Cliente *cli);

/* Função cliente_lista
    imprime um lista dos clientes cadastrados
*/
Cliente *cliente_lista(Cliente *cli);

/* Função cliente_edita
    abre sessão de edição, para modificar os dados pessoais
    do cliente
*/
void cliente_edita(Cliente *cli);

/* Função cliente_total

*/
int cliente_total();

/* Função cliente_cria_historico
    cria um arquivo de histórico para o cliente
*/
void cliente_cria_historico(Cliente *cli, char *doc);

/* Função cliente_atualiza_historico
    atualiza os dados pessoais do cliente no histórico
*/
void cliente_atualiza_historico(int tag, Cliente *cli, char* doc);

/* Função cliente_recupera_historico
    recarrega a lista de clientes no início da execução do programa
*/
Cliente *cliente_recupera_historico(Cliente *cli, Carro *carro, char *doc);

/* Função cliente_apaga_historico
    apaga histórico de um cliente (que foi apagado)
*/
void cliente_apaga_historico(Cliente *cli);

/* Função cliente_registra
    armazena um resumo dos dados do cliente,
    para recuperar histórico na re-execução do código
*/
void cliente_registra(Cliente *cli);

/* Função cliente_ordena
    retorna a posição ordenada do novo cliente,
    na ordem alfabética
*/
Cliente *cliente_ordena(Cliente *cli, char *nome);

/* Função cliente_leia
    lê o registro dos clientes, e recupera os históricos
*/
Cliente *cliente_leia(Cliente *cli, Carro *carro);
