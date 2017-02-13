/*----------------------------------------------------------------------------
 * Name:    ADC.c

reads ADC channel */


#include <stdio.h>
#include "STM32F4xx.h"
#include "LCD/lcd_driver.h"

void ADC1_init(void) {
	
	RCC->APB2ENR  |= RCC_APB2ENR_ADC1EN;       /* Enable ADC1 clock                */
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOCEN;      /* Enable GPIOC clock               */
	GPIOC->MODER = 0xffffffff;
	GPIOC->PUPDR = 0; 												//input/output analog 
	ADC1->CR1 = 0x00;
	ADC1->CR1 |= ADC_CR1_DISCEN;
	ADC1->CR2 = 0x00;
	ADC1->CR2 |= ADC_CR2_EOCS;     	
	ADC->CCR = 0x00;							//set the ADC common control register
	ADC1->SQR1 = 0x01;						//set the reqular seq register 1 for 1 conversion at a time 
	//configuring the sample time register 1 
	ADC1->SMPR1 = 0x00; 
	ADC1->SMPR1 = 0x0300; 				//set number of cycles (56 for 11-base2) of chanel 13
	
	ADC1->SQR1 = 0x01;
	ADC1->SQR3 = 0x0e;								/* ADC_IN14 = 0x0e: ADC_IN15 = 0x0f */
	ADC1->CR2 |=  ADC_CR2_ADON;//(1UL << 0);
	
}
	
/* function to read ADC and retun value */
unsigned int read_ADC1 (void) {
	
	ADC1->CR2 |= (1UL << 30)	;		/* set SWSTART to 1 to start conversion */
	while (!(ADC1->SR & ADC_SR_EOC));  // cycles until the conversion ends
																		//ADC_SR_EOC is set to 1 when the conversion ended
	return (ADC1->DR);
}

/*function that displays a text and integer on separate lines of the LCD*/
void display_Measure(char* Measurement, char* Unit, float value){

	lcd_write_string(Measurement,0,0); 
	
	char t[16] = {' '};
	char spaces[16] = "               ";
	sprintf(t, "%.4f %s%s", value,Unit,spaces);
	lcd_write_string(t,1,0);
}
