#include "STM32F4xx.h"
#include "CommonFunctions.h"
#include "LCD/lcd_driver.h"
#include "SWT.h"
#include "LED.h"
#include "menu.h"
#include "ADC.h"


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

int main(){
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
	
	//Initialise
	initLCD();
	initButtons();
	LED_Init();
	init_GIPOB();
	ADC1_init();
	
	//Run Multimeter
	menu();
	while(1){
		Delay(500);
		unsigned int value = read_ADC1();
		switch(MULTIMETER_MODE){
			case MODE_VOLTAGE:
				display_Measure("Voltage", "V", 3*4096/value);
				break;
					
			case MODE_CURRENT:
				display_Measure("Current", "A", value);
				break;
		}
	}
	
	
	
	return 0;
}
