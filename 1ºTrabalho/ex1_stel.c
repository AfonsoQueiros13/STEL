#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

double random_num;
double lambda;
double vmin;
double vmax;
int n;
double sample;
float infinito = INFINITY;

double ran_expo(double lambda, double values[],int histograma[]){ //gera numeros aleatorios pela distribuicao de poisson
      srand((unsigned)time(NULL));
      for(int h=1;h<n+1;h++){
        values[h-1]= (h-1)/(5*vmin);
        values[h]= h/(5*vmin);
        histograma[h-1]=0;
        printf("\n**Intervalo %d vai de %lf a %lf**",h-1,values[h-1],values[h]);
    }
      for(int i =0; i<n;i++){
      random_num = rand() / (RAND_MAX + 1.0);
      sample = -log(1- random_num) / lambda;
      if(sample > 5/lambda)
      sample=5/lambda;
      printf("\n////Sample number %d= %lf///\n",i,sample);
      for(int j=0;j<n;j++){
        if(sample >=values[j]  && sample <=values[j+1]){
          histograma[j]++;
          printf("\n//Colocou a sample %d na posicao %d//\n ",i,j);
        }
        if(j==n-1 && sample>=values[j]){
          histograma[j]++;
          printf("\n//Colocou a sample %d na posicao %d//\n ",i,j);
        }
    }
  }

}

int main(void)
{
  FILE *fp;
	printf("Choose Lambda:  ");
	scanf("%lf",&lambda);
  printf("Choose Number of discrete samples:  ");
	scanf("%d",&n);
  int *histograma= malloc(sizeof(double)*n);
  double *values= malloc(sizeof(double)*n);
	vmin= (1.0/5.0)*(1.0/lambda);
	vmax= 5.0*(1/lambda);
  printf("\nVmin: %lf",vmin);
  printf("\nVmax: %lf",vmax);
  ran_expo(lambda,values,histograma);
  fp = fopen("/home/afonso/Desktop/Pessoal Afonso Queirós/Mestrado FEUP/4º ano/STEL/Prática/1ºTrabalho/histograma.csv", "w");
  for(int i=0;i<n;i++){
    printf("\n Number of Samples in position %d of histogram: %d\n ",i,histograma[i]);

    if(fp == NULL){
      printf("Couldn't open file\n");
    return 0 ;
}
    fprintf(fp, "%d,%d\n", i,histograma[i]);

  }
  fclose(fp);
  return 0;

}
