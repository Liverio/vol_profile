#ifndef DATA_ADQUISITION_H
#define DATA_ADQUISITION_H

#define MAX_SAMPLES 10000000

typedef struct {
        char date[11];
        char time[9];
        float open;
        float close;
		float max;
        float min;
        int vol;
        int day_no;
        int month_no;
} tp_sample;

static char initial_date[5] = "2017";

void read_data(char *ticker, tp_sample *sample, char *source_date);

#endif
