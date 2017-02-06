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
/*----------------------------------------------------------------------------
   reads the user button
 *----------------------------------------------------------------------------*/
void waitForRelease() {
		
	while (SWT_Get() || BTN_Get()){};
}

/*----------------------------------------------------------------------------
   init GIPOB
 *----------------------------------------------------------------------------*/
void init_GIPOB(){
	
  RCC->AHB1ENR  |= ((1UL <<  3) );         /* Enable GPIOB clock                */

  GPIOB->MODER    &= ~((3UL << 2* 4) |
                       (3UL << 2* 9) |
                       (3UL << 2*10) |
                       (3UL << 2*11) |
                       (3UL << 2*12) |
                       (3UL << 2*13) |
                       (3UL << 2*14) |
                       (3UL << 2*15)  );   /* PD.8..15 is output               */
  GPIOB->MODER    |=  ((1UL << 2* 4) |
                       (1UL << 2* 9) | 
                       (1UL << 2*10) | 
                       (1UL << 2*11) | 
                       (1UL << 2*12) | 
                       (1UL << 2*13) | 
                       (1UL << 2*14) | 
                       (1UL << 2*15)  ); 
  GPIOB->OTYPER   &= ~((1UL <<    4) |
                       (1UL <<    9) |
                       (1UL <<   10) |
                       (1UL <<   11) |
                       (1UL <<   12) |
                       (1UL <<   13) |
                       (1UL <<   14) |
                       (1UL <<   15)  );   /* PD.8..15 is output Push-Pull     */
  GPIOB->OSPEEDR  &= ~((3UL << 2* 4) |
                       (3UL << 2* 9) |
                       (3UL << 2*10) |
                       (3UL << 2*11) |
                       (3UL << 2*12) |
                       (3UL << 2*13) |
                       (3UL << 2*14) |
                       (3UL << 2*15)  );   /* PD.8..15 is 50MHz Fast Speed     */
  GPIOB->OSPEEDR  |=  ((2UL << 2* 4) |
                       (2UL << 2* 9) | 
                       (2UL << 2*10) | 
                       (2UL << 2*11) | 
                       (2UL << 2*12) | 
                       (2UL << 2*13) | 
                       (2UL << 2*14) | 
                       (2UL << 2*15)  ); 
  GPIOB->PUPDR    &= ~((3UL << 2* 4) |
                       (3UL << 2* 9) |
                       (3UL << 2*10) |
                       (3UL << 2*11) |
                       (3UL << 2*12) |
                       (3UL << 2*13) |
                       (3UL << 2*14) |
                       (3UL << 2*15)  );   /* PD.8..15 is Pull up              */
  GPIOB->PUPDR    |=  ((1UL << 2* 4) |
                       (1UL << 2* 9) | 
                       (1UL << 2*10) | 
                       (1UL << 2*11) | 
                       (1UL << 2*12) | 
                       (1UL << 2*13) | 
                       (1UL << 2*14) | 
                       (1UL << 2*15)  ); 
}

/*----------------------------------------------------------------------------
   turns a GPIO on
 *----------------------------------------------------------------------------*/
void outputSignalON(unsigned int Switch) {
	GPIOB->BSRR = (1U << Switch); 
}

/*----------------------------------------------------------------------------
   turns a GPIO off
 *----------------------------------------------------------------------------*/
void outputSignalOFF(unsigned int Switch) {
		GPIOB->BSRR = (1U << Switch) << 16; 

}


