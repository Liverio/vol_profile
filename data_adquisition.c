#include <stdio.h>
#include <string.h>
#include "data_adquisition.h"

char report_data;
int SAMPLES;

void read_data(char *ticker, tp_sample *sample, char *source_date){
	// Input data
	FILE *f_data;
	char path[100];
	char trash[100];

	// Sample fields
	char date[9];
	char time[9];
	float open, high, low, close;
	int vol;
	int day_no, month_no;

	sprintf(path, "../input_data/%s/ticks/%s.txt", ticker, source_date);
	f_data = fopen(path, "r");

	// Read and store end-of-day data
	// Format:	<TICKER>,<PER>,<DTYYYYMMDD>,<TIME>,<OPEN>,<HIGH>,<LOW>,<CLOSE>,<VOL>,<OPENINT>
	//			.IBEX,I,20050103,090100,9085.2,9085.2,9085.2,9085.2,2710,0
	// Store OPEN, HIGH, LOW, CLOSE, consider DATE to know when to go ahead, and discard anything else

	// Skip header
	fscanf(f_data, "%s", trash);
	
	SAMPLES = 0;
		
	// Skip data until initial date is reached
	char year[5]; 
	do{
		fscanf(f_data, "%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%f,%d,%s", trash, trash, date, time, &open, &high, &low, &close, &vol, trash);
		date[8] = '\0';
		memcpy(year, date, 4*sizeof(char));
		year[4] = '\0';
	} while (strcmp(year, initial_date));

	do{
		// Discard data where time is later than close_time
		time[8] = '\0';
		memcpy(sample[SAMPLES].date, date, 8*sizeof(char));
		memcpy(sample[SAMPLES].time, time, 8*sizeof(char));
		sample[SAMPLES].date[8] = '\0';
		sample[SAMPLES].time[8] = '\0';
		sample[SAMPLES].open  = open;
		sample[SAMPLES].close = close;
		sample[SAMPLES].max = high;
		sample[SAMPLES].min = low;
		sample[SAMPLES].vol = vol;
		// #day will be used to optimize according to std dev
		if (SAMPLES == 0){
			day_no = 0;
			month_no = 0;
		}
		else if (strcmp(sample[SAMPLES].date, sample[SAMPLES-1].date)){
			day_no++;
			// next month?
			if ((sample[SAMPLES].date[4] != sample[SAMPLES-1].date[4]) || (sample[SAMPLES].date[5] != sample[SAMPLES-1].date[5]))
				month_no++;
		}
		
		sample[SAMPLES].day_no = day_no;
		sample[SAMPLES].month_no = month_no;
			
		SAMPLES++;
	} while (fscanf(f_data, "%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%f,%d,%s", trash, trash, date, time, &open, &high, &low, &close, &vol, trash) != -1);
		
	fclose(f_data);
	return ;
}
