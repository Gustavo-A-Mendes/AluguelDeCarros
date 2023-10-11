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

Aluguel *aluguel_cria(Aluguel* aluguel, Carro* carro, char *data, int duracao)
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
    novo_aluguel->duracao = duracao;
    novo_aluguel->status_aluguel = 1;
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
    printf("%-10s\t%-30s\t%-15s\t%-15s\t%-10s\t%-10s\n", "STATUS", "MODELO", "PLACA", "PRECO", "DATA ALUGUEL", "DURACAO");
    
    // printf("%-10s\t", aluguel->status_aluguel ? "ATIVO" : "FINALIZADO");
    // carro_imprime(aluguel->carro);
    // printf("\t%-10s\t%-10d\n", aluguel->data_aluguel, aluguel->duracao);
    
    printf("%-10s\t%-30s\t%-15s\tR$%-15.2f\t%-10s\t%-10d\n", aluguel->status_aluguel ? "ATIVO" : "FINALIZADO", aluguel->carro->modelo, aluguel->carro->placa, aluguel->carro->preco, aluguel->data_aluguel, aluguel->duracao);
}

int aluguel_imprime_historico(Aluguel *aluguel)
{   
    char escolha[10];
    int i = 0;
    while (1)
    {
        aluguel_imprime(aluguel);
        printf("\nPressione 'enter' para feixar o histórico.");
        while ((escolha[i] = getchar()) != '\n') i++;
        escolha[i] = '\0';
        if (strlen(escolha) > 0){
            return 1; 
        }else{
            printf(TXT_yellow"\nOpção inválida. Pressione 'enter' para voltar!\n"TXT_reset);    
        }
    }
}

void aluguel_atualiza_historico(Aluguel *aluguel, FILE *fl)
{
    Aluguel *aluguel_aux;
    for (aluguel_aux = aluguel; aluguel_aux != NULL; aluguel_aux=aluguel_aux->prox_aluguel)
    {
        fprintf(fl, "STATUS:\t%d\n", aluguel_aux->status_aluguel);
        fprintf(fl, "PRAZO ALUGUEL:\n");
        fprintf(fl, " -> DE:\t%s\n", aluguel_aux->data_aluguel);
        fprintf(fl, " -> ATE:\t%s\n", prazo(aluguel_aux->data_aluguel, aluguel_aux->duracao));
        fprintf(fl, "MODELO:\t%s\n", aluguel_aux->carro->modelo);
        fprintf(fl, "PLACA:\t%s\n", aluguel_aux->carro->placa);
        fprintf(fl, "PRECO:\t%.2f\n", aluguel_aux->carro->preco);

        fprintf(fl,"==================================================\n");
    }
}

Aluguel *aluguel_ordena(Aluguel *aluguel, char *data_inicio)
{
    Aluguel *ref = NULL;            /* ponteiro para indicar endereço de referência, inicializado com [NULL] */
	Aluguel *aluguel_aux = aluguel;			/* cria um ponteiro auxiliar "aluguel_aux", inicializada com a lista "aluguel" */

    // O critério de parada será o fim da fila ou encontrar 
    // uma data que venha depois:
	while (aluguel_aux != NULL && compara_data(aluguel->data_aluguel, data_inicio) == -1)		/* verifica "P" chegou na posição */
	{
		ref = aluguel_aux;		                    /* "ref" aponta para o valor atual de "aluguel_aux" */
		aluguel_aux = aluguel_aux->prox_aluguel;	/* "aluguel_aux" passa a apontar para o próximo valor */
	}
	
	return ref; /* retorna o endereço de referência para o novo cadastro */
}

char *aluguel_data_fim(Aluguel *aluguel)
{
    int data_inicio = data_para_num(aluguel->data_aluguel);
    char *data_fim = num_para_data(data_inicio + aluguel->duracao);
    return data_fim;
}

void aluguel_finaliza(Aluguel *aluguel)
{   
    aluguel->status_aluguel = 0;
    carro_alugado(aluguel->carro);
}