/******************************INCLUDES DAS BIBLIOTECAS NECESSARIAS****************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "struct_eventos.h" //lista ligada 
#include <time.h>
#include "gnuplot.h"
#include "tools.h"

void main()
{
    /**************************************INICIALIZAÇÕES****************************************/
    srand(time(NULL)); //gerar semetes diferentes sempre que se corre o programa
    lista *lista_eventos = NULL;
    int numCanaisGP = OPERADORESGP; // (mudar depois)
    int numCanaisAS = OPERADORESAS; // (mudar depois)

    double time_simulation = 0, c = 0.0, d = 0.0, prob = 0.0, time_transfer = 0.0;
    double duracao_chamada[100000]={0};
    double inicio_chamada_GP[10000]={0};
    double inicio_chamada_GPAS[10000]={0};
    double time_save_GP_AS[10000]={0};
    double time_save_AS[10000]={0};
    double time_save_GP[10000]={0};
    double start_delays_GP[10000]={0};
    double start_delays_GPAS[10000]={0};
    double atraso_pacotes[10000]={0};
    double atraso_pacotes_AS[10000]={0};
    double avg_waiting_time=0;
    double departure_time = 0.0;
    double actual_sym_time = 0.0;
    double delta = 1.0;
    double atraso_medio = 0.0;
    double atraso_medio_AS = 0.0;
    double cont_atraso = 0.0;
    double cont_atraso_AS = 0.0;
    unsigned int cont = 0, contAS = 0, auxGP = 0, auxAS = 0, queueGP = 0, queueAS = 0, est_queueGP = 0, est_queueAS = 0, packet_loss = 0;
    unsigned int i = 0, j = 0, n = 0, k = 0, b = 0, e = 0, v = 0, h = 0, z = 0, cont_delay_GP = 0, cont_delay_GPAS = 0;
    double estimator_delay;
    char enter;
    double random;
    char percentagem = 37; //apenas o simbolo "%" em ascii code para apresentação na area dos res.    
	double *EixoX= (double*) malloc(10000*sizeof(double));
	int *histogramaY = (int*) malloc(10000*sizeof(int));
	int *histogramaY1 = (int*) malloc(10000*sizeof(int));
    double erro_absoluto = 0.0;
    double erro_relativo = 0.0;


    /************************INTERACAO COM O UTILIZADOR(INSERCAO DE DADOS)**********************/
    printf("\n\t%sWELCOME TO CALL CENTER SIMULATOR (PRESS ENTER TO CONTINUE)... \n",KYEL);
    do 
    {
        enter = getchar();
    } while (enter != '\n' && enter != EOF);
    
    printf("%s",RESET);

	auxGP = numCanaisGP;
    auxAS = numCanaisAS;
    time_simulation = TIME;

    /*****************Inicio de  chegada de chamadas a 0.0 segundos*******************************/
	lista_eventos = adicionar(lista_eventos, CHEGADA, 0.0); //primeira chamada aos 0 segundos

	while (actual_sym_time < time_simulation)
	{
		/* carrega proximo evento */
		actual_sym_time = lista_eventos->tempo;
        //sleep(1);

        printf("\n\nEC tipo %d t: %lf | NCanaisGP: %d | NCanaisAS: %d | qGP: %d | qAS: %d | est_qQP: %d | est_qAS: %d | packet_loss: %d | cont: %d", lista_eventos->tipo, lista_eventos->tempo, numCanaisGP, numCanaisAS, queueGP, queueAS,est_queueGP,est_queueAS, packet_loss, cont);

		if (lista_eventos->tipo == CHEGADA)
		{
			random = getRandom(); //variavel que vai checkar se a chamada é geral ou especifica

            if(random < 0.3)
            {   //chamada só GERAL!! 

                if (queueGP == GERAL_QUEUE)
                {
                    packet_loss++;
                    printf("\nPacket loss! (0.3)");
                }

                if (numCanaisGP == 0 && queueGP < GERAL_QUEUE)
			    {
				    queueGP++;
				    est_queueGP++;
                    printf("\nStart delay at %lf | cont_delay_GP: %d", actual_sym_time, cont_delay_GP);
                    start_delays_GP[cont_delay_GP] = actual_sym_time;
                    cont_delay_GP++;
			    }

                if (numCanaisGP > 0)
			    {
                    inicio_chamada_GP[z] = actual_sym_time;
                    z++;

				    numCanaisGP--;
				    d = getDGeral(DMGERAL);

                    if(d + DGERALMIN > DGERALMAX) //se a chamada demora mais do que 5min
                    {
                        lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time + DGERALMAX);
                        printf("\n GeneralP - Partida em: %lf  ", actual_sym_time + DGERALMAX);
                        time_save_GP[v] = actual_sym_time + DGERALMAX;
                    }
                    else
                    {
                        lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time + DGERALMIN + d);
                        printf("\n GeneralP - Partida em: %lf  ", actual_sym_time + DGERALMIN + d);
                        time_save_GP[v] = actual_sym_time + DGERALMIN + d;
                    }

                    v++;
			    }

                c = getC(LAMBDA);
			    lista_eventos = adicionar(lista_eventos, CHEGADA, actual_sym_time + c);  
                printf ("\n Proxima chegada em: %lf", actual_sym_time + c);
            }
            else //chamada ESPECIFICA!!!
            {
                if (queueGP == GERAL_QUEUE)
                {
                    packet_loss++;
                    printf("\nPacket loss!.. (0.7)");
                }

                if (numCanaisGP == 0 && queueGP < GERAL_QUEUE) 
                {
                    queueGP++;
				    est_queueGP++;
                    printf("\nStart delay at %lf | cont_delay_GPAS: %d", actual_sym_time, cont_delay_GPAS);
                    start_delays_GPAS[cont_delay_GPAS] = actual_sym_time;
                    cont_delay_GPAS++;
                }

                if (numCanaisGP > 0)
			    {

				    numCanaisGP--;
                    //Tempo de espera Distribuição Normal
                    time_transfer = rand_normal(60.0, 20.0);

                    if (time_transfer > 120.0)
                        time_transfer = 120.0;
                    if (time_transfer < 30.0)
                        time_transfer = 30.0;

                    lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time + time_transfer);

                    //Guardar Partida em que será transferida a chamada
                    //Sempre que entrar aqui é guardada a proxima partida no vetor
                    inicio_chamada_GPAS[n] = actual_sym_time;
                    time_save_GP_AS[n] = actual_sym_time + time_transfer;
                    n++;

                    printf("\n GP -> AS Partida em: %lf", actual_sym_time + time_transfer);

			    }

                c = getC(LAMBDA);
			    lista_eventos = adicionar(lista_eventos, CHEGADA, actual_sym_time + c);  
                printf ("\n Proxima chegada em: %lf ", actual_sym_time + c);
            }

            //Contador de toda a recepção de chamadas
            cont++; 
		}
		if (lista_eventos->tipo == PARTIDA)
		{

                /////// GP ////////
                // Aqui entra chamada transferida (70%)
                for (int i = 0; i < n; i++) 
                {
                    if (lista_eventos->tempo == time_save_GP_AS[i])
                    {

                        if (numCanaisGP < auxGP && queueGP == 0)
                        {
                            duracao_chamada[e] = actual_sym_time - inicio_chamada_GPAS[i];
                            e++;
                            
                            printf("\n Case Partida 2. | duracao chamada: %lf", duracao_chamada[e-1]);

                            numCanaisGP++;
                        }

                        if (queueGP > 0)
                        {

                            queueGP--;

                            time_transfer = rand_normal(60.0, 20.0);

                            if (time_transfer > 120.0)
                                time_transfer = 120.0;
                            if (time_transfer < 30.0)
                                time_transfer = 30.0;

                            lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time + time_transfer);

                            time_save_GP_AS[n] = actual_sym_time + time_transfer;
                            n++;

                            atraso_pacotes[j] = actual_sym_time - start_delays_GPAS[cont_delay_GPAS-1];
                            cont_atraso += atraso_pacotes[j];

                            printf("\n Case 6 - +GP -> AS+ Partida em: %lf | Atraso: %lf | Começo em: %lf ", actual_sym_time + time_transfer, atraso_pacotes[j], start_delays_GPAS[cont_delay_GPAS-1]);
                            j++;
                        }

                        if (numCanaisAS ==  0)
                        {
                            queueAS++;
                            est_queueAS++;
                            printf("\nFila AS cheia!");
                        }

                        //Verifica se há operadores disponiveis em AS
                        if (numCanaisAS > 0) 
                        {   
                            // Remete a chamada a um operador disponivel na area especifica
                            numCanaisAS--;

                            d = getDSpec(DMSPEC);  
                            lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time + 60 + d);

                            time_save_AS[b] = actual_sym_time + 60 + d;
                            b++;

                            printf("\n AS, Partida em: %lf, time_sava_AS: %lf", actual_sym_time + 60 + d, time_save_AS[b-1]);

                            //ATRASO PACOTE AS
                            atraso_pacotes_AS[h] = actual_sym_time - start_delays_GPAS[cont_delay_GPAS-1];
                            cont_atraso_AS += atraso_pacotes_AS[h];
                            printf("\nAtraso AS: %lf | Começo em: %lf", atraso_pacotes_AS[h], start_delays_GPAS[cont_delay_GPAS-1]);
                            h++;
                        }

                        printf("\nEntrou em Gp->AS");
                        
                    }
                }

                // GENERALP 30%! //
                for (int i = 0; i < v; i++) 
                {
                    if (lista_eventos->tempo == time_save_GP[i])
                    {

                        if (numCanaisGP < auxGP && queueGP == 0) 
                        {
                            duracao_chamada[e] = actual_sym_time - inicio_chamada_GP[i];
                            e++;

                            numCanaisGP++;
                            printf("\nCase Partida 5 (0.3 p) | duracao chamada: %lf | I: %d | Inicio: %lf", duracao_chamada[e-1], i, inicio_chamada_GP[i]);
                        }

                        if (queueGP > 0)
                        {
                                d = getDGeral(DMGERAL);
                                queueGP--;

                                if((d + DGERALMIN) > DGERALMAX) 
                                {                                   //se a chamada demora mais do que 5min 
                                    lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time + DGERALMAX);
                                    time_save_GP[v] = actual_sym_time + DGERALMAX;
                                    printf("\n GeneralP - Partida em: %lf  ", actual_sym_time + DGERALMAX);
                                }
                                else 
                                {
                                    lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time + DGERALMIN + d);
                                    time_save_GP[v] = actual_sym_time + DGERALMIN + d;
                                    printf("\n GeneralP - Partida em: %lf  ", actual_sym_time + DGERALMIN + d);
                                }

                                atraso_pacotes[j] = actual_sym_time - start_delays_GP[cont_delay_GP-1];
                                printf("\n Case partida 4, D em partida: %lf  | Atraso: %lf | Começo em: %lf", d, atraso_pacotes[j], start_delays_GP[cont_delay_GP-1]);
                                cont_atraso += atraso_pacotes[j];
                                j++;
                        }
                        printf("\nEntrou em GP");
                    }
                }
                
                /////// AS ////////
                for (int i = 0; i < b; i++) 
                {
                    if (lista_eventos->tempo == time_save_AS[i]) 
                    {

                        if (numCanaisAS < auxAS && queueAS == 0) 
                        {
                            //Partida de AS!
                            numCanaisAS++;
                        }

                        if (queueAS > 0) 
                        {
                            queueAS--;

                            d = getDSpec(DMSPEC);  
                            lista_eventos = adicionar(lista_eventos, PARTIDA, actual_sym_time + 60 + d);

                            time_save_AS[b] = actual_sym_time + 60 + d;
                            b++;
                        }

                        printf("\n Case Partida 1");
                    }
                }  
		}

		if (lista_eventos->tipo != PARTIDA && lista_eventos->tipo != CHEGADA)
			printf("\n\n\t\tNO PACKETS TO ANALYSE! \n\n");

	    lista_eventos = remover(lista_eventos);
	}
   /**********************CONSTRUCAO DO HISTOGRAMA DE DELAYS***************************************/
    atraso_medio = ((double)cont_atraso / (double)est_queueGP);
    //histograma do erro medio absoluto
    for (int i = 0; i < cont_atraso; i++) {
		erro_absoluto += abs(atraso_pacotes[i] - atraso_medio);

	}  
    erro_absoluto = erro_absoluto/cont_atraso;
    printf("\nerro abs= %lf",erro_absoluto);
    erro_relativo = 100- (erro_absoluto/atraso_medio)*100;
    	for (int i = 0; i < cont; i++) {
		EixoX[i] = i * delta;    
	}

	for(int k=0;k < cont_atraso; k++)
	{
		for(int j=0;j<cont_atraso;j++)
		{
			if(atraso_pacotes[k] >= EixoX[j] && atraso_pacotes[k] < EixoX[j+1])
				histogramaY[j]++;
		}
	}

    

    /****** Running Average ******/
    for (int i = 0; i < cont; i++) {
       // printf("\nwaiting time of call %d -> %lf", i, duracao_chamada[i]);
		avg_waiting_time = approxRollingAverage(avg_waiting_time, duracao_chamada[i], cont);
        histogramaY1[i] = avg_waiting_time ;
        printf("\nAVG WT = %lf",avg_waiting_time);
	}
    printf("\n\n\t\t%sCALL CENTER STATISTICS\n", KBLU);
    printf("\n%sNUMBER OF EMPLOYEES(SERVERS) IN CALL CENTER : %d",RESET,auxGP);
    printf("\nGENERAL-PURPOSE-CALLS QUEUE SIZE : %d",GERAL_QUEUE);
    printf("\nPROBABILITY THAT A CALL IS DELAYED: %lf %c ", ((double)est_queueGP/(double)cont)*100,percentagem);
	printf("\nPROBABILITY THAT A CALL IS LOSTED: %lf %c ", ((double)packet_loss/(double)cont)*100,percentagem);
    printf("\nAVERAGE DELAY OF CALLS THAT SUFFERS DELAY: %lf s",atraso_medio);
    printf("\nAVERAGE WAITING TIME IN QUEUE: %lf s",avg_waiting_time);
    atraso_medio_AS = cont_atraso_AS/(double)h;
    printf("\nThe average time between the arrival of a call to the general-purpose and the call answering by the area-specific: %lf\n", atraso_medio_AS);
    printf("ABSOLUTE ERROR = %lf  s ",erro_absoluto);
    printf("RELATIVE ERROR = %lf   %c ",erro_relativo,percentagem);
    plotResultsDelay(EixoX,histogramaY,10);
    //histograma do valor (avg_waiting_time - atraso_medio)
    plotResultsAVGWT(EixoX,histogramaY1,cont);
    


}
