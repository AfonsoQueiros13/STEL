#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "struct_eventos.c"

#define CHEGADA 0
#define PARTIDA 1
#define FALSE 0
#define TRUE 1

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

	lista *lista_eventos = NULL;
	double time_simulation = 0.0, c = 0.0, lambda = 200.0, dm = 0.008, d = 0.0, prob = 0.0, delta = 0.001;
	unsigned int n = 0, cont = 0, queue = 0, est_queue = 0;
	int numCanais = 0, aux = 0;
	double time_init = 0.0;
	double atraso_medio = 0.0;
	double cont_atraso = 0.0;
	double start_delay, end_delay;
	double start_delays[50000];
	double end_delays[50000];
	double atraso_pacotes[50000];
	int i = 0, j = 0;
	double delay;
	int contador_delay = 0;
	double estimator_delay;

	double *EixoX= (double*) malloc(100000*sizeof(double));
	int *histogramaY = (int*) malloc(100000*sizeof(int));

	/*********************************************FIM INICIALIZAÇÕES************************************************/
	printf("\nNumber of channels: ");
	scanf("%d", &numCanais);
	aux = numCanais;
	printf("\n Simulation Time (s): ");
	scanf("%lf", &time_simulation);
	printf("\nIntroduce minimum delay for compare (s) : ");
	scanf("%lf", &delay);
	delay = delay / 1000.0;

	/*********************Inicio de  chegada de chamadas a 0.0 segundos********************************************/
	lista_eventos = adicionar(lista_eventos, CHEGADA, 0.0);

	while (time_init < time_simulation)
	{
		/* carrega proximo evento */
		time_init = lista_eventos->tempo;
		//printf("\n\n\nEvento do tipo: %d -- No tempo: %lf", lista_eventos->tipo, lista_eventos->tempo);

		if (lista_eventos->tipo == CHEGADA)
		{

			if (numCanais == 0)
			{
				start_delay = time_init;
				start_delays[i] = (double)start_delay;

				i++;
				queue++;
				est_queue++;
			}

			if (numCanais > 0)
			{
				numCanais--;

				d = getD(dm);
				lista_eventos = adicionar(lista_eventos, PARTIDA, time_init + d);
			}

			c = getC(lambda);
			lista_eventos = adicionar(lista_eventos, CHEGADA, time_init + c);
			cont++;
		}

		if (lista_eventos->tipo == PARTIDA)
		{
			// liberta Canal
			if (numCanais < aux && queue == 0)
			{
				numCanais++; // so quando queue=0!
			}

			if (queue > 0)
			{
				d = getD(dm);
				queue--;
				lista_eventos = adicionar(lista_eventos, PARTIDA, time_init + d);

				end_delay = time_init;
				end_delays[j] = (double)end_delay;
				cont_atraso += end_delays[j] - start_delays[j];
				atraso_pacotes[j] = end_delays[j] - start_delays[j];

				j++;
			}
		}

		if (lista_eventos->tipo != PARTIDA && lista_eventos->tipo != CHEGADA)
			printf("\n\nNão existem pacotes gerados!\n\n");

		lista_eventos = remover (lista_eventos);
	}

	for (int i = 0; i < est_queue; i++)
	{
		if (atraso_pacotes[i] > delay)
		{
			contador_delay++;
		}
	}

	/*** Para o grafico *****/
	for (int i = 0; i < cont; i++) {
		EixoX[i] = i*delta;
	}

	for(int k=0;k<cont; k++)
	{
		for(int j=0;j<cont;j++)
		{
			if(atraso_pacotes[k] >= EixoX[j] && atraso_pacotes[k] < EixoX[j+1])
				histogramaY[j]++;
		}
	}
	/***** Grafico *****/

	printf("\n\n\t\t***************RESULTADOS*******************\n");
	printf("\nChamadas na fila: %d", queue);
	printf("\nChamadas que entraram na fila: %d", est_queue);
	printf("\nContagem: %d", cont);
	prob = ((double)est_queue / (double)cont) * 100.0; //primeiro calculo pedido (prob delay)
	printf("\nDelay probability: %lf", prob);
	atraso_medio = (cont_atraso / cont);
	printf("\nAtraso medio de todos os pacotes : %lf (s)", atraso_medio);
	estimator_delay = ((double)(contador_delay/(double)cont)) * 100.0;
	printf("\nProbabilidade de ser maior do que %lf (s) é  : %lf \n", delay, estimator_delay);

	imprimir(lista_eventos);

	plotResults(EixoX,histogramaY,time_simulation);

	return 0;
}
