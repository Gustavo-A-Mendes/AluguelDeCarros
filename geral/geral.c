#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../cliente/cliente.c"
#include "../carro/carro.h"
#include "geral.h"

#define ATRASO 150
char *data_hoje = "01/01/2023";       /* dia inicial da simulação */
// int alert_cod = 0;

char *clear(void)
{    
    // Checking for windows OS with
    // _WIN32 macro
    #ifdef _WIN32
        return "cls";

    // Checking for mac OS with
    // __APPLE__ macro
    #elif __APPLE__
        return "clear";

    // Checking for linux OS with
    // __linux__ macro
    #elif __linux__
        return "clear";
    #endif
}

void cabecalho(char *titulo)
{
    printf("=======================================================\n");
    printf("   ALUGUEL DE CARROS\t%s\t%s\n", titulo, data_hoje);
    printf("=======================================================\n");
    printf(">>> [0] PASSAR TEMPO\n\n");
}

int menu_principal(Cliente* cli){
    int op1;
    
    system(clear());
    cabecalho("MENU INICIAR");
    printf(">>> [1] CLIENTE\n");
    printf(">>> [2] VEICULO\n");
    printf(">>> [3] SAIR\n");

    alert_msg();
    printf("\nEscolha uma opcao: ");
    op1 = teste_input();

    switch (op1) {
        case '0':
            data_hoje = passa_tempo(data_hoje);
            cliente_atualiza_aluguel(cli, data_hoje);

            break;

        case '1':
            printf("\nAbrindo Menu dos Clientes...\n");
            delay(ATRASO);
            break;
        
        case '2':
            printf("\nAbrindo Menu dos Carros...\n");
            delay(ATRASO);
            break;

        case '3':
            printf("\nEncerrando programa...\n");
            delay(ATRASO);
            break;

        default:    
            alert(1);
            break;
    }

    return op1;
}

Cliente *menu_cliente(Cliente *cli, Carro *carro)
{
    int op2, op_i;

    char dado[32];
    int tipo, count, escolha;
    int total_anterior, total_atual;

    char nome[32], doc[13], tel[13];
    Cliente *cliente_aux = NULL;
    Cliente *Busca = NULL;

    do {
        system(clear());
        
        cabecalho("MENU CLIENTE");
        
        printf(">>> [1] ALUGAR\n"); // submenu: adicionar cliente            
        printf(">>> [2] LISTAR\n"); // submenu: historico do cliente
        printf(">>> [3] BUSCAR\n"); // submenu: editar cliente e historico
        printf(">>> [4] CONSULTAR QUANTIDADE\n");    
        printf(">>> [5] REMOVER CLIENTE\n");    
        printf(">>> [6] VOLTAR\n");    
        
        alert_msg();
        printf("\nEscolha uma opcao: ");

        // Menu do Cliente:
        op2 = teste_input();

        switch (op2)
        {
            case '0':
                data_hoje = passa_tempo(data_hoje);
                cliente_atualiza_aluguel(cli, data_hoje);

                break;

            case '1':
                printf("\nIniciando Cadastro de Aluguel...\n");
                delay(ATRASO);

                system(clear());

                printf("1 - Novo Cadastro\n");      /* Cria um novo cadastro antes de iniciar o aluguel */
                printf("2 - Cliente Existente\n");  /* Cliente já está cadastrado no sistema */
                printf("\nEscolha uma opcao: ");

                op_i = teste_input();

                if (op_i == '1')          /* criar cadastro */
                {      
                    printf("\nCriando Cadastro...\n");
                    delay(ATRASO);
                    
                    system(clear());
                    
                    total_anterior = cliente_total(cli);    /* total de clientes antes do cadastro */

                    printf("Digite o Nome Completo: ");
                    scanf(" %31[^\n]", nome);
                    while (getchar() != '\n');
                    if (strlen(nome) > 30)                  /* verifica se o nome é válido */
                    {
                        alert(3);
                        break;
                    }

                    printf("Informe o numeros do CPF: ");
                    scanf(" %12[^\n]", doc);
                    while (getchar() != '\n');
                    if (teste_formato(doc))                 /* verifica se o CPF é válido */
                    {
                        if (strlen(doc) != 11) 
                        {
                            alert(4);
                            break;
                        }
                    }
                    else
                    {
                        alert(5);
                        break;
                    }

                    printf("Informe os numeros de um telefone para contato: ");
                    scanf(" %12[^\n]", tel);
                    while (getchar() != '\n');
                    if (teste_formato(tel))                 /* verifica se o telefone é válido */
                    {
                        if (strlen(tel) != 11)
                        {
                            alert(6);
                            break;
                        }
                    } else {
                        alert(7);
                        break;
                    }

                    // ponteiro auxiliar endereça para lista com novo cadastro:
                    cli = cliente_cadastra(1, cli, nome, doc, tel, 0);

                    total_atual = cliente_total(cli);   /* total de clientes após o cadastro */

                    // ==================================================
                    // caso o cadastro tenho sido realizado com sucesso,
                    // procura novo cliente dentro da nova lista:
                    if (total_atual != total_anterior)
                        cliente_aux = cliente_busca(cli, doc);
                    else
                    {
                        alert(-3);      /* Cadastro falhou */
                        break;
                    }

                }
                else if (op_i == '2')   /* busca cliente cadastrado no sistema */
                {
                    if (cli != NULL)    /* Verifica se a lista está vazia */
                    {
                        system(clear());
                        cliente_aux = cliente_lista(cli);
                    }
                    else
                    {
                        alert(-5);      /* Nao ha cadastros no sistema */
                        break;
                    }
                }
                else
                {
                    alert(1);
                    break;
                }

                // printf("\n:)\n");
                // Inicia o cadastro do aluguel:
                if (cliente_aux != NULL)
                {
                    if (cliente_aux->status == 0)
                        cliente_aluga(cliente_aux, carro, data_hoje);
                    else
                        alert(-2);  /* há um aluguel ativo */
                }
                else
                {
                    alert(-3);   /* cadastro cancelado */
                }
                delay(ATRASO);
                break;
            
            case '2':
                printf("\nListando Clientes Cadastrados...\n");
                delay(ATRASO);
                
                system(clear());
                
                while (1)
                {
                    cliente_aux = cliente_lista(cli);
                    if (cliente_aux != NULL)
                    {
                        system(clear());
                        if (cliente_consulta(cli, cliente_aux) == 0) break;
                    }
                    else break;
                }
                break;

            case '3':
                printf("\nBuscando Cliente...\n");
                delay(ATRASO);

                system(clear());
                
                printf("Insira o nome ou o CPF do cliente: ");
                scanf(" %30[^\n]", dado);
                while (getchar() != '\n');
                
                while (1)
                {
                    cliente_aux = cliente_filtra_busca(cli, dado);
                    if (cliente_aux != NULL)
                    {
                        system(clear());
                        if (cliente_consulta(cli, cliente_aux) == 0) break;
                    }
                    else break;
                }
                break;

            case '4':
                printf("\nConsultando Quantitativo...\n");
                delay(ATRASO);

                total_atual = cliente_total(cli);
                printf("Ha %d clientes cadastrados.\n", total_atual);
                delay(3000);       /* atraso para verificar resposta */
                break;
            
            case '5':
                printf("\nApagando Conta de Cadastro...\n");
                delay(ATRASO);

                system(clear());

                if (cli != NULL)    /* verifica se há cadastros no sistema */
                {
                    printf("Insira o nome ou o CPF do cliente: ");
                    scanf(" %30[^\n]", dado);
                    while (getchar() != '\n');

                    cliente_aux = cliente_filtra_busca(cli, dado);
                    if (cliente_aux != NULL)
                    {
                        cli = cliente_exclui(cli, cliente_doc(cliente_aux));
                    }
                }
                else
                {
                    alert(-5);      /* Nao ha cadastros no sistema */
                    delay(ATRASO);
                }
                break;

            case '6':
                printf("\nVoltando ao Menu Inicial...\n");
                delay(ATRASO);
                break;

            default:
                alert(1);
                delay(ATRASO);
                break;
        }
    } while (op2 != '6');

    return cli;
}

Carro *menu_carro(Cliente *cli, Carro *carro)
{
    int op3;
    Carro *carro_aux;

    do {
        system(clear());

        // Menu do Cliente:
        cabecalho("MENU CARRO");

        printf(">>> [1] ADICIONAR\n"); 
        printf(">>> [2] LISTAR\n"); //submenu: consultar disp. e consultar historico
        printf(">>> [3] CONSULTAR CARROS\n"); //Consultar se está disponivel 
        printf(">>> [4] VOLTAR\n");

        alert_msg();
        printf("\nEscolha uma opcao: ");

        op3 = teste_input();

        switch (op3) {
            case '0':
                data_hoje = passa_tempo(data_hoje);
            case '1':
                printf("\nAdicionando Carro ao Sistema...\n");
                delay(ATRASO);
                break;
            
            case '2':
                printf("\nListando Carros...\n");
                delay(ATRASO);

                system(clear());

                while (1)
                {
                    // Listando os carros da galeria
                    //============================================
                    carro_aux = carro_lista(carro);
                    if (carro_aux != NULL)
                    {
                        system(clear());
                        if(carro_consulta(carro, carro_aux) == 0) break;
                    }
                    else break;
                }
                break;

            case '3':
                printf("\nConsultando informacoes do Carro...\n");
                delay(ATRASO);
                break;

            case '4':
                printf("\nVoltando ao Menu Inicial...\n");
                delay(ATRASO);
                break;

            default:    
                printf("\nInsira uma opcao valida!\n");
                break;
        }
    } while (op3 != '4');

    return carro; 
}

long long data_para_num(char *data)
{
    int dia, mes, ano;
    int tempo_dia = 0;

    // quantidade de dias de cada mês:
    //               J   F   M   A   M   J   J   A   S   O   N   D 
    int meses[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);     /* separa os valores da data */

    tempo_dia += (ano-1) * 365;     /* converte ano em dias */
    

    /* Conversão de meses em dias */
    int i;
    for (i = 0; i < (mes-1); i++)
    {
        tempo_dia += meses[i];      /* incrementa os dias de cada mês completo */
    }
    
    tempo_dia += (dia-1);       /* soma os dias no resultado do cálculo */

    return tempo_dia;
}

char *num_para_data(long long data)
{
    int ano = 0, mes = 0, dia = 0;
    int dia_mes;

    // quantidade de dias de cada mês:
    //               J   F   M   A   M   J   J   A   S   O   N   D 
    int meses[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    ano = (data / 365);
    
    /* conversão de dias em meses */
    dia_mes = (data % 365);
    while (dia_mes >= meses[mes])    /* verifica os meses, de janeiro a dezembro */
    {
        dia_mes -= meses[mes];      /* decrementa a quantidade de dias do mês */
        mes++;                      /* contabiliza os meses completos */
    }

    dia = dia_mes;                  /* o resto do cálculo será o dia */

    char *data_format = (char*)malloc(11*sizeof(char));
    sprintf(data_format, "%02d/%02d/%04d", dia+1, mes+1, ano+1);

    return data_format;
}

int data_valida(char *data)
{
    int dia, mes, ano;
    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
    if (dia == 0 || mes == 0 || ano == 0)   /* verifica se a data é nula */
        return 0;
    
    // verificação do dia máximo de cada mês:
    if (dia > 31 || mes > 12)               /* valores máximos de dia e mês */
        return 0;
    else if (mes == 2 && dia > 28)
        return 0;
    else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30)
        return 0;

    return 1;
}

int compara_data(char *data1, char *data2)
{
    // converte as datas de string em valor numérico:
    int data1_num = data_para_num(data1);
    int data2_num = data_para_num(data2);

    // compara as datas:
    if (data1_num > data2_num)          /* data2 vem antes de data1 */
        return -1;
    else if (data1_num < data2_num)     /* data2 vem depois de data1 */
        return 1;
    else                                /* as datas são iguais */
        return 0;
}

char *prazo(char *data, long long duracao)
{   
    // converte data de início para valor numérico:
    long long data_ini = data_para_num(data);
    // soma a duração do aluguel:
    long long data_fim = data_ini + duracao;

    // retorna a data final do aluguel:
    return num_para_data(data_fim);
}

int compara(char *str, char *str_busca)
{
    /* Função que faz a comparação entre a string e a string
       da busca a partir do seu tamanho
    */
    return strncmp(str, str_busca, strlen(str_busca)); 
}

char *realoca_string(char *dado)
{
    dado = (char*)realloc(dado, strlen(dado)*sizeof(char));
    return dado;
}

void mascara(char *dado, char *dado_convertido, char formato[])
{
    char dado_base[100];
    strcpy(dado_base, dado);        /* recebe o valor de entrada */
    char *aux = (char*)malloc(100*sizeof(char));    /* string auxiliar para montar a máscara */

    int i = 0;      /* índice geral */
    int id = 0;     /* índice do valor de entrada */

    while (dado_base[id] != '\0')
    {
        if (formato[i] != '#')      /* adiciona a máscara */
        {
            aux[i] = formato[i];
            i++;
        }
        else        /* adiciona os valores do valor de entrada */
        {
            aux[i] = dado_base[id];
            id++;
            i++;
        }
    }
    aux[i] = '\0';      /* fecha a string */
    strcpy(dado_convertido, aux);   /* copia resultado no parâmetro "dado_convertido" */
    free(aux);      /* libera memória da variável auxiliar */
}

int teste_formato(char *str)
{
    int i;
    int negativo = 0;
    for (i = 0; str[i] != '\0'; i++)    /* verifica cada caracter */
    {
        if (!(str[i] >= '0' && str[i] <= '9'))  /* verifica se o caracter é numérico */
        {   
            if (i == 0 && str[i] == '-')
                negativo++;
            else
                return 0;   /* é string */
        }
    }
    if (negativo == 1)
    {
        printf("oi\n");
        return -1;          /* é número negativo */
    }

    return 1;               /* é número positivo */
}

int teste_input(void) // editar função
{
    char teste[100];
    int i = 0;
    while ((teste[i] = getchar()) != '\n') i++;
    teste[i] = '\0';
    int len = strlen(teste);

    if (len == 0)
        return '\n';
    else if(len == 1)    /* é esperado input com apenas 1 caracter */
        return toupper(teste[0]);

    return 0;       /* input invalido */
}

char *input_data(void)
{
    int dia, mes, ano;
    char str_dia[100], str_mes[100], str_ano[100];
    char *data = (char*)malloc(11*sizeof(char));
    int i;

    i = 0;
    while ((data[i] = getchar()) != '\n') i++;
    data[i] = '\0';

    sscanf(data, "%[^/]/%[^/]/%[^\n]\n", str_dia, str_mes, str_ano);
    // printf("Digite o dia: ");
    // i = 0;
    // while ((str_dia[i] = getchar()) != '\n') i++;
    // str_dia[i] = '\0';
    
    // printf("Digite o mes: ");
    // i = 0;
    // while ((str_mes[i] = getchar()) != '\n') i++;
    // str_mes[i] = '\0';

    // printf("Digite o ano: ");
    // i = 0;
    // while ((str_ano[i] = getchar()) != '\n') i++;
    // str_ano[i] = '\0';

    if (teste_formato(str_dia) == 1 && teste_formato(str_mes) == 1 && teste_formato(str_ano) == 1)
    {
        if (strlen(str_dia) == 2 && strlen(str_mes) == 2 && strlen(str_ano) == 4)
        {
            // convertendo para inteiro:
            dia = atoi(str_dia);
            mes = atoi(str_mes);
            ano = atoi(str_ano);
            sprintf(data, "%02d/%02d/%04d", dia, mes, ano);

            if (data_valida(data) == 1)
                return data;
        }
    }

    free(data);
    alert(2);                  /* formato inválido */
    return NULL;
}

char *passa_tempo(char *data_antiga)
{
    system(clear());

    char ch_dias[5];
    int i, op_data;
    long long dias;
    char *data_nova = NULL;
    
    do
    {
        system(clear());
        
        printf(">>> [1] Avancar Dias\n");
        printf(">>> [2] Inserir Data\n");
        printf(">>> [3] Voltar\n");

        alert_msg();
        printf("\nEscolha uma opcao: ");
        op_data = teste_input();

        switch (op_data)
        {
            case '1':
                system(clear());

                printf("Quantos dias quer avancar?\n");
                
                // scanf(" %[^\n]", ch_dias);
                // while (getchar() != '\n');
                i = 0;
                while ((ch_dias[i] = getchar()) != '\n') i++;
                ch_dias[i] = '\0';
                if (teste_formato(ch_dias) != 0)    /* o valor é numérico */
                {
                    dias = atoll(ch_dias);
                    data_nova = prazo(data_antiga, dias);
                    if (compara_data(data_antiga, data_nova) >= 0)
                    {
                        alert(-1);   /* data atualizada */
                        return data_nova;
                    }
                    alert(-9);  /*não pode ir pro passado */
                    free(data_nova);
                    return data_antiga;
                }
                else alert(2);  /* formato inválido */
                return data_antiga;
                break;
            
            case '2':
                system(clear());

                printf("Insira a nova data [dd/mm/aaaa]:\n");
                data_nova = input_data();
                if (data_nova != NULL)
                {
                    if (compara_data(data_antiga, data_nova) >= 0)
                    {
                        alert(-1);   /* data atualizada */
                        return data_nova;
                    }
                    alert(-9);  /*não pode ir pro passado */
                    free(data_nova);
                    return data_antiga;
                }
                else
                {
                    alert(2);   /* formato inválido */
                }
                break;

            case '3':
                alert(0);   /* voltando ao menu, sem mensagem de erro */
                break;

            default:
                alert(1);   /* opção inválida */
                break;
        }
    } while (op_data != '3');
    
    free(data_nova);
    return data_hoje;
}

char *string_upper(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        str[i] = toupper(str[i]);
    }

    return str;
}

void alert(int cod)
{
    alert_cod = cod;
}

void alert_msg(void)
{
    // mensagem limpa, sem erro:
    if (alert_cod == 0) printf("\n");
    
    // alerta de formato: 
    else if (alert_cod == 1) printf(TXT_yellow"\nInsira uma opcao valida!"TXT_reset);
    else if (alert_cod == 2) printf(TXT_red"\nFormato invalido!"TXT_reset);
    else if (alert_cod == 3) printf(TXT_red"\nErro! Tamanho maximo excedido."TXT_reset);
    else if (alert_cod == 4) printf(TXT_yellow"\nTamanho invalido! O CPF deve conter 11 digitos."TXT_reset);
    else if (alert_cod == 5) printf(TXT_yellow"\nO CPF deve conter apenas numeros."TXT_reset);
    else if (alert_cod == 6) printf(TXT_yellow"\nTamanho invalido! O telefone deve conter 11 digitos."TXT_reset);
    else if (alert_cod == 7) printf(TXT_yellow"\nO telefone deve conter apenas numeros."TXT_reset);
    // alerta de processo:
    else if (alert_cod == -1) printf(TXT_green"\nData atualizada!"TXT_reset);
    else if (alert_cod == -2) printf(TXT_red"\nO cliente possui um aluguel ativo!"TXT_reset);
    else if (alert_cod == -3) printf(TXT_red"\nCadastro cancelado!"TXT_reset);
    else if (alert_cod == -4) printf(TXT_green"\nCadastro apagado!"TXT_reset);
    else if (alert_cod == -5) printf(TXT_red"\nNao ha clientes cadastrados no sistema."TXT_reset);
    else if (alert_cod == -6) printf(TXT_red"\nERRO! Cliente nao encontrado."TXT_reset);
    else if (alert_cod == -7) printf(TXT_red"\nArquivo nao encontrado!"TXT_reset);
    else if (alert_cod == -8) printf(TXT_red"\nNao foi possivel concluir o cadastro"TXT_reset);
    else if (alert_cod == -9) printf(TXT_red"\nNao e possivel selecionar uma data anterior."TXT_reset);
    else if (alert_cod == -10) printf(TXT_red"\nNao ha alugueis no historico."TXT_reset);
    else if (alert_cod == -11) printf(TXT_green"\nAluguel criado!"TXT_reset);
    else if (alert_cod == -12) printf(TXT_red"\nAluguel falhou!"TXT_reset);
    else if (alert_cod == -13) printf(TXT_green"\nDado(s) Atualizado(s)!"TXT_reset);
    else if (alert_cod == -14) printf(TXT_green"\nCarro retirado do sistema com sucesso!"TXT_reset);

    alert(0);    /* reseta marcador */
}

void delay(double milissegundos)
{
    // Armazenando tempo inicial:
    clock_t tempo_inicial = clock();
    
    // looping till required time is not achieved
    while (milissegundos > clock() - tempo_inicial);
}

void registra(int data_hoje, Cliente *cli)
{
    FILE *fl = fopen("registro.txt", "wt");
    // verifica se o arquivo foi aberto corretamente:
    if (fl == NULL) 
    {
        printf("\nArquivo nao encontrado!\n");
        return; // erro ao acessar o arquivo
    }

    // ==================================================
    // escreve cabeçalho:
    fprintf(fl, "%s\t%s\t%s\n", "NOME", "CPF", "STATUS");

    // ==================================================
    // escreve os dados dos clientes:
    Cliente *cliente_aux;
    for (cliente_aux = cli; cliente_aux != NULL; cliente_aux = cliente_aux->prox_cliente)
    {
        fprintf(fl, "%s\t%s\t%d\n", cliente_aux->nome, cliente_aux->documento, cliente_aux->status);
    }
    fclose(fl);
}