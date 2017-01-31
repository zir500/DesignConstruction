#include "STM32F4xx.h"
#include "menu.h"
#include "CommonFunctions.h"
#include "LCD.h"


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

int main(){
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
	
	//Initialise
	initLCD();
	
	//Run Multimeter
	openMenu();
	return 0;
}
