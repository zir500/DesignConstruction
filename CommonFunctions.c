#include "STM32F4xx.h"
#include "CommonFunctions.h"
#include "SWT.h"

volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {                                              
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

/*----------------------------------------------------------------------------
   delays number of tick Systicks (happens every 1 ms) until a button is pressed 
  or times out and returns -1 if no button was pressed otherwise returns 
  the index of the button pressed.  Returns 8 if User button was Pressed.
 *----------------------------------------------------------------------------*/
int DelayForButton (uint32_t dlyTicks) {                                              
  uint32_t curTicks;

  curTicks = msTicks;
	int i=0;
  while ((msTicks - curTicks) < dlyTicks){
			for (i=0; i<=7; i++){
				if (SWT_Check(i)){
					return i; 
				}
			}
			if (BTN_Get()){
				return 8;
			}
	};
	return -1;
}

/*----------------------------------------------------------------------------
   reads the user button
 *----------------------------------------------------------------------------*/
uint32_t BTN_Get(void) {
	return (GPIOA->IDR & (1UL << 0 ));
}











