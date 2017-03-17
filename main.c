#include "STM32F4xx.h"
#include "CommonFunctions.h"
#include "LCD/lcd_driver.h"
#include "SWT.h"
#include "LED.h"
#include "menu.h"
#include "ADC.h"
#include "serial.h"
#include<stdlib.h>
#include <string.h>


#define BUFFER_SIZE 128 //TODO what is this????

extern int MULTIMETER_MODE;

void initLCD(){
	
	lcd_init(LCD_LINES_TWO, LCD_CURSOR_OFF, LCD_CBLINK_OFF, BUFFER_SIZE);
	lcd_clear_display();
}


void initButtons(){
	
  //user button 
	 RCC->AHB1ENR  |= ((1UL <<  0) );              /* Enable GPIOA clock         */

  GPIOA->MODER    &= ~((3UL << 2*0)  );         /* PA.0 is input              */
  GPIOA->OSPEEDR  &= ~((3UL << 2*0)  );         /* PA.0 is 50MHz Fast Speed   */
  GPIOA->OSPEEDR  |=  ((2UL << 2*0)  ); 
  GPIOA->PUPDR    &= ~((3UL << 2*0)  );         /* PA.0 is no Pull up         */
	
  SWT_Init();
}	



//Test function for now
void listen_port_test(){
	char* receivedString;
	lcd_write_string("Listenning", 1, 0);

	while(1) {
		if(USART2->SR & USART_SR_RXNE){
			char lengthOfMsg = (char)USART2->DR;
			//First byte should be the length of the string.
			receivedString = (char*)malloc(sizeof(char)*lengthOfMsg);
			
			if(receivedString != NULL){
				//Now that we have received the length of the message proceed to read the rest of the message
				int numChars = 0;
				char recieved = '\0';
				while (numChars < lengthOfMsg){
					if(USART2->SR & USART_SR_RXNE){
						recieved = (char)USART2->DR;
						receivedString[numChars] = recieved;
						numChars++;
					}
				}
				//Entire message received, append a string temination for convinience.
				receivedString[numChars] = '\0';

					send_packet("ack", 3);
				//DEBUG
				lcd_clear_display();
				lcd_write_string(receivedString, 1, 0);
				free(receivedString);
			}
		}
	}
}



int main(){
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
		
	//Initialisers
	initLCD();
	//initButtons();
	LED_Init();
	init_GIPOB();
	ADC1_init();
	init_TIM7();
	init_GPIOE();
	serial_init(); //either initButtons or serial_init should be commented as they are using the same port

		//printf("START");
	listen_port_test();
	
	/*uint32_t counter = 0;
	while(1) {
		Delay(2000);	
		counter++;
		printf("count() called, value = %d \n", counter);
	}*/
	
	
	//buzzerOn();
	//Run Multimeter
	//menu();
	
	return 0;
}
