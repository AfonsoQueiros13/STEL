#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
void plotResultsDelay(double* xData, int* yData, int dataSize) {
  	FILE *gnuplotPipe,*tempDataFile;
  	char *tempDataFileName;
  	double x;
	int y;
  	int i;
  	tempDataFileName = "histogram_delay.temp";
  	gnuplotPipe = popen("gnuplot -persist ","w");


 	if (gnuplotPipe)
  	{
		char * commandsForGnuplot[] = {
    	"set title \"Delay Histogram\"",
    	"set xlabel \"Delay(s)\"",
    	"set ylabel \"Number of Calls\"",
    	"plot 'histogram_delay.temp' with lines"
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

void plotResultsAVGWT(double* xData, int* yData, int dataSize) {
  	FILE *gnuplotPipe,*tempDataFile;
  	char *tempDataFileName;
  	double x;
	int y;
  	int i;
  	tempDataFileName = "histogram_avg.temp";
  	gnuplotPipe = popen("gnuplot -persist ","w");


 	if (gnuplotPipe)
  	{
		char * commandsForGnuplot[] = {
    	"set title \"AVG Waiting Time\"",
    	"set xlabel \" Time(s)\"",
    	"set ylabel \"Number of Calls\"",
    	"plot 'histogram_avg.temp' with lines"
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