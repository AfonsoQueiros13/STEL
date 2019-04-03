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
	double time_simulation = 0.0, c = 0.0, lambda = 200.0, dm = 0.008, d = 0.0, prob = 0.0;
	unsigned int i = 0, n = 0, cont = 0, queue = 0, est_queue = 0;
	int numCanais = 0;
	double time_init = 0.0;
	int aux = 0.0;
	char busy = FALSE, flag = FALSE;
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
		time_init = lista_eventos->tempo;
        printf("\n\n\nEvento do tipo: %d -- No tempo: %lf", lista_eventos->tipo, lista_eventos->tempo);

        if (lista_eventos->tipo == CHEGADA) 
		{
<<<<<<< HEAD
			
=======
>>>>>>> ff893b0300cf21c2127f678bc8d031516060dab4
			if (numCanais == 0) 
            {
                queue++;
				est_queue++;
				printf("\nest_queue= %d",est_queue);
				printf("\nqueue = %d",queue);
            } 

			c = getC(lambda);
            lista_eventos = adicionar(lista_eventos, CHEGADA, time_init + c);
            cont++;

            if (numCanais > 0 )
            {
                numCanais--;
				d = getD(dm);
                lista_eventos = adicionar(lista_eventos, PARTIDA, time_init + d);

            }
			
			
		} 
		
		if (lista_eventos->tipo == PARTIDA) 
		{
            // liberta Canal
			if(numCanais < aux) 
			{
				numCanais++;
			}	
				
			if (queue > 0) 
			{
				d = getD(dm);
                lista_eventos = adicionar(lista_eventos, PARTIDA, time_init + d);
	 			queue--;
			}

			
			
		} 

		if (lista_eventos->tipo != PARTIDA && lista_eventos->tipo != CHEGADA)
			printf("\n\nNão existem pacotes gerados!\n\n");

		lista_eventos = (lista*)lista_eventos->proximo;
	}

	printf("\nChamadas na fila: %d", queue);
	printf("\nChamadas que entraram na fila: %d",est_queue);
	printf("\nContagem: %d", cont);
	prob = ((double)est_queue / (double)cont) * 100.0; //primeiro calculo pedido (prob delay)
	printf("\nDelay probability: %lf\n", prob);
	return 0;
}