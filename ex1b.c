#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <unistd.h>

#define CHEGADA 0
#define PARTIDA 1

double c = 0.0;
double u = 0.0;
double delta = 1.0/1000;
double lambda = 0.0;
double cmedio= 0.0;

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
  //  printf ("\nValor de u: %f", u);
    return u;
}

double getC ()
{
    c = (-1/lambda) * log(getRandom());

		/* Probabilidade de lambda * delta */
		double probabilidade = lambda * delta;
		double random_num = getRandom();
		//printf("random= %lf",random_num);

		if(probabilidade <= random_num)
    	return c;
		else
			return -1;
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
	/*
	// Inicializações //
	*/
	lista * lista_eventos;
	int tipo_ev, cont = 0, neg = 0; double tempo_ev;
  double time_simulation = 0.0, k = 0.0, p = 0.0;
  unsigned int i = 0, n = 0;

	lista_eventos = NULL;

  printf("\nInsira o lambda: ");
  scanf("%lf", &lambda);

  // Inicio de  chegada de chamadas a 0.0 segundos
	lista_eventos = adicionar(lista_eventos, CHEGADA, 0.0);

  printf("\nInsira o Tempo de simulação: ");
  scanf("%lf", &time_simulation);

	double *EixoX= (double*) malloc(100000*sizeof(double));
	double *EixoY = (double*) malloc(100000*sizeof(double));
	int *histogramaY = (int*) malloc(100000*sizeof(int));

	while (p < time_simulation)
  {
		k = getC();

		if(k != -1)
		{
			p = p + k;
			EixoY[i++] = k;
			/* Valor de C incrementado
			printf("Valor de p: %f\n", p);	*/

			/* Adiciona novo evento e apaga anterior */
			if (lista_eventos != NULL)
			{
				tipo_ev = lista_eventos -> tipo;
				tempo_ev = lista_eventos -> tempo;
				lista_eventos = remover(lista_eventos);
			}
			lista_eventos = charge(lista_eventos, CHEGADA, p);

			/* contador */
		cont++;
		/* delta 1/1000 */
		EixoX[n++] = cont*delta;
		}
		else
		{
			neg++;
		}

  }

	/* Calculo do valor médio */
	cmedio=p/(double)cont;
	printf("O valor medio de chegadas : %lf. \n", cmedio);

	/* Print dos vetores que representam os eixos */
	for( i = 0; i < cont; i++)
	{
		printf("EIXO Y: %lf\n", EixoY[i]);
		printf("EIXO X: %lf\n", EixoX[i]);
	}

  printf("\n");
  printf("\nLISTA ACTUAL\n");
	imprimir(lista_eventos);

	/* Numero de chamadas em j tempo */
	for(i = 0;i < cont; i++)
	{
		for(int j = 0;j < cont; j++)
		{
			if(EixoY[i] >= EixoX[j] && EixoY[i] < EixoX[j+1]) histogramaY[j]++;
		}
	}

	printf("Numero de ocorrencias: %d\n",cont);
	for(int z=0;z<100;z++){
			printf("ocorrencias em  %d : %d \n",z,histogramaY[z]);
	}

	printf("\n Numeros que nao entraram: %d\n", neg);

	printf("cont: %d\n",cont);

	plotResults(EixoX,histogramaY,time_simulation);
	return 0;
}
