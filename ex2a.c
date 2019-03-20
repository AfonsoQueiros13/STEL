#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "struct_eventos.c"

#define CHEGADA 0
#define PARTIDA 1
#define FALSE 0
#define TRUE 1

/*********************************************RETORNA VALOR DE 0 a 1********************************/
double getRandom()
{
	double u = (double)random() / (RAND_MAX);
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
	//printf("\nValor de C: %f ", c);
	return d;
}
/*********************************************MAIN************************************************/

int main(void)
{
	/*********************************************INICIALIZAÇÕES************************************************/

	lista *lista_eventos;
	lista_eventos = NULL;
	int tipo_ev;
	double tempo_ev;
	double ic = 0.0, time_simulation = 0.0, c = 0.0, lambda = 200.0, dm = 0.008, delta = 0.001, d = 0.0, prob = 0.0;
	unsigned int i = 0, n = 0, cont = 0, neg = 0;
	int numCanais = 0;
	double time=0.0;
	char busy = FALSE;
	/*********************************************FIM INICIALIZAÇÕES************************************************/
	printf("\nNumber of channels: ");
	scanf("%d", &numCanais);
	int aux = numCanais;
	double id = 0.0;
	printf("\n Simulation Time (ms): ");
	scanf("%lf", &time_simulation);
	/*********************Inicio de  chegada de chamadas a 0.0 segundos********************************************/
	lista_eventos = adicionar(lista_eventos, CHEGADA, 0.0);
	numCanais--; //um canal fica já ocupado com a chegada de uma chamada
	cont = 1;
	while (time < time_simulation)
	{
		//sleep(1);
		printf("\nNumber of free channels: %d", numCanais);
		/*-------------------PROCESSAMENTO CHEGADA----------------*/
		printf("\nlista_eventos->tipo= %d",lista_eventos->tipo);
		if (lista_eventos->tipo == CHEGADA)
		{ //Ocorreu um evento CHEGADA!
			printf("\nCHEGADA! em %lf (s)", lista_eventos->tempo);
			/**********************Adiciona novo evento de chegada e apaga anterior *****************************/
				c = getC(lambda); // funcao que calcula c(chegada de uma nova chamada)
				lista_eventos = (lista *)lista_eventos -> proximo; // next loop verifies next event
				
				d = getD(dm);			//funcao que calcula d(tempo de partida desde da chegada)
				id = ic + d;
				printf("\nic= %lf", ic);
				printf("\nid= %lf", id);
				
				lista_eventos = adicionar(lista_eventos, CHEGADA, ic);
				lista_eventos = adicionar(lista_eventos, PARTIDA,id); //partida do pacote anterior
				ic += c;					//contador
				if(numCanais>0)
				numCanais--;
				cont++; //contagem de eventos de chegada
				
		}
			if (numCanais == 0)
				neg++;
		/*-------------------FIM PROCESSAMENTO CHEGADA----------------*/

		/*------------------- PROCESSAMENTO PARTIDA----------------*/

		if (lista_eventos->tipo == PARTIDA)
		{ //Ocorreu um evento PARTIDA!
			lista_eventos = (lista *)lista_eventos -> proximo; // next loop verifies next event
			printf("\nPARTIDA! em %lf (s)", lista_eventos->tempo);
			if(numCanais<aux)
			numCanais++;
			/*-------------------FIM PROCESSAMENTO PARTIDA----------------*/
		}
		time+=ic;
	}
	printf("\n\nLISTA DE EVENTOS\n\n");
	imprimir(lista_eventos);
	printf("Chamadas negadas: %d\n", neg);
	printf("Contagem: %d\n", cont);
	prob = ((double)neg / (double)cont) * 100.0;
	printf("Percentagem de bloco: %lf\n", prob);
	return 0;
}
