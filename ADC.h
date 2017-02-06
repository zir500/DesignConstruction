#ifndef __ADC_
#define __ADC_


void ADC1_init(void) ;
unsigned int read_ADC1 (void) ;
void display_Measure(char* Measurement, char* Unit, int value);

#endif //__ADC_
