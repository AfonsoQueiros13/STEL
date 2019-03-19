#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

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
			printf("Tipo=%d\tTempo=%lf\n", apontador -> tipo, apontador -> tempo);
			apontador = (lista *)apontador -> proximo;
		}
	}
}

double getRandom() {
    double u = (double) random()/(RAND_MAX);
    // printf ("\nValor de u: %f", u);
    return u;
}

double getC (double lambda) {
    double c = 0;
    c = (-1/lambda) * log(getRandom());
    //printf("\nValor de C: %f ", c);
    return c;
}

double getD (double dm) {
	double d = 0;
    d = (-dm) * log(getRandom());
    //printf("\nValor de C: %f ", c);
    return d;
}

// Pequeno exemplo de utiliza��o
int main(void)
{
	lista * lista_eventos;
	lista_eventos = NULL;
	/*
	// Inicializações //
	*/
	int tipo_ev; double tempo_ev;
  double ic = 0.0, time_simulation = 0.0, c = 0.0, lambda = 200.0, dm = 0.008, delta = 0.001, d = 0.0, prob = 0.0;
  unsigned int i = 0, n = 0, cont = 0, neg = 0;
	int numCanais = 0;
	char busy = FALSE;

	/* Channel and Cont */
	printf("\nInsira o numero de canais: ");
  scanf("%d", &numCanais);
	int aux= numCanais;
	double id=0;
	printf("\nInsira o Tempo de simulação (em ms): ");
  scanf("%lf", &time_simulation);

    // Inicio de  chegada de chamadas a 0.0 segundos
	lista_eventos = adicionar(lista_eventos, CHEGADA, 0.0);
	numCanais--;
	cont=1;

  while (ic < time_simulation)
    {
		printf("numero de canais: %d", numCanais);
		printf ("State : %d\n", busy);
		c=getC(lambda);
		ic+=c; //contador
		lista_eventos = adicionar(lista_eventos, CHEGADA, ic);
		if(numCanais>0)
			{
				cont++;
				numCanais--;
				d = getD(dm);
				id=ic+d;
				printf("\nid= %lf",id);
				if(numCanais!=0){
				lista_eventos = adicionar(lista_eventos, PARTIDA, id);
				numCanais++;
			}
				if(numCanais==0)
			{
				neg++;
			}
		}
	 if ( lista_eventos != NULL)
			{
				tipo_ev = lista_eventos -> tipo;
				tempo_ev = lista_eventos -> tempo;
				lista_eventos = remover(lista_eventos);
			}
}
	 lista_eventos = charge(lista_eventos, CHEGADA, ic);

	imprimir(lista_eventos);
	printf ("Chamadas negadas: %d\n", neg);
	printf ("Contagem: %d\n", cont);
	prob = ((double)neg / (double)cont)*100.0;
	printf ("Percentagem de bloco: %lf\n", prob);
	return 0;
}
