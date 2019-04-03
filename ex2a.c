#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define CHEGADA 0
#define PARTIDA 1
#define FALSE 0
#define TRUE 1

// Defini��o da estrutura da lista
typedef struct{
	int tipo;
	double tempo;
	struct lista * proximo;
} lista;

// Fun��o que remove o primeiro elemento da lista
lista * remover (lista * apontador)
{
	lista * lap = (lista *)apontador -> proximo;
	free(apontador);
	return lap;
}

lista * charge (lista * apontador, int n_tipo, double n_tempo)
{
	lista * lap = apontador;
	lista * ap_aux, * ap_next;
	if(apontador == NULL)
	{
		apontador = (lista *) malloc(sizeof (lista));
		apontador -> proximo = NULL;
		apontador -> tipo = n_tipo;
		apontador -> tempo = n_tempo;
		return apontador;
	}
    else  {
        lista * lap = (lista *)apontador -> proximo;
	    free(apontador);
	    return lap;
    }
}

// Fun��o que adiciona novo elemento � lista, ordenando a mesma por tempo
lista * adicionar (lista * apontador, int n_tipo, double n_tempo)
{
	lista * lap = apontador;
	lista * ap_aux, * ap_next;
	if(apontador == NULL)
	{
		apontador = (lista *) malloc(sizeof (lista));
		apontador -> proximo = NULL;
		apontador -> tipo = n_tipo;
		apontador -> tempo = n_tempo;
		return apontador;
	}
	else
	{
		if (apontador->tempo > n_tempo) {
	        ap_aux = (lista *) malloc(sizeof (lista));
	        ap_aux -> tipo = n_tipo;
            ap_aux -> tempo = n_tempo;
            ap_aux -> proximo = (struct lista *) apontador;
            return ap_aux;
	    }

		ap_next = (lista *)apontador -> proximo;
		while(apontador != NULL)
		{
			if((ap_next == NULL) || ((ap_next -> tempo) > n_tempo))
				break;
			apontador = (lista *)apontador -> proximo;
			ap_next = (lista *)apontador -> proximo;
		}
		ap_aux = (lista *)apontador -> proximo;
		apontador -> proximo = (struct lista *) malloc(sizeof (lista));
		apontador = (lista *)apontador -> proximo;
		if(ap_aux != NULL)
			apontador -> proximo = (struct lista *)ap_aux;
		else
			apontador -> proximo = NULL;
		apontador -> tipo = n_tipo;
		apontador -> tempo = n_tempo;
		return lap;
	}
}

// Fun��o que imprime no ecra todos os elementos da lista
void imprimir (lista * apontador)
{
	if(apontador == NULL)
		printf("Lista vazia!\n");
	else
	{
		while(apontador != NULL)
		{
            if(apontador->tipo==1){
			printf("Tipo=PARTIDA  Tempo=%lf\n", apontador -> tempo);
			apontador = (lista *)apontador -> proximo;
            }
            else
            {
                printf("Tipo=CHEGADA  Tempo=%lf\n", apontador -> tempo);
			apontador = (lista *)apontador -> proximo;
            }
            
		}
	}
}

/*********************************************RETORNA VALOR DE 0 a 1********************************/
double getRandom()
{
	double u = (double)random() / (RAND_MAX); //calculo de u
	return u;
}
/*********************************************RETORNA C***************************************/

double getC(double lambda)
{
	double c = 0;
	c = (-1 / lambda) * log(getRandom());
	return c;
}
/*********************************************RETORNA D********************************************/

double getD(double dm)
{
	double d = 0;
	d = (-dm) * log(getRandom());
	return d;
}
/*********************************************MAIN************************************************/

int main(void)
{
	/*********************************************INICIALIZAÇÕES************************************************/

<<<<<<< HEAD
	lista *lista_eventos = NULL;
	double time_simulation = 0.0, c = 0.0, lambda = 200.0, dm = 0.008, d = 0.0, prob = 0.0;
=======
<<<<<<< HEAD
	lista *lista_eventos = NULL;
	double time_simulation = 0.0, c = 0.0, lambda = 200.0, dm = 0.008, d = 0.0, prob = 0.0;
=======
	lista *lista_eventos;
	int tipo_ev;
	double tempo_ev;
	double ic = 0.0, time_simulation = 0.0, c = 0.0, lambda = 200.0, dm = 0.008, delta = 0.001, d = 0.0, prob = 0.0;
>>>>>>> eb1ca9497cf68b06f4659e7afe62ec34308e2005
>>>>>>> ff893b0300cf21c2127f678bc8d031516060dab4
	unsigned int i = 0, n = 0, cont = 0, neg = 0;
	int numCanais = 0;
	double time_init = 0.0;
	int aux = 0.0;
<<<<<<< HEAD
=======
<<<<<<< HEAD
=======
	double id = 0.0;
>>>>>>> eb1ca9497cf68b06f4659e7afe62ec34308e2005
>>>>>>> ff893b0300cf21c2127f678bc8d031516060dab4
	char busy = FALSE;
	/*********************************************FIM INICIALIZAÇÕES************************************************/
	printf("\nNumber of channels: ");
	scanf("%d", &numCanais);
	aux = numCanais;
	printf("\n Simulation Time (s): ");
	scanf("%lf", &time_simulation);
	/*********************Inicio de  chegada de chamadas a 0.0 segundos********************************************/
	lista_eventos = adicionar(lista_eventos, CHEGADA, 0.0);

	while (time_init < time_simulation)
	{
		/* carrega proximo evento */
<<<<<<< HEAD
		time_init = lista_eventos->tempo;
=======
<<<<<<< HEAD
		time_init = lista_eventos->tempo;
=======
		lista_eventos = (lista*)lista_eventos->proximo;
		time_init = lista_eventos->tempo;

>>>>>>> eb1ca9497cf68b06f4659e7afe62ec34308e2005
>>>>>>> ff893b0300cf21c2127f678bc8d031516060dab4
        printf("\n\n\nEvento do tipo: %d -- No tempo: %lf", lista_eventos->tipo, lista_eventos->tempo);

        if (lista_eventos->tipo == CHEGADA) 
		{
			if (numCanais == 0) 
            {
                busy = TRUE;
                neg++;
            } 

            if (numCanais > 0)
            {
                numCanais--;

                if (busy == FALSE) 
                {
                    d = getD(dm);
<<<<<<< HEAD
                    lista_eventos = adicionar(lista_eventos, PARTIDA, time_init + d);
=======
<<<<<<< HEAD
                    lista_eventos = adicionar(lista_eventos, PARTIDA, time_init + d);
=======
                    adicionar(lista_eventos, PARTIDA, time_init + d);
>>>>>>> eb1ca9497cf68b06f4659e7afe62ec34308e2005
>>>>>>> ff893b0300cf21c2127f678bc8d031516060dab4
                }  
            }

			c = getC(lambda);
<<<<<<< HEAD
            lista_eventos = adicionar(lista_eventos, CHEGADA, time_init + c);
=======
<<<<<<< HEAD
            lista_eventos = adicionar(lista_eventos, CHEGADA, time_init + c);
            cont++;
			
		} 
		
		if (lista_eventos->tipo == PARTIDA) 
		{
            // liberta Canal
=======
            adicionar(lista_eventos, CHEGADA, time_init + c);
>>>>>>> ff893b0300cf21c2127f678bc8d031516060dab4
            cont++;
			
		} 
		
		if (lista_eventos->tipo == PARTIDA) 
		{
<<<<<<< HEAD
            // liberta Canal
=======
            /* liberta Canal */
>>>>>>> eb1ca9497cf68b06f4659e7afe62ec34308e2005
>>>>>>> ff893b0300cf21c2127f678bc8d031516060dab4
			if(numCanais < aux) 
			{
				numCanais++;
			    busy = FALSE;
			} 
		} 

		if (lista_eventos->tipo != PARTIDA && lista_eventos->tipo != CHEGADA)
			printf("\n\nNão existem pacotes gerados!\n\n");
<<<<<<< HEAD

		lista_eventos = (lista*)lista_eventos->proximo;
=======
<<<<<<< HEAD

		lista_eventos = (lista*)lista_eventos->proximo;
=======
>>>>>>> eb1ca9497cf68b06f4659e7afe62ec34308e2005
>>>>>>> ff893b0300cf21c2127f678bc8d031516060dab4
	}

	printf("\nChamadas negadas: %d\n", neg);
	printf("Contagem: %d\n", cont);
	prob = ((double)neg / (double)cont) * 100.0;
	printf("Percentagem de bloco: %lf\n", prob);
	return 0;
}
