#include "STM32F4xx.h"
#include "CommonFunctions.h"
#include <stdint.h>
#include <math.h>


#define DATA_PORT 5
#define W_CLK_PORT 8
#define FQ_UND_PORT 7
#define DDS_RESET 4
#define DDS_CLKIN 125000000

//Writes a number to the DDS
void write_number(uint32_t number) {

		for(int i=0; i<32; i++){
		GPIOB->ODR &= ~(1U << DATA_PORT);
		GPIOB->ODR |= ((number>>i) & 1) << DATA_PORT;
		GPIOB->ODR |= 1U << W_CLK_PORT;
		Delay(1);
		GPIOB->ODR &= ~(1U << W_CLK_PORT);
		Delay(1);
	}
}

//Writes 8 emptyt bits to the DDS (For skipping the control bits)
void write_no_control(){
	for(int i=0; i < 8;i++){
		Delay(1);
		GPIOB->ODR &= ~(1U << DATA_PORT);
		GPIOB->ODR |= 1U << W_CLK_PORT;
		Delay(1);
		GPIOB->ODR &= ~(1U << W_CLK_PORT);
	}
}

void setFrequency(unsigned long freq){
	GPIOB -> ODR |= 1U << DDS_RESET;
			Delay(1);
	GPIOB -> ODR &= ~(1U << DDS_RESET);
			//Delay(1);
	GPIOB->ODR |= 1U << W_CLK_PORT;
			Delay(1);
	GPIOB->ODR &= ~(1U << W_CLK_PORT);
			//Delay(1);
	GPIOB -> ODR |= 1U << FQ_UND_PORT;
			Delay(1);
	GPIOB -> ODR &= ~(1U << FQ_UND_PORT);
	
	uint32_t deltaPhase = (uint32_t) (freq * pow(2, 32)/DDS_CLKIN);
	write_number(deltaPhase);
	write_no_control();
	GPIOB -> ODR |= 1U << FQ_UND_PORT;
			//Delay(1);
	GPIOB -> ODR &= ~(1U << FQ_UND_PORT);
}
