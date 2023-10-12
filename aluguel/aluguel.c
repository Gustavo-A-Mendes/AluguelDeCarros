#include <stdio.h>
#include <stdlib.h>
#include "../carro/carro.c"
#include "aluguel.h"
// #include "../cliente/cliente.h"

struct aluguel
{
    char *data_aluguel;
    int duracao;
    int status_aluguel;     /* 1 -> ativo, 0 -> finalizado */
    
    Carro *carro;
    Aluguel *prox_aluguel;
};

Aluguel *aluguel_cria(Aluguel* aluguel, Carro* carro, char *data, int duracao, int status)
{
    // aloca o espaço necessário para o aluguel novo:
    Aluguel *novo_aluguel = (Aluguel*)malloc(sizeof(Aluguel));
    if (novo_aluguel == NULL)
    {
        alert(-12);
        // printf("\nNao foi possivel criar o registro de aluguel\n");
        return NULL;
    }

    novo_aluguel->data_aluguel = (char *)malloc(11 * sizeof(char));

    // ==================================================
    // insere os dados do cliente:
    strcpy(novo_aluguel->data_aluguel, data);
    novo_aluguel->data_aluguel = realoca_string(novo_aluguel->data_aluguel);
    novo_aluguel->duracao = duracao;
    novo_aluguel->status_aluguel = status;
    novo_aluguel->carro = carro;
    carro_alugado(novo_aluguel->carro);
    
    // carro->disponibilidade = 0;
    // printf("%d", carro->disponibilidade);
    // ==================================================
    // encadea o endereço dos alugueis:

    // endereço do elemento imediatamente antes do novo elemento, na ordem cronológica:
    Aluguel *ref = aluguel_ordena(aluguel, data);
    if (ref == NULL)   /* verifica se o novo cadastro ficará na primeira posição da lista */
    {
        novo_aluguel->prox_aluguel = aluguel;
        aluguel = novo_aluguel;
    }
    else
    {
        novo_aluguel->prox_aluguel = ref->prox_aluguel;
        ref->prox_aluguel = novo_aluguel;
    }
    // printf(":)\n"); delay(500);
    return aluguel;
}

void aluguel_libera(Aluguel *aluguel)
{
    Aluguel *aluguel_aux = aluguel;   /* ponteiro inicializado com a lista */
    Aluguel *aux;             /* ponteiro auxiliar */

    // ==================================================
    // laço de repetição, enquanto valor de "P" não for [NULL] (Fim da lista):
    while (aluguel_aux != NULL) 
    {
        aux = aluguel_aux->prox_aluguel;
        free(aluguel_aux->data_aluguel);
        // carro_libera(aluguel_aux->carro);
        free(aluguel_aux);
        aluguel_aux = aux;
    }
}

void aluguel_imprime(Aluguel *aluguel)
{   
    
    printf("%-10s\t", aluguel->status_aluguel ? "ATIVO" : "FINALIZADO");
    carro_imprime(aluguel->carro);
    printf("%-10s -> %-10s\n", aluguel->data_aluguel, aluguel_data_fim(aluguel));
}

void aluguel_imprime_historico(Aluguel *aluguel, int *historico)
{   
    Aluguel *aluguel_aux = aluguel;
    char escolha[100];
    int i;

    while (aluguel_aux != NULL)
    {
        aluguel_imprime(aluguel_aux);
        aluguel_aux = aluguel_aux->prox_aluguel;
    }

    alert_msg();
    printf("\nPressione ENTER para sair o historico.");
    i = 0;
    while ((escolha[i] = getchar()) != '\n') i++;
    escolha[i] = '\0';
    
    if (strlen(escolha) > 0)
    {
        alert(1);   /* Opção inválida */
    }
    else
    {
        *historico = 0;
        alert(0);   /* retorna a pagina anterior, sem mensagem de erro */
    }
}

void aluguel_atualiza_historico(Aluguel *aluguel, FILE *fl)
{
    Aluguel *aluguel_aux;
    for (aluguel_aux = aluguel; aluguel_aux != NULL; aluguel_aux=aluguel_aux->prox_aluguel)
    {
        fprintf(fl, "\nSTATUS:\t%d\n", aluguel_aux->status_aluguel);
        fprintf(fl, "PRAZO ALUGUEL:\n");
        fprintf(fl, "-> DE:\t%s\n", aluguel_aux->data_aluguel);
        fprintf(fl, "-> ATE:\t%s\n", prazo(aluguel_aux->data_aluguel, aluguel_aux->duracao-1));
        fprintf(fl, "MODELO:\t%s\n", aluguel_aux->carro->modelo);
        fprintf(fl, "PLACA:\t%s\n", aluguel_aux->carro->placa);
        fprintf(fl, "PRECO:\t%.2f\n", aluguel_aux->carro->preco);

        fprintf(fl,"==================================================");
    }
}

Aluguel *aluguel_ordena(Aluguel *aluguel, char *data_inicio)
{
    Aluguel *ref = NULL;            /* ponteiro para indicar endereço de referência, inicializado com [NULL] */
	Aluguel *aluguel_aux = aluguel;			/* cria um ponteiro auxiliar "aluguel_aux", inicializada com a lista "aluguel" */

    // O critério de parada será o fim da fila ou encontrar 
    // uma data que venha depois:
	while (aluguel_aux != NULL && compara_data(aluguel->data_aluguel, data_inicio) < 0)		/* verifica "P" chegou na posição */
	{
		ref = aluguel_aux;		                    /* "ref" aponta para o valor atual de "aluguel_aux" */
		aluguel_aux = aluguel_aux->prox_aluguel;	/* "aluguel_aux" passa a apontar para o próximo valor */
	}
	
	return ref; /* retorna o endereço de referência para o novo cadastro */
}

char *aluguel_data_fim(Aluguel *aluguel)
{
    int data_inicio = data_para_num(aluguel->data_aluguel);
    char *data_fim = num_para_data(data_inicio + aluguel->duracao - 1);
    return data_fim;
}

void aluguel_finaliza(Aluguel *aluguel)
{   
    aluguel->status_aluguel = 0;
    carro_alugado(aluguel->carro);
}