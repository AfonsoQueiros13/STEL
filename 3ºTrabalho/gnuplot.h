#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
void plotResults(double* xData, int* yData, int dataSize) {
  	FILE *gnuplotPipe,*tempDataFile;
  	char *tempDataFileName;
  	double x;
	int y;
  	int i;
  	tempDataFileName = "data.temp";
  	gnuplotPipe = popen("gnuplot -persist ","w");


 	if (gnuplotPipe)
  	{
		char * commandsForGnuplot[] = {
    	"set title \"Call Arrivals\"",
    	"set xlabel \"Time(s)\"",
    	"set ylabel \"Arrival Rate\"",
    	"plot 'data.temp' with lines"
		};

  	tempDataFile = fopen(tempDataFileName,"w");

	for (i=0; i < 4; i++)
	{
		fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]);
	}

    fflush(gnuplotPipe);

    for (i=0; i < dataSize; i++)
	{
        x = xData[i];
      	y = yData[i];
        fprintf(tempDataFile,"%lf %d\n",x,y);
  	}

	fclose(tempDataFile);
	fprintf(gnuplotPipe,"exit \n");
	sleep(1);
	remove(tempDataFileName);
  }
  else
  {
    printf("gnuplot not found...");
  }
}