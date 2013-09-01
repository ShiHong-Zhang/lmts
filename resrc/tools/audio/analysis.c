#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#define OFFSET 		88244
#define OFFSET 		44
#define SAMPLE_RATE	44100
#define PERIODS		20
#define SLIDERLEN	5

#define LOG_OUTPUT	stdout


unsigned int jump_step = 1;

signed short readvalue(char unit[], FILE *fp)
{
	signed short value;
	unsigned short t;
	
	// we will protect to avoid meet the end of this file
	if (!feof(fp))
		// read two bytes from file
		fread(unit, 2, 1, fp);
	if (jump_step == 2)
		fseek(fp, 2, SEEK_CUR);
	
	t = ((unit[1] & 0x00ff) << 8) | (unit[0] & 0x00ff);
	value = (signed short)t;
	
	return value;
}
	

int main(int argc, char *argv[])
{
	FILE *fp;
	// peaks
	int peak[PERIODS] = {0};
	int peak_amp[PERIODS] = {0};
	int average_peak_amp = 0;
	int variance_peak_amp = 0;
	float average_peak = 0;
	float variance_peak = 0;
	int peak_index = 0;
	// valleys
	int valley[PERIODS] = {0};
	int valley_amp[PERIODS] = {0};
	int average_valley_amp = 0;
	int variance_valley_amp = 0;
	float average_valley = 0;
	float variance_valley = 0;
	int valley_index = 0;
	// up and down zero point
	int upzero[PERIODS] = {0};
	int upzero_index = 0;
	float average_upzero = 0;
	float variance_upzero = 0;
	
	int downzero[PERIODS] = {0};
	int downzero_index = 0;
	float average_downzero = 0;
	float variance_downzero = 0;
	
	char unit[2] = {0};
	signed short tmp;
	float ftmp;
	signed short slider[SLIDERLEN];
	
	int i = 0, j = 0;
	int sum = 0;
	float variance_sum = 0;
	int t = 0;
	
	float percent_freq_error = 0.0;
	float percent_average_symmetry_amp = 0.0;
	float percent_variance_peak_amp = 0.0;
	float percent_variance_valley_amp = 0.0;
	float percent_variance_freq = 0.0;
	
	int flag_wave_form = 0;
	int flag_wrong_freq = 0;
	int flag_is_noise = 0;
	int flag_is_asymmetry = 0;
	int flag_failed = 0;
	int frequency = 0;
	
	unsigned short channels = 1;
	unsigned short sample_rate = SAMPLE_RATE;
	
	if (argc < 2 || argc > 3) {
		fprintf(LOG_OUTPUT, "Usage: ./analysis arg1 [freq]\n");
		exit(1);
	}
	
	if ((fp = fopen(argv[1], "rb")) == NULL) { 
		fprintf(LOG_OUTPUT, "Can't open this file: %s\n", argv[1]);
		fclose(fp);
		exit(1);		
	}
	
	if (argc == 3) {
		frequency = atoi(argv[2]);
		if (frequency == 0)
			frequency = 1000;
	}
	else {
		frequency = 1000;	
	}	
	
	// Jump to the NumChannels data area
	fseek(fp, 22, SEEK_SET);
	channels = readvalue(unit, fp);
	// mono
	if (channels == 1) {
		jump_step = 1;
	}
	// stereo
	else if (channels == 2) {
		jump_step = 2;
	}
	else {
		fprintf(LOG_OUTPUT, "The input file is not mono or stereo.\n");
		fclose(fp);
		exit(1);
	}

	sample_rate = readvalue(unit, fp);
	// if sample rate don't equal 44100(0xac44) and 48000(0xbb80)
	if (sample_rate != 0xac44 && sample_rate != 0xbb80) {
		fprintf(LOG_OUTPUT, "The input file's sample rate is not 44.1K or 48K.\n");
		fclose(fp);
		exit(1);
	}
	
	rewind(fp);

	// jump to the end of 1s' data
	fseek(fp, sample_rate * 2 + OFFSET, SEEK_SET);
	for (i=0; i<SLIDERLEN; i++) {
		slider[i] = readvalue(unit, fp);
	}
	i = SLIDERLEN - 1;
	do {
		int flag_jmp = 0;
		if (slider[0] < 0 && slider[2] > 0 && slider[1] >= 0) {
			upzero[upzero_index] = i - (SLIDERLEN - 2);
			upzero_index++;
			flag_jmp = 1;			
		}

		if (slider[0] > 0 && slider[2] < 0 && slider[1] <= 0) {
			downzero[downzero_index] = i - (SLIDERLEN - 2);
			downzero_index++;
			flag_jmp = 1;
		}
		
		// NOTE: here can't write two '<='	
		if (slider[2] > 0 
			&& slider[0] < slider[2] 
			&& slider[1] <= slider[2]
			&& slider[2] >= slider[3]
			&& slider[2] > slider[4]) {
						
			peak[peak_index] = i - (SLIDERLEN - 2);
			peak_amp[peak_index] = slider[SLIDERLEN - 2];
			peak_index++;
			flag_jmp = 1;
		}

		// NOTE: here can't write two '>='
		if (slider[2] < 0 
			&& slider[0] > slider[1] 
			&& slider[1] >= slider[2]
			&& slider[2] <= slider[3]
			&& slider[3] < slider[4]) {

			valley[valley_index] = i - (SLIDERLEN - 2);
			valley_amp[valley_index] = slider[SLIDERLEN - 2];
			valley_index++;
			
			flag_jmp = 1;
		}
		
		if (!flag_jmp) {
			i++;
			tmp = readvalue(unit, fp);
			// printf("value = %x\n", tmp);
			for (j=0; j<SLIDERLEN-1; j++)
				slider[j] = slider[j+1];
			slider[SLIDERLEN-1] = tmp;
		}
		else {
			fseek(fp, (sample_rate/frequency/8 + 1 - SLIDERLEN)*2*jump_step, SEEK_CUR);
			for (j=0; j<SLIDERLEN; j++) {
				slider[j] = readvalue(unit, fp);
			}
			i += (sample_rate/frequency/8 + 1);
		}
	} while ( peak_index < PERIODS && upzero_index < PERIODS && i < sample_rate && !feof(fp) );

	//===============================================================
	// Counting and Calcaulating	
	//===============================================================
	
	// average peak amp
	sum = 0; t = 0; 
	for (i=0; i<PERIODS; i++) {
		if (peak_amp[i] > 0) {
			sum += peak_amp[i];
			t++;
		}
	}
	if (t > 0)
		average_peak_amp = sum / t;
	
	variance_sum = 0; t = 0;
	for (i=0; i<PERIODS; i++) {
		if (peak_amp[i] > 0) {
			ftmp = (peak_amp[i] - average_peak_amp);
			variance_sum += ftmp * ftmp;
			t++;
		}
	}
	if (t > 0)
		variance_peak_amp = sqrt(variance_sum / t);
	
	// average peak index
	sum = 0; t = 0;
	for (i=0; i<PERIODS - 1; i++) {
		if (peak[i+1] - peak[i] > 0) {
			sum += peak[i+1] - peak[i];
			t++;
		}
	}
	if (t > 0)
		average_peak = sum * 1.0 / t;
	
	variance_sum = 0; t = 0;
	for (i=0; i<PERIODS - 1; i++) {
		if (peak[i+1] - peak[i] > 0) {
			ftmp = (peak[i+1] - peak[i] - average_peak);
			variance_sum += ftmp * ftmp;
			t++;
		}
	}
	if (t > 0)
		variance_peak = sqrt(variance_sum / t);

	sum = 0; t = 0;
	// average valley amp
	for (i=0; i<PERIODS; i++) {
		if (valley_amp[i] < 0) {
			sum += valley_amp[i];
			t++;
		}
	}
	if (t > 0)
		average_valley_amp = sum / t;

	variance_sum = 0; t = 0;
	for (i=0; i<PERIODS; i++) {
		if (valley_amp[i] < 0) {
			ftmp = (valley_amp[i] - average_valley_amp);
			variance_sum += ftmp * ftmp;
			t++;
		}
	}
	if (t > 0)
		variance_valley_amp = sqrt(variance_sum / t);

	// average valley index
	sum = 0; t = 0;
	for (i=0; i<PERIODS - 1; i++) {
		if (valley[i+1] - valley[i] > 0) {
			sum += valley[i+1] - valley[i];
			t++;
		}
	}
	if (t > 0)
		average_valley = sum / t;

	variance_sum = 0; t = 0;
	for (i=0; i<PERIODS - 1; i++) {
		if (valley[i+1] - valley[i] > 0) {
			ftmp = (valley[i+1] - valley[i] - average_valley);
			variance_sum += ftmp * ftmp;
			t++;
		}
	}
	if (t > 0)
		variance_valley = sqrt(variance_sum / t);
	
	// average up zero point index
	sum = 0; t = 0;
	for (i=0; i<PERIODS - 1; i++) {
		if (upzero[i+1] - upzero[i] > 0) {
			sum += upzero[i+1] - upzero[i];
			t++;
		}
	}
	if (t > 0)
		average_upzero = sum * 1.0 / t;

	variance_sum = 0; t = 0;
	for (i=0; i<PERIODS - 1; i++) {
		if (upzero[i+1] - upzero[i] > 0) {
			ftmp = (upzero[i+1] - upzero[i] - average_upzero);
			variance_sum += ftmp * ftmp;
			t++;
		}
	}
	if (t > 0)
		variance_upzero = sqrt(variance_sum / t);
	
	// average down zero point index
	sum = 0; t = 0;
	for (i=0; i<PERIODS - 1; i++) {
		if (downzero[i+1] - downzero[i] > 0) {
			sum += downzero[i+1] - downzero[i];
			t++;
		}
	}
	if (t > 0)
		average_downzero = sum * 1.0 / t;

	variance_sum = 0; t = 0;
	for (i=0; i<PERIODS - 1; i++) {
		if (downzero[i+1] - downzero[i] > 0) {
			ftmp = (downzero[i+1] - downzero[i] - average_downzero);
			variance_sum += ftmp * ftmp;
			t++;
		}
	}
	if (t > 0)
		variance_downzero = sqrt(variance_sum / t);

	//====================================================================
	// Judgement
	//====================================================================
	
		
	if (peak_index < 1 || upzero_index < 3) {
		flag_wave_form = 1;
		flag_failed = 1;	
	}
	
	// peak numbers and upzero numbers must be matched.
	if (peak_index > upzero_index + 1 || peak_index < upzero_index - 1) {
		//fprintf(LOG_OUTPUT, "Not regulary this wave form is on amplitude.\n");
		flag_wave_form = 1;
	}
	
	percent_variance_freq = (variance_upzero * 1.0 / average_upzero) * 100;
	// 2% error permitted
	if (percent_variance_freq > 2) {
		//fprintf(stderr, "Not regulary this wave form is on time axis.\n");
		flag_wave_form = 1;
	}

	percent_average_symmetry_amp = ((average_peak_amp - abs(average_valley_amp)) * 1.0 / average_peak_amp) * 100;
	// 1% error permitted
	if ( percent_average_symmetry_amp > 1) {
		//fprintf(stderr, "Wave asymmetry degree is larger than 1%%\n");
		flag_is_asymmetry = 1;
	}

	// judeg whether it is noise 
	percent_freq_error = (1 - (sample_rate / average_upzero) * 1.0 / frequency) * 100;
	// 2% error permitted
	if (percent_freq_error > 2) {
		//fprintf(stderr, "Wave frequency error rate is larger than 2%%.\n");
		flag_wrong_freq = 1;
		flag_is_noise = 1;
	}

	//========================================================================
	// Message Output
	//========================================================================
	if (flag_failed)
		fprintf(LOG_OUTPUT, "Get no valid values, the following counting data may be WRONG!\n");
		
	fprintf(LOG_OUTPUT, "peak_index:%d\n", peak_index);
	fprintf(LOG_OUTPUT, "upzero_index:%d\n", upzero_index);
	fprintf(LOG_OUTPUT, "average_upzero:%f\n", average_upzero);
	fprintf(LOG_OUTPUT, "Frequency(Hz):%d\n", (int)(sample_rate / average_upzero));
	fprintf(LOG_OUTPUT, "Frequency Errors(%%):%f\n", percent_freq_error);
	fprintf(LOG_OUTPUT, "Frequency Variance(%%):%f\n", percent_variance_freq);
	fprintf(LOG_OUTPUT, "Amplitude:%f\n", average_peak_amp * 1.0 / 0x7fff);
	

	fclose(fp);
	return 0;
}
