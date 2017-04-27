#ifndef __ADC_
#define __ADC_


void ADC1_init(void) ;
unsigned int read_ADC1 (void) ;
void display_Measure(char* measurement, char rangeMode, char* range, char* unit, float value, int n);

#endif //__ADC_
