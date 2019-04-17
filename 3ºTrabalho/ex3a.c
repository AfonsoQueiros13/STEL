/******************************INCLUDES DAS BIBLIOTECAS NECESSARIAS****************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "struct_eventos.c"
#include <time.h>
/******************************DEFINICOES DE ALGUMAS VARIAVEIS************************/
#define CHEGADA 0
#define PARTIDA 1
#define FALSE 0
#define TRUE 1
#define LAMBDA 0.02 // 0.02 chamadas/s 
#define DMGERAL 120 //duracao media da chamada geral
#define DMSPEC  150 //duracao media da chamada especifica
#define DGERALMIN 60  //duracao minima da chamada geral
#define DGERALMAX 300 //duracao maxima da chamada geral
#define DSPECMIN 30  //duracao minima da chamada especifica
#define DSPECMAX 120 //duracao maxuma da chamada especifica
#define GERAL_QUEUE 50
#define SPECIFIC_QUEUE 20000 //tamanho maximo da fila especifica
#define GAUSSMED 60 //media da distribuicao gaussiana
#define GAUSSDESVIO 20 //desvio da distribuicao gaussiana

/******************************FUNCOES USADAS NA MAIN*******************************************/
double rand_normal(double mean, double stddev)
{//Box muller method
    static double n2 = 0.0;
    static int n2_cached = 0;
    if (!n2_cached)
    {
        double x, y, r;
        do
        {
            x = 2.0*rand()/RAND_MAX - 1;
            y = 2.0*rand()/RAND_MAX - 1;

            r = x*x + y*y;
        }
        while (r == 0.0 || r > 1.0);
        {
            double d = sqrt(-2.0*log(r)/r);
            double n1 = x*d;
            n2 = y*d;
            double result = n1*stddev + mean;
            n2_cached = 1;
            return result;
        }
    }
    else
    {
        n2_cached = 0;
        return n2*stddev + mean;
    }
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


double getDGeral(double dm) //retorna c (chegada de uma chamada apartir do tempo atual)
{
    double c = 0;
    c = (-dm) * log(getRandom());
    return c;
}

double getDSpec(double dm) //retorna c (chegada de uma chamada apartir do tempo atual)
{
    double c = 0;
    c = (-dm) * log(getRandom());
    return c;
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
    
   /* printf("\n Simulation Time (s): "); //tempo de simulacao do programa
	scanf("%lf", &time_simulation);
    printf("\nGeral Queue (units): ");
    scanf("%d",&geral_queue);*/
    
    
    /*****************Inicio de  chegada de chamadas a 0.0 segundos*******************************/

	while (1)
	{
        sleep(1);
		/* carrega proximo evento */
		double random_gauss= rand_normal(GAUSSMED,GAUSSDESVIO);
        printf("\nnum_geral -> %lf",random_gauss/60.0);
       
		
    }
}