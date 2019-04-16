/******************************INCLUDES DAS BIBLIOTECAS NECESSARIAS****************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "struct_eventos.c"
#include <time.h>
/******************************DEFINICOES DE ALGUMAS VARIAVEIS BOLEANAS************************/
#define CHEGADA 0
#define PARTIDA 1
#define FALSE 0
#define TRUE 1
#define LAMBDA 80.0
#define DM 0.008
#define SPECIFIC_QUEUE 20000
/******************************FUNCOES USADAS NA MAIN*******************************************/
double getGaussRandomNumbers() { //funcao que gera numeros gaussianos random
    double u = ((double) rand() / (RAND_MAX)) * 2 - 1;
    double v = ((double) rand() / (RAND_MAX)) * 2 - 1;
    double r = u * u + v * v;
    if (r == 0 || r > 1) return getGaussRandomNumbers();
    double c = sqrt(-2 * log(r) / r);
    return u * c;
}
double approxRollingAverage (double avg, double new_sample,int cont) { //atualiza a media

    avg -= avg / cont;
    avg += new_sample / cont;

    return avg;
}
double getRandom() //retorna numero de 0 a 1
{
    double u = (double)rand() / (RAND_MAX); //calculo de u
    return u;
}

double getC(double lambda) //retorna c (chegada de uma chamada apartir do tempo atual)
{
    double c = 0;
    c = (-1 / LAMBDA) * log(getRandom());
    return c;
}
double getD(double dm) //retorna d(partida de uma chamada apartir do tempo atual)
{
    double d = 0;
    d = (-DM) * log(getRandom());
    return d;
}

void main(){
    /**************************************INICIALIZAÇÕES****************************************/
    srand(time(NULL)); //gerar semetes diferentes sempre que se corre o programa
    lista *lista_eventos = NULL;
    double time_simulation = 0.0, c = 0.0, d = 0.0, prob = 0.0;
    unsigned int n = 0, cont = 0, queue = 0, est_queue = 0;
    int numCanais = 0;
    double time_init = 0.0;
    double atraso_medio = 0.0;
    double cont_atraso = 0.0;
    clock_t start_delay, end_delay;
    int aux = 0;
    char busy = FALSE;
    double start_delays[50000];
    double end_delays[50000];
    double atraso_pacotes[50000];
    int i = 0, j = 0;
    double delay;
    int contador_delay = 0;
    double estimator_delay;
    int queue_max;
    int packet_loss = 0;
    int neg = 0;
    char character;
    double random;
    int geral_queue;
    /************************INTERACAO COM O UTILIZADOR(INSERCAO DE DADOS)**********************/
    printf("\n\tWelcome to Call Center System Simulator (PRESS ENTER TO CONTINUE)...\n");
    do 
    {
        character = getchar();
    } while (character != '\n' && character != EOF);
    
    printf("\n Simulation Time (s): "); //tempo de simulacao do programa
	scanf("%lf", &time_simulation);
    printf("\nGeral Queue (units): ");
    scanf("%d",&geral_queue);
    
    
    /*****************Inicio de  chegada de chamadas a 0.0 segundos*******************************/
	lista_eventos = adicionar(lista_eventos, CHEGADA, 0.0);
	while (time_init < time_simulation)
	{
		/* carrega proximo evento */
		time_init = lista_eventos->tempo;
		printf("\n\n\nEvento do tipo: %d -- No tempo: %lf", lista_eventos->tipo, lista_eventos->tempo);

		if (lista_eventos->tipo == CHEGADA)
		{
            sleep(1);
            //aqui a chamada será sempre atendida pelo atendimento geral
            //a chamada tem 0.7 de probabilidade de ser especifica e 0.3 de ser geral
            random = getRandom(); //variavel que decide se a chamada e geral ou especifica
            if(random < 0.3)
            {
                //chamada geral
            }
            else
            {
                //chamada especifica
            }
            
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
					d = getD(DM);
					lista_eventos = adicionar(lista_eventos, PARTIDA, time_init + d);
				}
			}

			c = getC(LAMBDA);
			lista_eventos = adicionar(lista_eventos, CHEGADA, time_init + c);
			cont++;
		}

		if (lista_eventos->tipo == PARTIDA)
		{
			// liberta Canal
			if (numCanais < aux)
			{
				numCanais++;
				busy = FALSE;
			}
		}

		if (lista_eventos->tipo != PARTIDA && lista_eventos->tipo != CHEGADA)
			printf("\n\nNão existem pacotes gerados!\n\n");

		lista_eventos = (lista *)lista_eventos->proximo;
	}


}