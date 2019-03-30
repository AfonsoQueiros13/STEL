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
	double u = (double)random() / (double) (RAND_MAX); //calculo de u
	return u;
}
/*********************************************RETORNA C***************************************/

double getC(double lambda)
{
	double c;
	c = (-1 / lambda) * log(getRandom());
	return c;
}
/*********************************************RETORNA D********************************************/

double getD(double dm)
{
	double d;
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
	double ic = 0.0,id=0.0, time_simulation = 0.0, c = 0.0, lambda = 200.0, dm = 0.008, d = 0.0, prob = 0.0;
	unsigned int i = 0, cont = 1, neg = 0;
	int numCanais = 0;
	double time=0.0;
	/*********************************************FIM INICIALIZAÇÕES************************************************/
	printf("\nNumber of channels: ");
	scanf("%d", &numCanais);
	int aux = numCanais;
	int prim_vez=0;
	char busy= FALSE;
	printf("\n Simulation Time (ms): ");
	scanf("%lf", &time_simulation);

	/*********************Inicio de  chegada de chamadas a 0.0 segundos********************************************/
	lista_eventos = adicionar(lista_eventos, CHEGADA, 0.0);
	/********************************************CICLO WHILE*********************************************/
	while (time < time_simulation)
	{
		//sleep(1);
		printf("\nNumber of free channels: %d", numCanais);
		/*-------------------GERAR CHEGADA DO PROX PACOTE E PARTIDA DO ANTERIOR----------------*/
		if (lista_eventos->tipo == CHEGADA)
		{ //Ocorreu um evento CHEGADA!
			printf("\nCHEGADA! em %lf (s)", lista_eventos->tempo);
			prim_vez++;
			c = getC(lambda); // funcao que calcula c(chegada de uma nova chamada)	
			d = getD(dm);			//funcao que calcula d(tempo de partida desde da chegada)
			id = ic + d;      //como id depende de ic e queremos a partida do anterior este calculo e feito antes de ic
		    ic += c;
			printf("\nic= %lf", ic);
	    	printf("\nid= %lf", id);
			lista_eventos = adicionar(lista_eventos, CHEGADA, ic);
			if(busy==FALSE){
				if(prim_vez==1)
					lista_eventos = adicionar(lista_eventos, PARTIDA, d); //partida do pacote anterior
				else
					lista_eventos = adicionar(lista_eventos, PARTIDA, id); //partida do pacote anterior
			}
			
			if (numCanais == 0){
				busy=TRUE;
			}
			if(busy==TRUE)
				neg++;
				
			if(numCanais>0)
				numCanais--;

			cont++; //contagem de eventos de chegada
				
		}	
		/*-------------------FIM PROCESSAMENTO CHEGADA----------------*/

		/*------------------- PROCESSAMENTO PARTIDA----------------*/

		if (lista_eventos->tipo == PARTIDA)
		{ //Ocorreu um evento PARTIDA!
			printf("\nPARTIDA! em %lf (s)", lista_eventos->tempo);
			if(numCanais<aux){
			numCanais++;
			busy=FALSE;
			}
			/*-------------------FIM PROCESSAMENTO PARTIDA----------------*/
		}
		time+=ic;
		lista_eventos = (lista *)lista_eventos -> proximo; // next loop verifies next event
		
	}
	printf("\n\nLISTA DE EVENTOS\n\n");
	imprimir(lista_eventos);
	printf("Chamadas negadas: %d\n", neg);
	printf("Contagem de chegadas: %d\n", cont);
	prob = ((double)neg / (double)cont) * 100.0;
	printf("Probabilidade de bloco : %lf % \n", prob);
	return 0;
}
