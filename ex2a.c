#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

#define CHEGADA 0
#define PARTIDA 1
#define FALSE 0
#define TRUE 1

double u = 0;

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
    u = (double) random()/(RAND_MAX);
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

void plotResults(double* xData, int* yData, int dataSize) {
    FILE *gnuplotPipe,*tempDataFile;
    char *tempDataFileName;
    double x;
    int y;
    int i;
    tempDataFileName = "data.temp";
    gnuplotPipe = popen("gnuplot -persist ","w");

    if (gnuplotPipe)
    {
		char * commandsForGnuplot[] = {
    	"set title \"Call Arrivals\"",
    	"set xlabel \"Time(s)\"",
    	"set ylabel \"Arrival Rate\"",
    	"plot 'data.temp' with lines"
		};

  	tempDataFile = fopen(tempDataFileName,"w");

	for (i=0; i < 4; i++)
	{
		fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]);
	}

    fflush(gnuplotPipe);

    for (i=0; i < dataSize; i++)
	{
        x = xData[i];
      	y = yData[i];
        fprintf(tempDataFile,"%lf %d\n",x,y);
  	}

	fclose(tempDataFile);
	printf("press enter to continue...");
	getchar();

	fprintf(gnuplotPipe,"exit \n");
	sleep(1);
	remove(tempDataFileName);
  }
  else
  {
    printf("gnuplot not found...");
  }
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

	double *EixoX= (double*) malloc(10000*sizeof(double));
	double *EixoY = (double*) malloc(10000*sizeof(double));
	int *histogramaY = (int*) malloc(100000*sizeof(int));

	/* Channel and Cont */
	printf("\nInsira o numero de canais: ");
    scanf("%d", &numCanais);

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

		if (numCanais == 0){
			busy=TRUE;
		//	numCanais++;
			}
		//if (numCanais > 0) /* Se existir canais */
		//{
			d = getD(dm);
			c = getC(lambda);
			ic += c;

			if (d < c )  //partida
			{
				busy = FALSE;
				/* uma partida
				if ( lista_eventos != NULL)
				{
					tipo_ev = lista_eventos -> tipo;
					tempo_ev = lista_eventos -> tempo;
					lista_eventos = remover(lista_eventos);
				}
				lista_eventos = charge(lista_eventos, PARTIDA, ic + d);*/
				//if(busy==FALSE)
				numCanais++;
				printf ("Partida em d = %lf e numero de canais = %d\n", d, numCanais);
			}
			else
			{
				if (busy == FALSE)  //chegada
				{
					/*if ( lista_eventos != NULL)
					{
						tipo_ev = lista_eventos -> tipo;
						tempo_ev = lista_eventos -> tempo;
						lista_eventos = remover(lista_eventos);
					}
					lista_eventos = charge(lista_eventos, CHEGADA, ic);*/
					printf ("Chegada em ic = %lf e numero de canais = %d\n", ic, numCanais);
					numCanais--;
				}
				if(busy==TRUE){
					neg++;
				}
				cont++;
				/*printf("\n");
				printf("\nChegada\n");
				imprimir(lista_eventos);*/
			}
	//	}
    }

	printf ("Chmadas negadas: %d\n", neg);
	printf ("Contagem: %d\n", cont);
	prob = ((double)neg / (double)cont)*100.0;
	printf ("Percentagem de bloco: %lf\n", prob);
	//printf("Numero de ocorrências: %d\n", cont);

	/* Print dos vetores que representam os eixos
	for( i = 0; i < cont; i++)
	{
		printf("EIXO Y: %lf\n", EixoY[i]);
		printf("EIXO X: %lf\n", EixoX[i]);
	}
	*/

	/* Numero de chamadas em j tempo
	for(int k=0;k<cont; k++)
	{
		for(int j=0;j<cont;j++)
		{
			if(EixoY[k] >= EixoX[j] && EixoY[k] < EixoX[j+1])
				histogramaY[j]++;
		}
	}*/

	/* Cria gráfico */
	//plotResults(EixoX,histogramaY,time_simulation);
	return 0;
}
