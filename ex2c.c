#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "struct_eventos.c"
#include <time.h>

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
    unsigned int n = 0, cont = 0, queue = 0, est_queue = 0;
    int numCanais = 0;
    double time_init = 0.0;
    double atraso_medio = 0.0;
    double cont_atraso = 0.0;
    clock_t start_delay, end_delay;
    int aux = 0.0;
    char busy = FALSE, flag = FALSE;
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
    /*********************************************FIM INICIALIZAÇÕES************************************************/
    printf("\nNumber of channels: ");
    scanf("%d", &numCanais);
    aux = numCanais;
    printf("\n Simulation Time (s): ");
    scanf("%lf", &time_simulation);
    printf("\nIntroduce minimum delay for compare (s) : ");
    scanf("%lf", &delay);
    delay = delay / 1000.0;
    printf("\nIntroduce Lmax (queue max) : ");
    scanf("%d", &queue_max);
    /*********************Inicio de  chegada de chamadas a 0.0 segundos********************************************/
    lista_eventos = adicionar(lista_eventos, CHEGADA, 0.0);

    while (time_init < time_simulation)
    {
        /* carrega proximo evento */
        time_init = lista_eventos->tempo;
        printf("\n\n\nEvento do tipo: %d -- No tempo: %lf", lista_eventos->tipo, lista_eventos->tempo);

        if (lista_eventos->tipo == CHEGADA)
        {
            if (queue_max != 0)
            {

                if (numCanais == 0 && queue < queue_max)
                {
                    start_delay = time_init;
                    printf("\nstart_delay: %lf", time_init);
                    start_delays[i] = (double)start_delay;
                    i++;
                    queue++;
                    est_queue++;
                }

                if (queue == queue_max)
                {
                    packet_loss++;
                }

                if (numCanais > 0 && queue < queue_max)
                {
                    numCanais--;
                    d = getD(dm);
                    lista_eventos = adicionar(lista_eventos, PARTIDA, time_init + d);
                }
                c = getC(lambda);
                lista_eventos = adicionar(lista_eventos, CHEGADA, time_init + c);
                cont++;
            }
            if (queue_max == 0)
            {
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
                        d = getD(dm);
                        lista_eventos = adicionar(lista_eventos, PARTIDA, time_init + d);
                    }
                }

                c = getC(lambda);
                lista_eventos = adicionar(lista_eventos, CHEGADA, time_init + c);
                cont++;
            }
        }

        if (lista_eventos->tipo == PARTIDA)
        {
            // liberta Canal
            if (queue_max != 0)
            {
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

            if (queue_max == 0)
            {
                // liberta Canal
                if (numCanais < aux)
                {
                    numCanais++;
                    busy = FALSE;
                }
            }
        }
        if (lista_eventos->tipo != PARTIDA && lista_eventos->tipo != CHEGADA)
            printf("\n\nNão existem pacotes gerados!\n\n");

        lista_eventos = (lista *)lista_eventos->proximo;
    }

    for (int i = 0; i < est_queue; i++)
    {
        if (atraso_pacotes[i] > delay)
        {
            contador_delay++;
        }
    }

    printf("\n\n\t\t***************RESULTADOS*******************\n");
    if (queue_max == 0)
    {
        prob = ((double)neg / (double)cont) * 100.0;
        printf("Percentagem de bloco: %lf", prob);
    }
    printf("\nChamadas na fila: %d", queue);
    printf("\nChamadas que entraram na fila: %d", est_queue);
    printf("\nContagem: %d", cont);
    prob = ((double)est_queue / (double)cont) * 100.0; //primeiro calculo pedido (prob delay)
    printf("\nDelay probability: %lf", prob);
    atraso_medio = (cont_atraso / cont);
    printf("\nAtraso medio de todos os pacotes : %lf (s)", atraso_medio);
    estimator_delay = (double)(contador_delay / (double)cont) * 100.0;
    printf("\nProbabilidade de ser maior do que %lf (s) é  : %lf % ", delay, estimator_delay);
    printf("\nNumber of packets losses:  %d", packet_loss);
    printf("\nPacket Loss probability (%) = %lf ", (((double)packet_loss / (double)cont)) * 100.0);

    return 0;
}
