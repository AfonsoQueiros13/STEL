/******************************INCLUDES DAS BIBLIOTECAS NECESSARIAS****************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "struct_eventos.h" //lista ligada 
#include <time.h>
#include "gnuplot.h"
#include "tools.h"
void main(){
    /**************************************INICIALIZAÇÕES****************************************/
    srand(time(NULL)); //gerar semetes diferentes sempre que se corre o programa
    lista *lista_eventos = NULL;
    double time_simulation, c = 0.0, d = 0.0, prob = 0.0;
    unsigned int n = 0, cont = 0, queue = 0, est_queue = 0;
    int numCanais = 2; // (mudar depois)
    double actual_sym_time = 0.0;
    double delta=1;
    double atraso_medio = 0.0;
    double cont_atraso = 0.0;
    clock_t start_delay, end_delay;
    int aux = 0;
    char busy = FALSE;
    char geral=0;
    double start_delays[50000]={0};
    double end_delays[50000]={0};
    double atraso_pacotes[50000]={0};
    int i = 0, j = 0;
    double delay;
    int contador_delay = 0;
    double estimator_delay;
    int packet_loss = 0;
    int neg = 0;
    char enter;
    double random;
    int geral_queue;
    char percentagem = 37; //apenas o simbolo "%" em ascii code para apresentação na area dos res.    
	double *EixoX= (double*) malloc(100000*sizeof(double));
	int *histogramaY = (int*) malloc(100000*sizeof(int));

    /************************INTERACAO COM O UTILIZADOR(INSERCAO DE DADOS)**********************/
    printf("\n\t%sWELCOME TO CALL CENTER SIMULATOR (PRESS ENTER TO CONTINUE)... \n",KYEL);
    do 
    {
        enter = getchar();
    } while (enter != '\n' && enter != EOF);
	aux = numCanais;
    printf("\n%sSimulation Time (s): ",KMAG); //tempo de simulacao do programa
	scanf("%lf",&time_simulation);
    printf("%s",RESET);
    /*****************Inicio de  chegada de chamadas a 0.0 segundos*******************************/
	lista_eventos = adicionar(lista_eventos, CHEGADA, 0.0); //primeira chamada aos 0 segundos
	while (actual_sym_time < time_simulation)
	{
		/* carrega proximo evento */
		actual_sym_time = lista_eventos->tempo;
		if (lista_eventos->tipo == CHEGADA)
		{
			random = getRandom(); //variavel que vai checkar se a chamada é egral ou especifica
            if(random < 0.3){   //chamada só GERAL!! 
                geral=1;
                if (numCanais == 0 && queue < GERAL_QUEUE)
			    {
				    start_delay = actual_sym_time;
				    start_delays[i] = (double)start_delay;
				    i++;
				    queue++;
				    est_queue++;
			}
                if (queue == GERAL_QUEUE)
                {
                    packet_loss++;
                }

                if (numCanais > 0)
			{
				    numCanais--;
				    d = getDGeral(DMGERAL);
                    if(d+UMMINUTO>DGERALMAX) //se a chamada demora mais do que 5min
                    {
                        lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time+300);
                    }
                    else
                    {
                        lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time+UMMINUTO+d);

                    }    
			}
                c = getC(LAMBDA);
			    lista_eventos = adicionar(lista_eventos, CHEGADA, actual_sym_time + c);
                
            }
            else //chamada ESPECIFICA!!!
            {
                geral=0;
            }
            cont++; 
		}

		if (lista_eventos->tipo == PARTIDA)
		{
            if(geral==1){
			    if (numCanais < aux && queue == 0)
			    {
				    numCanais++; // so quando queue=0!
			    }

			    if (queue > 0)
			    {
				    d = getDGeral(DMGERAL);
				    queue--;
				    if(d+UMMINUTO>DGERALMAX) //se a chamada demora mais do que 5min
				        lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time+300);
                    else
                        lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time+UMMINUTO+d);
				end_delay = actual_sym_time;
				end_delays[j] = (double)end_delay;
				cont_atraso += end_delays[j] - start_delays[j];
				atraso_pacotes[j] = end_delays[j] - start_delays[j];
				j++;
			    }
            }
		}

		if (lista_eventos->tipo != PARTIDA && lista_eventos->tipo != CHEGADA)
			printf("\n\n\t\tNO PACKETS TO ANALYSE! \n\n");
        if(geral==1)
		    lista_eventos = (lista *)lista_eventos->proximo;
	}
   /**********************CONSTRUCAO DO HISTOGRAMA DE DELAYS***************************************/
	for (int i = 0; i < cont; i++) {
		EixoX[i] = i*delta;
        printf("\natraso[%d] -> %lf",i,atraso_pacotes[i]);
	}
	for(int k=0;k<cont; k++)
	{
		for(int j=0;j<cont;j++)
		{
			if(atraso_pacotes[k] >= EixoX[j] && atraso_pacotes[k] < EixoX[j+1])
				histogramaY[j]++;
		}
	}
    printf("\n\n\t\tCALL CENTER STATISTICS\n");
    printf("\nNUMBER OF EMPLOYEES(SERVERS) IN CALL CENTER : %d",aux);
    printf("\nGENERAL-PURPOSE-CALLS QUEUE SIZE : %d",GERAL_QUEUE);
    printf("\nPROBABILITY THAT A CALL IS DELAYED: %lf %c ", ((double)est_queue/(double)cont)*100,percentagem);
	printf("\nPROBABILITY THAT A CALL IS LOSTED: %lf %c ", ((double)packet_loss/(double)cont)*100,percentagem);
	atraso_medio = (cont_atraso / cont);
    printf("\nAVERAGE DELAY OF CALLS THAT SUFFERS DELAY: %lf s\n",atraso_medio);
    plotResultsDelay(EixoX,histogramaY,40);


}
