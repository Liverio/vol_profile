#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "data_adquisition.h"
#include "math.h"

extern int SAMPLES;

// Full data
tp_sample *sample;

int main(int argc, char *argv[]){
	int i, j;
    char ticker[10];
    
    sample = (tp_sample *)malloc(MAX_SAMPLES * sizeof(tp_sample));
    
	strcpy(ticker, argv[1]);
	read_data(ticker, sample, argv[2]);
	
	i = 0;
	// Set trading range
	int min = 100000;
	int max = 0;
	while (i < SAMPLES){
		if ((int)(sample[i].close) < min)
			min = (int)(sample[i].close);
		
		if ((int)(sample[i].close) > max)
			max = (int)(sample[i].close);

		i++;
	}
	
	// Allocate mem for the trading range
	int *trading_range;
	trading_range = (int *)malloc((max - min + 1) * sizeof(int));
	
	for (i = 0; i < max - min + 1; i++)
		trading_range[i] = 0;
		
	// Fill trading range
	while (i < SAMPLES){
		(trading_range[(int)(sample[i].close) - min])++;	
		i++;
	}	
	
	// Print vol profile
	for (i = min; i <= max; i++)
		printf("%d\t%d\n", i, trading_range[i - min]);
	
	exit(0);
}




