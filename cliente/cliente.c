#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../aluguel/aluguel.c"
// #include "../carro/carro.h"
#include "../geral/geral.h"
#include "cliente.h"

struct cliente
{
    char *nome;
    char *documento;
    char *telefone;

    Cliente *prox_cliente;
    Cliente *ant_cliente;
    
    int status;
    Aluguel *ultimo_aluguel;
};

Cliente *cliente_cadastra(int tag, Cliente *cli, char *nome, char *doc, char *tel)
{
    // aloca o espaço necessário para o cliente novo:
    Cliente *novo_cliente = (Cliente *)malloc(sizeof(Cliente));
    if (novo_cliente == NULL)
    {
        alert(-8);  /* cadastro não concluído */
        return cli;
    }

    novo_cliente->nome = (char *)malloc(31 * sizeof(char));
    novo_cliente->documento = (char *)malloc(15 * sizeof(char));
    novo_cliente->telefone = (char *)malloc(15 * sizeof(char));
    novo_cliente->ultimo_aluguel = NULL;

    // ==================================================
    // insere os dados do cliente:
    strcpy(novo_cliente->nome, strupr(nome));
    novo_cliente->nome = realoca_string(novo_cliente->nome);
    strcpy(novo_cliente->documento, doc);
    novo_cliente->documento = realoca_string(novo_cliente->documento);
    strcpy(novo_cliente->telefone, tel);
    novo_cliente->telefone = realoca_string(novo_cliente->telefone);
    novo_cliente->status = 0;   /* cadastra sem aluguel ativo */

    // ==================================================
    // encadea o endereço dos clientes:
    
    // endereço do elemento imediatamente antes do novo elemento, na ordem alfabética:
    Cliente *ref = cliente_ordena(cli, nome);
    if (ref == NULL)   /* verifica se o novo cadastro ficará na primeira posição da lista */
    {
        novo_cliente->prox_cliente = cli;
        novo_cliente->ant_cliente = NULL;

        if (cli != NULL)
            cli->ant_cliente = novo_cliente;
        
        cli = novo_cliente;
    }
    else
    {
        novo_cliente->prox_cliente = ref->prox_cliente;
        novo_cliente->ant_cliente = ref;
    
        if (ref->prox_cliente != NULL)    /* verifica se o novo cadastro é o último da lista*/
            ref->prox_cliente->ant_cliente = novo_cliente;
        
        ref->prox_cliente = novo_cliente;
    }

    // ==================================================
    // cria um arquivo de histórico, caso seja um novo cadastro:
    
    // verifica se é um cadastro novo:
    if (tag == 1)
    {
        cliente_cria_historico(novo_cliente, doc);
        cliente_registra(cli);
    }
    // printf("\nCadastro feito com sucesso!\n");
    // delay(1000);        /* atraso para verificar resposta */
    return cli;
}

Cliente *cliente_exclui(Cliente *cli, char *dado)
{
    int op;
    // procura o cliente do dado especificado:
    Cliente *cliente_excluido = cliente_busca(cli, dado);
    if (cliente_excluido == NULL)
        return cli;
            
    while (1)
    {
        system(clear());

        alert_msg();
        printf("\nO cadastro sera apagado. Deseja Continuar [S/N]?\n");
        op = teste_input();

        if (op == 'S')
        { 
            // ==================================================
            // retira elemento do encadeamento:
            if (cliente_excluido == cli) /* teste se é o primeiro elemento */
                cli = cliente_excluido->prox_cliente;
            else
                cliente_excluido->ant_cliente->prox_cliente = cliente_excluido->prox_cliente;

            if (cliente_excluido->prox_cliente != NULL)    /* teste se é o último elemento */
                cliente_excluido->prox_cliente->ant_cliente = cliente_excluido->ant_cliente;

            // ==================================================
            // apaga o arquivo de histórico de aluguel:
            cliente_apaga_historico(cliente_excluido);

            // ==================================================
            // libera o espaço de memória:
            free(cliente_excluido->nome);
            free(cliente_excluido->documento);
            free(cliente_excluido->telefone);
            aluguel_libera(cliente_excluido->ultimo_aluguel);
            free(cliente_excluido);
            
            alert(-4);      /* cadastro excluido */
            break;
        } 
        else if (op == 'N')
            break;
        else
            alert(1);
    }

    return cli;
}

void cliente_libera(Cliente *cli)
{
    Cliente *cliente_aux = cli;   /* ponteiro inicializado com a lista */
    Cliente *auxiliar;            /* ponteiro auxiliar */

    // ==================================================
    // laço de repetição, enquanto valor de "cliente_aux" não for [NULL] (Fim da lista):
    while (cliente_aux != NULL) 
    {
        auxiliar = cliente_aux->prox_cliente;
        free(cliente_aux->nome);
        free(cliente_aux->documento);
        free(cliente_aux->telefone);
        aluguel_libera(cliente_aux->ultimo_aluguel);
        free(cliente_aux);
        cliente_aux = auxiliar;
    }
}

char *cliente_doc(Cliente *cli)
{
    return cli->documento;
}

void cliente_aluga(Cliente *cli, Carro* carro, char *data_hoje)
{
    Carro *carro_aluga = carro_lista(carro);
    printf("%s\n", carro_aluga->modelo);
    char *data;
    int duracao;
    
    printf("Qual a Data do aluguel?\n");
    data = input_data();

    // data_hoje = num_para_data(hoje);
    if (compara_data(data_hoje, data) != -1)     /* data do aluguel não pode ser antes da data atual */
    {
        printf("Qual a duracao do aluguel?\n");
        scanf("%d", &duracao);
        while (getchar() != '\n');

        cli->ultimo_aluguel = aluguel_cria(cli->ultimo_aluguel, carro_aluga, data, duracao);
        printf("oi2\n");
        cliente_atualiza_historico(1, cli, cli->documento);
        cli->status = 1;
        alert(-11);
    }
    else
        alert(-9);  /* Data inválida */
}

void cliente_imprime(Cliente *cli)
{
    char cliente_doc[15], cliente_tel[15];

    mascara(cli->documento, cliente_doc, "###.###.###-##");
    mascara(cli->telefone, cliente_tel, "(##)#####-####");
    printf("%-30s\t%-15s\t%-15s\n", cli->nome, cliente_doc, cliente_tel);
}

Cliente *cliente_busca(Cliente *cli, char *dado_busca)
{
    Cliente *cliente_aux;
    for (cliente_aux = cli; cliente_aux != NULL; cliente_aux = cliente_aux->prox_cliente)
    {
        if (compara(cliente_aux->documento, dado_busca) == 0)
        return cliente_aux;
    }
    return NULL;
}

Cliente *cliente_filtra_busca(Cliente *cli, char *dado_busca)
{
    Cliente *cliente_aux, *cliente_escolha;
    char ch_escolha[31];
    int escolha, i, id_cliente;

    if (cli != NULL)
    {
        while (1)
        {
            system(clear());

            // ==================================================
            // exibe cabeçalho:
            printf("==========================================================================================\n");
            printf("%-3s\t%-30s\t%-15s\t%-15s\t%-10s\n", "ID", "NOME", "CPF", "TELEFONE", "STATUS");
            printf("==========================================================================================\n");

            // verifica o tipo de dado usado para a busca [nome/CPF]:
            int tipo = teste_formato(dado_busca);
            if (tipo == 0)  /* procura o elemento na lista pelo nome do cliente */
            {
                id_cliente = 0;
                for (cliente_aux = cli; cliente_aux != NULL; cliente_aux = cliente_aux->prox_cliente)   
                {
                    if (compara(cliente_aux->nome, string_upper(dado_busca)) == 0)
                    {
                        printf("%d\t", id_cliente);
                        cliente_imprime(cliente_aux);
                        id_cliente++;
                    }
                }
            }
            else            /* procura o elemento na lista pelo CPF do cliente */
            {
                id_cliente = 0;
                for (cliente_aux = cli; cliente_aux != NULL; cliente_aux = cliente_aux->prox_cliente)
                {
                    if (compara(cliente_aux->documento, dado_busca) == 0) {
                        printf("%d\t", id_cliente);
                        cliente_imprime(cliente_aux);
                        id_cliente++;
                    }
                }
            }
            
            printf("\nFoi encontrado %d resultado(s).\n", id_cliente);

            alert_msg();
            printf("\nDigite o ID do cliente para continuar (ou deixe em branco para voltar ao menu): ");
            
            int i = 0;
            while ((ch_escolha[i] = getchar()) != '\n') i++;
            ch_escolha[i] = '\0';
            if (strlen(ch_escolha) > 0)                 /* verifica se está vazio */
            {
                if (teste_formato(ch_escolha) != 0)     /* verifica se é um número */
                {
                    escolha = atoi(ch_escolha);
                    if (escolha >= 0 && escolha <= id_cliente-1)
                    {
                        cliente_escolha = cli;
                        for (i = 0; i <= escolha;)
                        while (1)
                        {
                            if (tipo == 0)  /* procura o elemento na lista pelo nome do cliente */
                            {
                                if (compara(cliente_escolha->nome, string_upper(dado_busca)) == 0)
                                {
                                    i++;
                                }
                            }
                            else            /* procura o elemento na lista pelo CPF do cliente */
                            {
                                if (compara(cliente_escolha->documento, string_upper(dado_busca)) == 0)
                                {
                                    i++;
                                }
                            }

                            if (i > escolha)
                                break;
                            
                            cliente_escolha = cliente_escolha->prox_cliente;
                        }
                        return cliente_escolha;
                    }
                }
                alert(1); /* valor inválido */
            }
            else
            {
                alert(0);    /* voltando para o menu (sem mensagem de erro) */
                break;
            }
        }
    }
    else
        alert(-6);          /* lista está vazia */

    // retorna o cliente escolhido:
    return NULL;
}

/* FALTA TERMINAR. ADICIONAR FERRAMENTAS DE EDIÇÃO */
void cliente_consulta(Cliente *cli, Cliente *consultado)
{
    int op_cons;
    char consultado_doc[15], consultado_tel[15];
    Aluguel *aluguel_aux, *aluguel_lista;

    while (1)
    {
        system(clear());

        mascara(consultado->documento, consultado_doc, "###.###.###-##");
        mascara(consultado->telefone, consultado_tel, "(##)#####-####");
        
        printf("==========================================================================================\n");
        printf("DADOS DO CLIENTE:\n");
        printf("==========================================================================================\n");
        printf("%-30s\t%-15s\t%-15s\t%-10s\n", "NOME", "CPF", "TELEFONE", "STATUS");
        printf("%-30s\t%-15s\t%-15s\t%-10s\n", consultado->nome, consultado_doc, consultado_tel, consultado->status ? "ATIVO" : "INATIVO");
        printf("\n==========================================================================================\n");
        printf("INFORMACOES DE ALUGUEL:\n");
        printf("==========================================================================================\n");
        
        if (consultado->status == 1)
        {
            aluguel_aux = consultado->ultimo_aluguel;
            aluguel_imprime(aluguel_aux);
        }
        else
            printf("Nao ha aluguel ativo.\n");
        

        printf("\n>>>[1] Editar\n");
        printf(">>>[2] Excluir\n");
        printf(">>>[3] Visualizar historico\n");
        printf(">>>[4] Voltar ao Menu\n");
        
        alert_msg();
        printf("\nEscolha uma opcao: ");
        op_cons = teste_input();

        switch (op_cons)
        {
        case '1':
            cliente_edita(cli, consultado);
            return;
        
        case '2':
            cliente_exclui(consultado, consultado->documento);
            return;
        
        case '3':
            aluguel_lista = consultado->ultimo_aluguel;
            if (aluguel_lista != NULL)
            {
                printf("To aq\n"); delay(1000);
                while (aluguel_lista != NULL)
                {
                    aluguel_imprime(aluguel_lista);
                    aluguel_lista = aluguel_lista->prox_aluguel;
                    delay(1000);
                }
            }
            else
            {
                printf("Nao, to aq\n"); delay(1000);
                alert(-10);
            }
            break;
        
        case '4':
            alert(0);
            return;

        default:
            alert(1);
            break;
        }
    }
}

Cliente *cliente_lista(Cliente *cli)
{
    Cliente *cliente_escolha = NULL, *cliente_aux = NULL;
    char cliente_doc[15], cliente_tel[15];
    char ch_escolha[31];
    int escolha;
    int i, id_cliente;

    if (cli != NULL)
    {
        while (1)
        {
            id_cliente = 0;
            system(clear());

            // ==================================================
            // exibe cabeçalho:
            printf("==========================================================================================\n");
            printf("%-3s\t%-30s\t%-15s\t%-15s\t%-10s\n", "ID", "NOME", "CPF", "TELEFONE", "STATUS");
            printf("==========================================================================================\n");
            
            // ==================================================
            // exibe as informações do cliente:
            for (cliente_aux = cli ; cliente_aux != NULL ; cliente_aux = cliente_aux->prox_cliente)
            {
                printf("%d\t", id_cliente);
                cliente_imprime(cliente_aux);
                id_cliente++;
            }

            printf("\nFoi encontrado %d resultado(s).\n", id_cliente);

            alert_msg();
            printf("\nDigite o ID do cliente para continuar (ou deixe em branco para voltar ao menu): ");

            int i = 0;
            while ((ch_escolha[i] = getchar()) != '\n') i++;
            ch_escolha[i] = '\0';
            if (strlen(ch_escolha) > 0)                 /* verifica se está vazio */
            {
                if (teste_formato(ch_escolha) != 0)     /* verifica se é um número */
                {
                    escolha = atoi(ch_escolha);
                    if (escolha >= 0 && escolha <= id_cliente-1)
                    {
                        cliente_escolha = cli;
                        for (i = 0; i < escolha; i++)
                        {
                            cliente_escolha = cliente_escolha->prox_cliente;
                        }
                        return cliente_escolha;
                    }
                }
                alert(1);   /* valor inválido */
            }
            else
            {
                alert(0);   /* voltando para o menu (sem mensagem de erro) */
                break;
            }
        }
    }
    else
        alert(-6);          /* lista está vazia */

    return NULL;
}

void cliente_edita(Cliente *cli, Cliente *editado)
{
    char nome[32], doc[16], tel[16];    /* dados pessoais cliente */
    int i, op_edit = 0;

    // caso mude o CPF, será preciso renomear o arquivo
    // cria o arquivo de histórico:
    char nome_antigo[51] = "./cliente/historico/cliente";
    char nome_novo[51] = "./cliente/historico/cliente";

    while (op_edit < 3)
    {
        system(clear());

        printf("Deixe em branco para manter o dado salvo:\n");
        printf("==================================================\n");

        switch (op_edit)
        {
        
        // dados pessoais do cliente:

        case 0:     /* nome do cliente */
            printf("Digite o novo nome:\n");
            printf("Antigo: %s\n", editado->nome);
            printf("Novo: ");
            i = 0;
            while ((nome[i] = getchar()) != '\n') i++;
            nome[i] = '\0';

            if (strlen(nome) > 0)
            {
                if (strlen(nome) > 30)  /* verifica se o nome é válido */
                {
                    alert(3);           /* tamanho máximo excedido */
                    break;
                }
                strcpy(editado->nome, strupr(nome));
            }
            break;

        case 1:     /* CPF do cliente*/
            printf("Digite o novo CPF (somente numeros):\n");
            
            // nome antigo arquivo de histórico
            strcat(nome_antigo, editado->documento);
            strcat(nome_antigo, ".txt");

            mascara(editado->documento, doc, "###.###.###-##");
            printf("Antigo: %s\n", doc);
            printf("Novo: ");
            // scanf(" %12[^\n]", doc);
            i = 0;
            while ((doc[i] = getchar()) != '\n') i++;
            doc[i] = '\0';
            if (strlen(doc) > 0)
            {
                if (teste_formato(doc)) /* verifica se o CPF é válido */
                {
                    if (strlen(doc) == 11) 
                    {
                        // nome novo do arquivo:
                        strcat(nome_novo, doc);
                        strcat(nome_novo, ".txt");
                        rename(nome_antigo, nome_novo);
                        // muda o nome do cliente:
                        strcpy(editado->documento, doc);
                    }
                    alert(4);           /* Tamanho de CPF inválido */
                    break;
                }
                else
                {
                    alert(5);           /* Formato de CPF inválido */
                    break;
                }
            }
            break;
        
        case 2:     /* Telefone de contato do cliente */
            printf("Novo telefone (somente numeros):\n");

            mascara(editado->telefone, tel, "(##)#####-####");
            printf("Antigo: %s\n", tel);
            printf("Novo: ");
            i = 0;
            while ((tel[i] = getchar()) != '\n') i++;
            tel[i] = '\0';
            if (strlen(tel) > 0)
            {
                if (teste_formato(tel))     /* verifica se o telefone é válido */
                {
                    if (strlen(tel) != 11)
                    {
                        alert(6);           /* Tamanho de telefone inválido */
                        break;
                    }
                } else {
                    alert(7);               /* Formato de telefone inválido */
                    break;
                }
                strcpy(editado->telefone, tel);
            }
            break;
        
        default:
            break;
        }
        op_edit++;
    }

    cliente_atualiza_historico(0, editado, editado->documento);
    cliente_registra(cli);
    alert(-13);
}

Cliente *cliente_atualiza_aluguel(Cliente *cli, char *data_hoje)
{
    Cliente *cliente_aux;
    Aluguel *aluguel_aux;
    char *data_final;
    for (cliente_aux = cli; cliente_aux != NULL; cliente_aux = cliente_aux->prox_cliente)
    {
        aluguel_aux = cliente_aux->ultimo_aluguel;
        data_final = aluguel_data_fim(aluguel_aux);
        if (compara_data(data_final, data_hoje) == 1)
        {
            cliente_aux->status = 0;
            aluguel_finaliza(aluguel_aux);
            cliente_atualiza_historico(1, cli, cli->documento);
        }
    }
}

int cliente_total(Cliente *cli)
{
    int total_cliente = 0;
    Cliente *cliente_aux;
    for (cliente_aux = cli; cliente_aux != NULL; cliente_aux = cliente_aux->prox_cliente)
    {
        total_cliente++;
    }

    return total_cliente;
}

void cliente_cria_historico(Cliente *cli, char* doc)
{
    // cria o arquivo de histórico:
    char nome_arquivo[51] = "./cliente/historico/cliente";

    strcat(nome_arquivo, doc);
    strcat(nome_arquivo, ".txt");

    FILE *hist = fopen(nome_arquivo, "wt");
    if (hist == NULL)
    {
        alert(-7);  /* Arquivo não encontrado */
        return;
    }

    // ==================================================
    // escreve os dados no arquivo:
    fprintf(hist, "===== DADOS DO CLIENTE =====\n");
    fprintf(hist, "NOME:\t%s\n", cli->nome);
    fprintf(hist, "CPF:\t%s\n", cli->documento);
    fprintf(hist, "TELEFONE:\t%s\n", cli->telefone);
    fprintf(hist, "STATUS:\t%d\n", cli->status);

    fprintf(hist,"%%\n");     /* Indicador de parada, para busca do histórico */
    
    // fprintf(hist, "===== HISTORICO DE ALUGUEL =====\n");

    fclose(hist);
}

void cliente_atualiza_historico(int tag, Cliente *cli, char* doc)
{
    // cria o arquivo de histórico:
    char nome_arquivo[51] = "./cliente/historico/cliente";

    strcat(nome_arquivo, doc);
    strcat(nome_arquivo, ".txt");

    FILE *hist = fopen(nome_arquivo, "r+");
    if (hist == NULL)
    {
        alert(-7);  /* Arquivo não encontrado */
        return;
    }
    rewind(hist);

    // ==================================================
    // atualização dos dados modificado:
    if (tag == 0)       /* atualiza os dados pessoais */
    {
        // escreve dados pessoais no arquivo:
        fprintf(hist, "===== DADOS DO CLIENTE =====\n");
        fprintf(hist, "NOME:\t%s\n", cli->nome);
        fprintf(hist, "CPF:\t%s\n", cli->documento);
        fprintf(hist, "TELEFONE:\t%s\n", cli->telefone);
        fprintf(hist, "STATUS:\t%d\n", cli->status);

        fprintf(hist,"%%\n");     /* Indicador de parada, para busca do histórico */
        
    }        
    else if (tag == 1)  /* atualiza a lista de histórico */
    {
        while (fgetc(hist) != '%');     /* pula a sessão dos dados do cliente */

        // escreve os dados no arquivo, após a sessão dos dados do cliente:
        fprintf(hist,"\n");     /* pula a linha do '%' */

        fprintf(hist, "===== HISTORICO DE ALUGUEL =====\n");
        Aluguel *aluguel_aux = cli->ultimo_aluguel;
        aluguel_atualiza_historico(aluguel_aux, hist);  
    }

    fclose(hist);
}

Cliente *cliente_recupera_historico(Cliente *cli, Carro *carro, char *doc)
{
    char cli_nome[31], cli_doc[12], cli_tel[12];    /* dados do cliente */
    int status;

    char data_ini[11], data_fim[11];                /* dados aluguel */
    int status_aluguel, duracao;

    char modelo[35] , placa[7];                     /* dados carros */
    float preco;

    Carro *carro_aux;

    // abre histórico do cliente:
    char nome_arquivo[51] = "./cliente/historico/cliente";

    strcat(nome_arquivo, doc);
    strcat(nome_arquivo, ".txt");

    FILE *hist = fopen(nome_arquivo, "rt");
    if (hist == NULL)
    {
        alert(-7);  /* Arquivo não encontrado */
        return cli; 
    }
    // printf("\nHistorico ta top!\n");

    // ==================================================
    // leitura do arquivo, iniciando pelos dados do cliente:
    char pula[100];             /* usado para pular partes indesejadas do arquivo */
    fgets(pula, 100, hist);     /* pula o cabeçalho do cliente */
    
    fscanf(hist, "%[^\t]\t%[^\n]\n", pula, cli_nome);
    fscanf(hist, "%[^\t]\t%[^\n]\n", pula, cli_doc);
    fscanf(hist, "%[^\t]\t%[^\n]\n", pula, cli_tel);
    fscanf(hist, "%[^\t]\t%d\n", pula, &status);

    // printf("%s\t%s\t%s\n\n", cli_nome, cli_doc, cli_tel);
    // delay(1000);        /* atraso para verificar resposta */
    cli = cliente_cadastra(0, cli, cli_nome, cli_doc, cli_tel);
    
    // ==================================================
    // atualiza a lista do histórico de aluguel:
    if (!feof(hist))
    {
        // escreve os dados no arquivo, após a sessão dos dados do cliente:
        fgets(pula, 100, hist);     
        fgets(pula, 100, hist);     /* pula cabeçalho do histórico */

        while (!feof(hist))     /* lê todo o histórico */
        {
        
            // ==================================================
            // dados do aluguel:
            fscanf(hist, "%[^\t]\t%d\n", pula, &status_aluguel);
    
            fgets(pula, 100, hist);

            fscanf(hist, "%[^\t]\t%[^\n]\n", pula, data_ini);
            fscanf(hist, "%[^\t]\t%[^\n]\n", pula, data_fim);

            duracao = data_para_num(data_fim) - data_para_num(data_ini);

            // ==================================================
            // dados do carro alugado:
            fscanf(hist, "%[^\t]\t%[^\n]\n", pula, modelo);
            fscanf(hist, "%[^\t]\t%[^\n]\n", pula, placa);
            fscanf(hist, "%[^\t]\t%f\n", pula, &preco);
            carro_aux = carro_busca(carro, placa);
            
            fgets(pula, 100, hist);
            
            // ==================================================
            // adiciona o aluguel na pilha:
            cli->ultimo_aluguel = aluguel_cria(cli->ultimo_aluguel, carro_aux, data_ini, duracao);
        }
    }

    fclose(hist);
    return cli;
}

void cliente_apaga_historico(Cliente *cli)
{
    // busca o arquivo de histórico do cliente:
    char nome_arquivo[51] = "./cliente/historico/cliente";

    strcat(nome_arquivo, cli->documento);
    strcat(nome_arquivo, ".txt");

    FILE *hist = fopen(nome_arquivo, "rt");
    if (hist != NULL)   /* histórico encontrado */
    {
        // printf("deu certo abrir %s\n", nome_arquivo);
        fclose(hist);
        // tentativa de apagar o histórico:
        remove(nome_arquivo);
        
    }
    else alert(-7); /* Arquivo não encontrado */
}

void cliente_registra(Cliente *cli)
{
    FILE *fl = fopen("./cliente/registro.txt", "wt");
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

Cliente *cliente_ordena(Cliente *cli, char *nome)
{
	Cliente *ref = NULL;        /* ponteiro para indicar endereço de referência, inicializado com [NULL] */
	Cliente *cliente_aux = cli;			/* cria um ponteiro auxiliar "cliente_aux", inicializada com a lista "cli" */

    // O critério de parada será o fim da fila ou encontrar 
    // um nome que venha depois, na ordem alfabética:
	while (cliente_aux != NULL && compara(cliente_aux->nome, nome) == -1)		/* verifica "cliente_aux" chegou na posição */
	{
		ref = cliente_aux;		        /* "ref" aponta para o valor atual de "cliente_aux" */
		cliente_aux = cliente_aux->prox_cliente;	/* "cliente_aux" passa a apontar para o próximo valor */
	}
	
	return ref; /* retorna o endereço de referência para o novo cadastro */
}

Cliente *cliente_leia(Cliente *cli, Carro *carro)
{
    FILE *fl = fopen("./cliente/registro.txt", "rt");
    // verifica se o arquivo foi aberto corretamente:
    if (fl == NULL) 
    {
        printf("\nArquivo nao encontrado!\n");
        return 0; // erro ao acessar o arquivo
    }

    // ==================================================
    // move o cursor do arquivo para o fim
    // e verifica se o arquivo está vazio:
    fseek(fl, 0, SEEK_END);
    if (ftell(fl) != 0) {
        // retorna o cursor ao início do arquivo:
        rewind(fl);

        int i, id;
        char nome[41], doc[15], status[15];
        
        // pula a linha do cabeçalho:
        char pula[100];
        fgets(pula, 100, fl);

        // printf("Dados registro:\n");
        while (!feof(fl))
        {
            fscanf(fl, "%[^\t]\t%[^\t]\t%[^\n]\n", nome, doc, status);
            // printf("%s\t%s\n\n", nome, doc);
            // cli = cliente_cadastra(cli, nome, doc, tel);
            cli = cliente_recupera_historico(cli, carro, doc);

        }
    }
    // delay(1000);            /* atraso para verificar resposta */

    fclose(fl);
    return cli;
}