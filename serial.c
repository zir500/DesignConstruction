#include "serial.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "LED.h"

static void _configUSART2(uint32_t BAUD)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;	/* Enable USART2 Clock */

	GPIOA->MODER |= GPIO_MODER_MODER3_1;		//Setup PA3 as Input (USART2 Rx Pin)
	GPIOA->MODER |=  GPIO_MODER_MODER2_1;		/* Setup TX and  pin for Alternate Function */
	
	GPIOA->AFR[0] |= (7 << (4*2));		/* Setup RX as the Alternate Function */
	GPIOA->AFR[0] |= (7 << (4*3));		/* Setup TX as the Alternate Function */
	
	
  USART2->CR1 |= USART_CR1_UE;	/* Enable USART */
	
	USART2->CR1 &= ~USART_CR1_M;  //M bit - word length
	 
	USART2->CR2 &= ~USART_CR2_STOP;  //set to 1 stop bit

	USART2->BRR = SystemCoreClock/BAUD;  //the actual baud rate is given by this formula

	USART2->CR1 |= USART_CR1_TE;	/* Enable Tx */
	
	USART2->CR1 |= USART_CR1_RE;	/* Enable RX */
}

void serial_init(void) {
	_configUSART2(38400);
}

//Test function for now
void listen_port(){
	
	while(1) {
		
		if(USART2->SR & USART_SR_RXNE){
			char recieved = (char)USART2->DR;
			printf("\n %c", recieved);
			 LED_On(1);
	}
}
}
