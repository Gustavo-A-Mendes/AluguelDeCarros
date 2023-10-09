/* Módulo geral */

/* Dependência de módulo */
#include "../cliente/cliente.h"
// #include "../carro/carro.h"

/* Funções Exportadas */

/* Função clear
    limpa o terminal de acordo com o SO do usuário
*/
char *clear(void);

/* Função cabecalho
    Adiciona o cabeçalho do menu atual
*/
void cabecalho(char *titulo);

/* Função menu_principal 
    Exibe o menu principal
*/
int menu_principal(void);

/* Função menu_cliente
    Exibe o menu cliente
*/
Cliente *menu_cliente(Cliente *cli, Carro *carro);

/* Função menu_carro
    Exibe o menu carro
*/
Carro *menu_carro(Carro *carro);

/* Função data_para_num
    Converte uma string de data em seu correspondente numérico
*/
int data_para_num(char *data);

/* Função num_para_data
    Converte um valor numérico em uma string de data
*/
char *num_para_data(int data);

/* Função data_valida
    Verifica se a data inseria é válida
*/
int data_valida(char *data);

/* Função compara_data
    Faz a comparação entre duas datas.
    Retorna 1 se a primeira data vem antes,
    -1 se for a segunda, e 0 se forem iguais.
*/
int compara_data(char *data1, char *data2);

/* Função calcula_tempo
    Calcula a data final do prazo estipulado
*/
char *prazo(char *data, int duracao);

/* Função compara
    Faz a comparação entre as strings
*/
int compara(char *nome, char *nome_busca);

/* Função realoca_string
    Realoca o espaço de memória utilzado por uma string
*/
char *realoca_string(char *dado);

/* Função mascara
    rescreve uma string dentro do formato definido em "formato[]"
*/
void mascara(char *dado, char *dado_convertido, char formato[]);

/* Função teste_formato
    retorna 0 para caso de ser string, 1 se for inteiro positivo e -1 se for negativo.
*/
int teste_formato(char *str);

/* Função teste_input
    Verifica se o valor é uma string, número positivo ou negativo
*/
int teste_input(void);

/* Função input_data
    Solicita o dia, mês e ano e retorna uma data
*/
char *input_data(void);

/* Função passa_tempo
    Muda a data atual definida pelo programa, 
    para simular a passagem de tempo
*/
char *passa_tempo(char *data_antiga);

/* Função string_upper
    Converte os caracteres de uma string para 
    o seu correspondente maiúsculo
*/
char *string_upper(char *str);

/* Função alert
    Modifica o código de erro atual do sistema
*/
void alert(int cod);

/* Função alert_msg
    Exibe uma mensagem de erro referente ao código de erro salvo no sistema
*/
void alert_msg(void);

/* Função delay
    Gera um atraso, em milissegundos, na resposta do programa
*/
void delay(double milissegundos);
