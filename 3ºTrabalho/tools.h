
/******************************DEFINICOES DE ALGUMAS VARIAVEIS************************/
#define KRED  "\x1B[31m" 
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\x1B[0m" //cores do texto do terminal
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
#define GERAL_QUEUE 2
#define SPECIFIC_QUEUE 20000 //tamanho maximo da fila especifica
#define GAUSSMED 60 //media da distribuicao gaussiana
#define GAUSSDESVIO 20 //desvio da distribuicao gaussiana
#define UMSEGUNDO 60
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

double getDGeral(double dm) //retorna d (partida de uma chamada apartir do tempo atual)
{
    double d = 0;
    d = (-dm) * log(getRandom());
    return d;
}

double getDSpec(double dm) //retorna c (partida de uma chamada apartir do tempo atual)
{
    double d = 0;
    d = (-dm) * log(getRandom());
    return d;
}