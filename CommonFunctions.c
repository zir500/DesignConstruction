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
					if (SWT_Check(buttons[i])){
						return buttons[i]; 
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
		
	//TODO remove BTN_GET()
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
   turns a GPIOE on
 *----------------------------------------------------------------------------*/
void outputSignalON(unsigned int gpio_no) {
	GPIOE->BSRR = (1U << gpio_no); 
}

/*----------------------------------------------------------------------------
   turns a GPIOE off
 *----------------------------------------------------------------------------*/
void outputSignalOFF(unsigned int gpio_no) {
		GPIOE->BSRR = (1U << gpio_no) << 16; 
}

void selectMode(unsigned int mode) {

	if (mode == 0xF) { //default mode, enable = 0, all control signals 0
		outputSignalOFF(3);
		outputSignalOFF(4);
		outputSignalOFF(5);
		outputSignalOFF(6);
		outputSignalOFF(7);
	} else {
		//outputSignalON(0xF ); //turn on the enable signal
		
		//turn on the desired control signals
		GPIOE->ODR |= mode << 3;
	}
}

void init_GPIOE(){
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOEEN;     /* Enable GPIOE clock        */

  GPIOE->MODER    &= ~(	(3UL <<  2* 3) |
												(3UL <<  2* 4) |
												(3UL <<  2* 5) |
												(3UL <<  2* 6) |
												(3UL <<  2* 7) );   /* PE. 4..7 is output       */
	
	GPIOE->MODER    |=  (	(1UL <<  2* 3) |
												(1UL <<  2* 4) |
												(1UL <<  2* 5) |
												(1UL <<  2* 6) |
												(1UL <<  2* 7) );  
	
	GPIOE->OSPEEDR  &= ~(	(3UL <<  2* 3) |
												(3UL <<  2* 4) |
												(3UL <<  2* 5) |
												(3UL <<  2* 6) |
												(3UL <<  2* 7) );   /* PE.3..8 50MHz Fast Speed */
	
	GPIOE->OSPEEDR  |=  (	(2UL <<  2* 3) |
												(2UL <<  2* 4) |
												(2UL <<  2* 5) |
												(2UL <<  2* 6) |
												(2UL <<  2* 7) ); 
	
	GPIOE->PUPDR    &= ~( (3UL <<  2* 3) |
												(3UL <<  2* 4) |
												(3UL <<  2* 5) |
												(3UL <<  2* 6) |
												(3UL <<  2* 7) );   /* PE.3..8 is Pull up       */
												
	GPIOE->PUPDR    |=  ( (1UL <<  2* 3) |
												(1UL <<  2* 4) |
												(1UL <<  2* 5) |
												(1UL <<  2* 6) |
												(1UL <<  2* 7) );


  GPIOE->OTYPER   &= ~( (1UL <<  2* 3) |
												(1UL <<  2* 4) |
												(1UL <<  2* 5) |
												(1UL <<  2* 6) |
												(1UL <<  2* 7) );   /* PE 3 is output Push-Pull  */
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
