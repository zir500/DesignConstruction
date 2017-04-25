#include "STM32F4xx.h"
#include "CommonFunctions.h"
#include "SWT.h"
#include "menu.h"

volatile uint32_t msTicks;  


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
void waitForRelease() {
			while (SWT_Get()){};
}

/*----------------------------------------------------------------------------
   init GIPOB
 *----------------------------------------------------------------------------*/
void init_GIPOB(){
	
  RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOBEN;         /* Enable GPIOB clock                */

  GPIOB->MODER    &= ~((3UL <<  2* 4) |		//TODO: Check whether there are constants to initialise these registers
                       (3UL <<  2* 5) |
                       (3UL <<  2* 7) |
											 (3UL <<  2* 8) |
											 (3UL <<  2*15) );   /* PB.4,5,7,8, 15 is output               */
  GPIOB->MODER    |=  ((1UL <<  2* 4) |
                       (1UL <<  2* 5) | 
                       (1UL <<  2* 7) |
											 (1UL <<  2* 8) |
											 (1UL <<  2*15) ); //TODO WHY do we init it ??? 
  GPIOB->OTYPER   &= ~((1UL <<    4) |
                       (1UL <<    5) |
                       (1UL <<    7) |
											 (1UL <<    8) |
											 (1UL <<  15)  );   /* PB 4,5,7,8, 15 is output Push-Pull     */
  GPIOB->OSPEEDR  &= ~((3UL << 2* 4) |
                       (3UL << 2* 5) |
                       (3UL << 2* 7) |
											 (3UL << 2* 8) |
											 (3UL << 2*15) );   /* PB.4,5,7,8, 15 is 50MHz Fast Speed     */
  GPIOB->OSPEEDR  |=  ((2UL << 2* 4) |
                       (2UL << 2* 5) | 
                       (2UL << 2* 7) |
											 (2UL << 2* 8) |
											 (2UL << 2*15) ); 
  GPIOB->PUPDR    &= ~((3UL << 2* 4) |
                       (3UL << 2* 5) |
                       (3UL << 2* 7) |
											 (3UL << 2* 8) |
											 (3UL << 2*15) );   /* PB.4,5,7,8,15 is Pull up              */
  GPIOB->PUPDR    |=  ((1UL << 2* 4) |
                       (1UL << 2* 5) | 
                       (1UL << 2* 7) |
											 (1UL << 2* 8) |
											 (1UL << 2*15) ); 
}

/*----------------------------------------------------------------------------
   init GIPOC 
NOTE: pin for ADC1 is init in a separate function
 *----------------------------------------------------------------------------*/
void init_GIPOC(){
	
  RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOCEN;         /* Enable GPIOB clock                */

  GPIOC->MODER    &= ~((3UL <<  2* 5 ) |		
                       (3UL <<  2* 6 ) |
											 (3UL <<  2* 7 ) |
                       (3UL <<  2* 13) );   /* PB. 5,6,7,13 is output               */
  GPIOC->MODER    |=  ((1UL <<  2* 5 ) | 
                       (1UL <<  2* 6 ) |
											 (1UL <<  2* 7 ) |
											 (1UL <<  2* 13) ); 
  GPIOC->OTYPER   &= ~((1UL <<    5 ) |
                       (1UL <<    6 ) |
											 (1UL <<    7 ) |
                       (1UL <<    13) );   /* PB 5,6,7,13 is output Push-Pull     */
  GPIOC->OSPEEDR  &= ~((3UL << 2* 5 ) |
                       (3UL << 2* 6 ) |
											 (3UL << 2* 7 ) |
                       (3UL << 2* 13) );   /* PB. 5,6,7, 13 is 50MHz Fast Speed     */
  GPIOC->OSPEEDR  |=  ((2UL << 2* 5 ) |
                       (2UL << 2* 6 ) | 
											 (2UL << 2* 7 ) |
                       (2UL << 2* 13) ); 
  GPIOC->PUPDR    &= ~((3UL << 2* 5 ) |
                       (3UL << 2* 6 ) |
 											 (3UL << 2* 7 ) |
                       (3UL << 2* 13) );   /* PB.5,6,7, 13 is Pull up              */
  GPIOC->PUPDR    |=  ((1UL << 2* 5 ) |
                       (1UL << 2* 6 ) | 
											 (1UL << 2* 7 ) |											 
                       (1UL << 2* 13) ); 
}

/*----------------------------------------------------------------------------
   init GIPOC 
NOTE: pins for buttons are on their own init
 *----------------------------------------------------------------------------*/
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



/*----------------------------------------------------------------------------
   turns a GPIOB on
 *----------------------------------------------------------------------------*/
void GPIOB_SignalON(unsigned int gpio_no) {
	GPIOB->BSRR = (1U << gpio_no); 
}

/*----------------------------------------------------------------------------
   turns a GPIOB off
 *----------------------------------------------------------------------------*/
void GPIOB_SignalOFF(unsigned int gpio_no) {
		GPIOB->BSRR = (1U << gpio_no) << 16; 
}

/*----------------------------------------------------------------------------
   turns a GPIOC on
 *----------------------------------------------------------------------------*/
void GPIOC_SignalON(unsigned int gpio_no) {
	GPIOC->BSRR = (1U << gpio_no); 
}

/*----------------------------------------------------------------------------
   turns a GPIOC off
 *----------------------------------------------------------------------------*/
void GPIOC_SignalOFF(unsigned int gpio_no) {
	GPIOC->BSRR = (1U << gpio_no) << 16; 
}


/*----------------------------------------------------------------------------
   turns a GPIOE on
 *----------------------------------------------------------------------------*/
void GPIOE_SignalON(unsigned int gpio_no) {
	GPIOE->BSRR = (1U << gpio_no); 
}

/*----------------------------------------------------------------------------
   turns a GPIOE off
 *----------------------------------------------------------------------------*/
void GPIOE_SignalOFF(unsigned int gpio_no) {
	GPIOE->BSRR = (1U << gpio_no) << 16; 
}


void selectMode(unsigned int mode, unsigned int range) {

	if ( (mode == 0xF) && (range == 0xF) ) { //default mode, enable = 0, all control signals 0
		
		//mode signal
		GPIOC_SignalOFF(5);
		GPIOC_SignalOFF(6);
		GPIOC_SignalOFF(13);
		
		//range signals
		GPIOE_SignalOFF(5);
		GPIOE_SignalOFF(6);
		GPIOE_SignalOFF(7);
		
		//enable signal
		GPIOB_SignalOFF(15);
	} else {
		//outputSignalON(0xF ); //turn on the enable signal
		
		//turn on the desired control signals
		
		//mode signal
		GPIOC -> ODR |= (mode & 1)<<5;
		GPIOC -> ODR |= ((mode>>1) & 1)<<6;
		GPIOC -> ODR |= ((mode>>2) & 1)<<13;

		//range signals
		GPIOE -> ODR |= (range & 1)<<5;
		GPIOE -> ODR |= ((range>>1) & 1)<<6;
		GPIOE -> ODR |= ((range>>2) & 1)<<7;

		//GPIOE->ODR |= mode << 3;
		
		//enable signal
	  GPIOE_SignalON(3);
	}
}



//turn on the buzzer for a specified number of milli-seconds 
//max value for the timer is 6500 ms (=6.5s)
void buzzerOn(int miliseconds) { 
	GPIOE->BSRR |= 1U << 4;
	
	//convert miliseconds 
	//TIM7->ARR is 16 bit register, so we have to avoid overflowing
	if (miliseconds < 6500) {
		TIM7->ARR = miliseconds * 10;
	}
	
	TIM7->DIER &= ~TIM_DIER_UIE;
	TIM7->EGR |= TIM_EGR_UG;
	TIM7->SR = 0;
	TIM7->DIER |= TIM_DIER_UIE; 					/*Enable interrupts */
	TIM7->CR1 |= TIM_CR1_CEN;  //stop timer
}

void buzzerOFF() {
	
	GPIOE->BSRR |= (1U <<4) << 16;					/*Enable interrupts */
	TIM7->CR1 &= ~TIM_CR1_CEN;  //stop timer
}

//init TIM7
void init_TIM7() {
	
 	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; 	/*Enable clock*/
 	TIM7->PSC = (SystemCoreClock/2)/10000; 											
	TIM7->ARR = 30000;  //set to 3 seconds
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
