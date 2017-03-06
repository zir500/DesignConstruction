#include "STM32F4xx.h"
#include "CommonFunctions.h"
#include "SWT.h"
#include "menu.h"

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
int DelayForButton (uint32_t dlyTicks, int buttons[], int size) {                                              
  uint32_t curTicks;

  curTicks = msTicks;
	int i=0;
  while ((msTicks - curTicks) < dlyTicks){
			for (i=0; i<=size-1; i++){
				if (buttons[i] != 8) {
					if (SWT_Check(buttons[i])){
						return buttons[i]; 
					}
					
				} else {
					if (BTN_Get()) {
						return 8;
					}
				}
		}
	}
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
	
  RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOBEN;         /* Enable GPIOB clock                */

  GPIOB->MODER    &= ~((3UL <<  2* 4) |		//TODO: Check whether there are constants to initialise these registers
                       (3UL <<  2* 5) |
                       (3UL <<  2*7) |
											 (3UL <<  2*8) |
											 (3UL <<  2*15) );   /* PB.4,5,7,8, 15 is output               */
  GPIOB->MODER    |=  ((1UL <<  2* 4) |
                       (1UL <<  2* 5) | 
                       (1UL <<  2*7) |
											 (1UL <<  2*8) |
											 (1UL <<  2*15)); 
  GPIOB->OTYPER   &= ~((1UL <<    4) |
                       (1UL <<    5) |
                       (1UL <<    7) |
											 (1UL <<    8) |
											 (1UL <<  	15));   /* PB 4,5,7,8, 15 is output Push-Pull     */
  GPIOB->OSPEEDR  &= ~((3UL << 2* 4) |
                       (3UL << 2* 5) |
                       (3UL << 2* 7) |
											 (3UL << 2*8) |
											 (3UL << 2*15));   /* PB.4,5,7,8, 15 is 50MHz Fast Speed     */
  GPIOB->OSPEEDR  |=  ((2UL << 2* 4) |
                       (2UL << 2* 5) | 
                       (2UL << 2* 7) |
											 (2UL << 2*8) 	|
											 (2UL << 2*15)); 
  GPIOB->PUPDR    &= ~((3UL << 2* 4) |
                       (3UL << 2* 5) |
                       (3UL << 2* 7) |
											 (3UL << 2*8) |
											 (3UL << 2*15));   /* PB.4,5,7,8,15 is Pull up              */
  GPIOB->PUPDR    |=  ((1UL << 2* 4) |
                       (1UL << 2* 5) | 
                       (1UL << 2* 7) |
											 (1UL << 2* 8)  |
											 (1UL <<  2*15)); 
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

void selectMode(unsigned int mode) {

	if (mode == 0xF) { //default mode, enable = 0
		outputSignalOFF(4);
		outputSignalOFF(5);
		outputSignalOFF(7);
		outputSignalOFF(8);
		outputSignalOFF(15);
	} else {
		outputSignalON(0xF);
		
		GPIOB->ODR |= (mode & 0x3) << 4;
		GPIOB->ODR |= (mode & (0x3<<2)) << 5;

	}
}

void init_GPIOE(){
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOEEN;         /* Enable GPIOE clock        */

  GPIOE->MODER    &= ~((3UL <<  2* 3) );   /* PE.3 is output               */
  GPIOE->MODER    |=  ((1UL <<  2* 3) ); 
  GPIOE->OTYPER   &= ~((1UL <<     3) );   /* PE 3 is output Push-Pull     */
  GPIOE->OSPEEDR  &= ~((3UL << 2* 3)  );   /* PE.3,50MHz Fast Speed       */
  GPIOE->OSPEEDR  |=  ((2UL << 2* 3)  ); 
  GPIOE->PUPDR    &= ~((3UL << 2* 3)  );   /* PE.3 is Pull up             */
  GPIOE->PUPDR    |=  ((1UL << 2* 3)  ); 
}

//turn on the buzzer for a specified number of milli-seconds 
void buzzerOn() { 
	GPIOE->BSRR |= 1U << 3;
	
	TIM7->DIER &= ~TIM_DIER_UIE;
	TIM7->EGR |= TIM_EGR_UG;
	TIM7->SR = 0;
	TIM7->DIER |= TIM_DIER_UIE; 					/*Enable interrupts */
	TIM7->CR1 |= TIM_CR1_CEN;  //stop timer
}

void buzzerOFF() {
	
	GPIOE->BSRR |= (1U <<3) << 16;					/*Enable interrupts */
	TIM7->CR1 &= ~TIM_CR1_CEN;  //stop timer
}

//init TIM7
void init_TIM7() {
	
 	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; 	/*Enable clock*/
 	TIM7->PSC = 42000; 											
	TIM7->ARR = 1000;  //set to 3 seconds
	TIM7->DIER |= TIM_DIER_UIE; 					/*Enable interrupts */
	NVIC_EnableIRQ(TIM7_IRQn); 				/*Register interrupt*/

}

//handle interrupts 
void TIM7_IRQHandler(void) {
	
	/*Resets the interrupt flag*/
	TIM7->SR &= ~TIM_SR_UIF;
	TIM7->CR1 &= ~TIM_CR1_CEN;
	
	buzzerOFF();
}
