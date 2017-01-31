#include "STM32F4xx.h"
#include "menu.h"
#include "CommonFunctions.h"
#include "LCD.h"
#include "SWT.h"
#include "LED.h"

void initLCD(){
	
	LCD_Initpins();	
	LCD_DriverOn();
	
	Delay(10);
	LCD_Init();

	LCD_DriverOn();
	LCD_On(1);
	Delay(20);
	LCD_Clear();
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
	
	//Run Multimeter
	menu();
	return 0;
}
