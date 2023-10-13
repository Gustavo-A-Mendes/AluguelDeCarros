#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "../cliente/cliente.h"
#include "../geral/geral.h"
#include "carro.h"

#define TXT_red "\x1b[31m"
#define TXT_green "\x1b[32m"
#define TXT_yellow "\x1b[33m"
#define TXT_reset "\x1b[0m"

int alert_cod = 0;

struct carro
{
    char *placa; 
    char *modelo;
    // 0 -> indisponível; 1 -> disponível
    int disponibilidade; 
    float preco;
    // Cliente *cliente;
    Carro *ant_carro;
    Carro *prox_carro;
};

Carro *carro_cadastra(Carro *carro, char *modelo, char *placa, float preco)
{
    Carro *novo = (Carro*)malloc(sizeof(Carro));

    novo->placa = (char*)malloc(41*sizeof(char)); 
    novo->modelo = (char*)malloc(15*sizeof(char)); 
    // ==================================================
    // encadea o endereço dos clientes:
    
    // ==================================================
    // insere os dados do cliente:
    strcpy(novo->modelo, strupr(modelo));
    novo->modelo = realoca_string(novo->modelo);
    strcpy(novo->placa, placa);
    novo->placa = realoca_string(novo->placa);
    novo->disponibilidade = 1;
    novo->preco = preco;

    // ==================================================
    // encadea o endereço dos clientes:

    // endereço do elemento imediatamente antes do novo elemento, na ordem alfabética:
    Carro *ref = carro_ordena(carro, modelo);
    if (ref == NULL)   /* verifica se o novo cadastro ficará na primeira posição da lista */
    {
        novo->prox_carro = carro;
        novo->ant_carro = NULL;

        if (carro != NULL)
            carro->ant_carro = novo;

        carro = novo;
    }
    else
    {
        novo->prox_carro = ref->prox_carro;
        novo->ant_carro = ref;
    
        if (ref->prox_carro != NULL)    /* verifica se o novo cadastro é o último da lista*/
            ref->prox_carro->ant_carro = novo;
        
        ref->prox_carro = novo;
    }
    return carro;
}

void carro_libera(Carro *carro)
{
    Carro *carro_aux = carro;   /* ponteiro inicializado com a lista */
    Carro *t;           /* ponteiro auxiliar */

    // ==================================================
    // laço de repetição, enquanto valor de "P" não for [NULL] (Fim da lista):
    while (carro_aux != NULL) 
    {
        t = carro_aux->prox_carro;
        free(carro_aux->placa);
        free(carro_aux->modelo);
        free(carro_aux);
        carro_aux = t;
    }
}

char *carro_modelo(Carro *carro)
{
    return carro->modelo;
}

int carro_disponibilidade(Carro *carro)
{
    return carro->disponibilidade;
}

void carro_imprime(Carro *carro)
{
    printf("%-15s\t%-10s\tR$%-10.2f\t", carro->modelo, carro->placa, carro->preco);
}

// void carro_imprime_lista(Carro *carro, int *qtd_carro){
//     int id_carro = 0;
// 
//     // ==================================================
//     // exibe cabeçalho:
//     cabecalho("LISTA DE CARROS\t", "\t\t");
//  
//     printf("==========================================================================================\n");
//     printf("%-3s\t%-15s\t%-10s\t%-10s\t%-10s\n", "ID", "MODELO", "PLACA", "PRECO", "STATUS");
//     printf("==========================================================================================\n");
//    
//     // ==================================================
//     // exibe as informações do carro:
//     Carro *carro_aux;
//     for (carro_aux = carro ; carro_aux != NULL ; carro_aux=carro_aux->prox_carro)
//     {
//         printf("%d\t", id_carro);
//         carro_imprime(carro_aux);
//         printf("%-10s\n", carro_aux->disponibilidade ? "Disponivel" : "Indisponivel");
//         id_carro++;
//     }
//     *qtd_carro = id_carro;
// }

Carro *carro_lista(Carro *carro)
{
    Carro *carro_escolha = NULL, *carro_aux = NULL;
    char ch_escolha[100];
    int i, id_carro, escolha;
    
    if (carro != NULL)
    {
        while(1)
        {
            id_carro = 0;
    
            // ==================================================
            // exibe cabeçalho:            
            printf("==========================================================================================\n");
            printf("%-3s\t%-15s\t%-10s\t%-10s\t%-10s\n", "ID", "MODELO", "PLACA", "PRECO", "STATUS");
            printf("==========================================================================================\n");
            
            // ==================================================
            // exibe as informações do carro:
            for (carro_aux = carro ; carro_aux != NULL ; carro_aux=carro_aux->prox_carro)
            {   
                printf("%d\t", id_carro);
                carro_imprime(carro_aux);
                printf("%-10s\n", carro_aux->disponibilidade ? "Disponivel" : "Indisponivel");
                id_carro++;
            }
            
            printf("\nFoi encontrado %d resultado(s).\n", id_carro);

            alert_msg();
            printf("\nDigite o ID do cliente para continuar (ou deixe em branco para voltar ao menu): ");

            i = 0;
            while ((ch_escolha[i] = getchar()) != '\n') i++;
            ch_escolha[i] = '\0';

            if (strlen(ch_escolha) > 0)                 /* verifica se está vazio */
            {
                if (teste_formato(ch_escolha) != 0)     /* verifica se é um número */
                {
                    escolha = atoi(ch_escolha);
                    if (escolha >= 0 && escolha <= id_carro-1)
                    {
                        carro_escolha = carro;
                        for (i = 0; i < escolha; i++)
                        {
                            carro_escolha = carro_escolha->prox_carro;
                        }
                        return carro_escolha;
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
        alert(-5);          /* lista está vazia */

    return NULL;
}

// void carro_disponivel(Carro *carro)
// {
//     Carro *auxiliar;
//     printf("CARROS DISPONIVEIS:\n");
//     printf("%s\t%s\t%s", "MODELO", "PLACA", "PRECO");
//     for(auxiliar = carro; auxiliar != NULL; auxiliar=auxiliar->prox_carro){
//         if(auxiliar->disponibilidade != 0){
//             printf("%s\t%s\tR$%.2f\n", auxiliar->modelo, auxiliar->placa, auxiliar->preco);
//         }   
//     }
// }

void carro_alugado(Carro *carro)
{
    if (carro->disponibilidade == 1)
        carro->disponibilidade = 0;
    else    
        carro->disponibilidade = 1;
}

Carro *carro_busca(Carro *carro, char *dado_busca, int tipo)
{
    Carro *carro_aux;

    if (tipo == 0)      /* procura o carro pelo modelo */
    {
        for (carro_aux = carro; carro_aux != NULL; carro_aux = carro_aux->prox_carro)   
        {
            if (compara(carro_aux->modelo, strupr(dado_busca)) == 0) {
                // printf("achou, %s\n", C->nome);
                return carro_aux;
            }
        }
        return NULL;
    }
    else                /* procura o carro pela placa */
    {
        for (carro_aux = carro; carro_aux != NULL; carro_aux = carro_aux->prox_carro)
        {
            if (compara(carro_aux->placa, dado_busca) == 0) {
                // printf("achou, %s\n", C->documento);
                return carro_aux;
            }
        }
        return NULL;
    }
}

Carro *carro_ordena(Carro *carro, char *modelo)
{
    Carro *ref = NULL;          /* ponteiro para indicar endereço de referência, inicializado com [NULL] */
	Carro *carro_aux = carro;			/* cria um ponteiro auxiliar "P", inicializada com a lista "cli" */
    // O critério de parada será o fim da fila ou encontrar 
    // um nome que venha depois, na ordem alfabética:
	while (carro_aux != NULL && compara(carro_aux->modelo, modelo) < 0)		/* verifica "carro_aux" chegou na posição */
	{
		ref = carro_aux;		        /* "ref" aponta para o valor atual de "P" */
		carro_aux = carro_aux->prox_carro;	    /* "carro_aux" passa a apontar para o próximo valor */
	}
    // printf("oi\n");
	
	return ref; /* retorna o endereço de referência para o novo cadastro */
}

Carro *carro_leia(Carro *carro)
{
    FILE *fl = fopen("./carro/galeria.txt", "rt");
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

        int i = 0;
        char modelo[41], placa[15];
        float preco;
        
        // pula a linha do cabeçalho:
        char pula[100];
        fgets(pula, 100, fl);
        // printf("Dados registro:\n");
        while (!feof(fl))
        {
            fscanf(fl, "%[^\t]\t%[^\t]\t%f\t%[^\n]\n", modelo, placa, &preco, pula);
            
            carro = carro_cadastra(carro, modelo, placa, preco);
        }
    }
    // delay(1000);            /* atraso para verificar resposta */

    fclose(fl);
    return carro;
}

void carro_edita(Carro *carro, Carro *carro_consultado)
{
    int i;
    char novo_preco[10];

    system(clear());

    printf("Deixe em branco para manter o dado salvo:\n");
    printf("==================================================\n");

    printf("Digite o novo nome:\n");
    printf("Antigo: R$%.2f\n", carro_consultado->preco);
    printf("Novo: ");

    i = 0;
    while ((novo_preco[i] = getchar()) != '\n') i++;
    novo_preco[i] = '\0';
    if (strlen(novo_preco) > 0)
    {
        carro_consultado->preco = atof(novo_preco);
    }
    carro_atualiza_galeria(carro);
    alert(-13);
}

void carro_exclui(Carro *carro, Carro *carro_consultado)
{
    int op;
    Carro *carro_aux;

    while(1)
    {
        system(clear());

        alert_msg();
        printf("\nO cadastro sera apagado. Deseja Continuar [S/N]?\n");
        op = teste_input();

        if (op == 'S')
        {
            //for (carro_aux = carro; carro_aux != NULL; carro_aux = carro_aux->prox_carro)
            //{
             
                if (carro_consultado == carro)
                    carro = carro_consultado->prox_carro;
                else
                    carro_consultado->ant_carro->prox_carro = carro_consultado->prox_carro;
                if(carro_consultado->prox_carro != NULL)
                    carro_consultado->prox_carro->ant_carro = carro_consultado->ant_carro;  
                
                carro_atualiza_galeria(carro);
                free(carro_consultado->modelo);
                free(carro_consultado->placa);
                free(carro_consultado);
                alert(-14);

                break;;
            //} 
        }
        else if (op == 'N')
            break;
        else
            alert(1);
    }
}

int carro_consulta(Carro *carro, Carro *carro_consultado)
{
    int op_cons;

    while(1)
    {
        system(clear());

        printf("==========================================================================================\n");
        printf("DADOS DO CARRO:\n");
        printf("==========================================================================================\n");
        printf("%-30s\t%-10s\t%-10s\t%-10s\n", "MODELO", "PLACA", "PRECO", "STATUS");
        printf("%-30s\t%-10s\tR$%-10.2f\t%-10s\n", carro_consultado->modelo, carro_consultado->placa, carro_consultado->preco, carro_consultado->disponibilidade ? "Disponivel" : "Indisponivel");
        printf("\n==========================================================================================\n");
        

        printf("\n>>>[1] Editar\n");
        printf(">>>[2] Excluir\n");
        printf(">>>[3] Voltar a Lista\n");
        printf(">>>[4] Voltar ao Menu\n");

        alert_msg();
        printf("\nEscolha uma opcao: ");
        op_cons = teste_input();

        switch (op_cons)
        {
            case '1':
                carro_edita(carro, carro_consultado);
                break;

            case '2':
                carro_exclui(carro, carro_consultado);
                return 0;

            case '3':
                alert(0);
                return 1;

            case '4':
                alert(0);
                return 0;
            
            default:
                alert(1);
                break;   
        }
    }
}

void carro_atualiza_galeria(Carro *carro)
{
    char nome_arquivo[60] = "./carro/galeria.txt";
    Carro *carro_aux;

    FILE *galeria = fopen(nome_arquivo, "wt");
    if (galeria == NULL)
    {
        alert(-7);
        return;
    }

    fprintf(galeria, "%s\t%s\t%s\t%s\n", "MODELO", "PLACA", "PRECO", "STATUS");

    for(carro_aux = carro; carro_aux != NULL; carro_aux = carro_aux->prox_carro){
        fprintf(galeria, "%s\t%s\t%.2f\t%d\n", carro_aux->modelo, carro_aux->placa, carro_aux->preco, carro_aux->disponibilidade);
    }

    fclose(galeria);
}
