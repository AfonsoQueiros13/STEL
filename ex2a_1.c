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
double getRandom() {
    double u = (double) random()/(RAND_MAX);
    return u;
}
/*********************************************RETORNA C***************************************/

double getC (double lambda) {
    double c = 0;
    c = (-1/lambda) * log(getRandom());
    return c;
}
/*********************************************RETORNA D********************************************/

double getD (double dm) {
	double d = 0;
    d = (-dm) * log(getRandom());
    //printf("\nValor de C: %f ", c);
    return d;
}
/*********************************************MAIN************************************************/

int main(void)
{
/*********************************************INICIALIZAÇÕES************************************************/

	lista * lista_eventos;
	lista_eventos = NULL;
	int tipo_ev; double tempo_ev;
  double ic = 0.0, time_simulation = 0.0, c = 0.0, lambda = 200.0, dm = 0.008, delta = 0.001, d = 0.0, prob = 0.0;
  unsigned int i = 0, n = 0, cont = 0, neg = 0;
	int numCanais = 0;
	char busy = FALSE;
	/*********************************************FIM INICIALIZAÇÕES************************************************/
	printf("\nNumber of channels: ");
  scanf("%d", &numCanais);
	int aux= numCanais;
	double id=0;
	printf("\n Simulation Time (ms): ");
  scanf("%lf", &time_simulation);
  /*********************Inicio de  chegada de chamadas a 0.0 segundos********************************************/
	lista_eventos = adicionar(lista_eventos, CHEGADA, 0.0);
	numCanais--; //um canal fica já ocupado com a chegada de uma chamada
	cont=1;
  while (ic < time_simulation)
    {
		printf("numero de canais: %d", numCanais);
				/*-------------------PROCESSAMENTO CHEGADA----------------*/
		lista_eventos = adicionar(lista_eventos, CHEGADA, ic);

		
		if(lista_eventos->tipo==CHEGADA){ //Ocorreu um evento CHEGADA!
			printf("\nCHEGADA!");
			c=getC(lambda); // funcao que calcula c(chegada de uma nova chamada)
			ic+=c; //contador

			/**********************Adiciona novo evento de chegada e apaga anterior *****************************/
			/*if ( lista_eventos != NULL)
		{
				tipo_ev = lista_eventos -> tipo;
				tempo_ev = lista_eventos -> tempo;
				lista_eventos = remover(lista_eventos);
		}*/
			//lista_eventos = adicionar(lista_eventos, CHEGADA,ic); //geração de uma chegada
		 	cont++; //contagem de eventos de chegada 		
			if(numCanais>0){
				numCanais--;
				d = getD(dm); //funcao que calcula d(tempo de partida desde da chegada)
		  	id=ic+d; 
    		lista_eventos = adicionar(lista_eventos, PARTIDA, id);
			}
			else 
				neg++;
		}
				/*-------------------FIM PROCESSAMENTO CHEGADA----------------*/

				/*------------------- PROCESSAMENTO PARTIDA----------------*/

		if (lista_eventos->tipo==PARTIDA) { //Ocorreu um evento PARTIDA!
				printf("\nPARTIDA!");
				numCanais++;
		/*-------------------FIM PROCESSAMENTO PARTIDA----------------*/
			}
		}		
	printf("\n\n");
	imprimir(lista_eventos);
	printf ("Chamadas negadas: %d\n", neg);
	printf ("Contagem: %d\n", cont);
	prob = ((double)neg / (double)cont)*100.0;
	printf ("Percentagem de bloco: %lf\n", prob);
	return 0;
}
