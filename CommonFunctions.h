#ifndef __COMMON_FUNCTIONS_H
#define __COMMON_FUNCTIONS_H

	//initializers 
	void init_GIPOB(void);
	void init_GPIOE(void);
	void init_TIM7(void);
	void init_GIPOC(void);

	void SysTick_Handler(void);
	void Delay (uint32_t dlyTicks);
	int DelayForButton (uint32_t dlyTicks, int buttons[], int size);
	void outputSignalON(unsigned int gpio_no);
  void outputSignalOFF(unsigned int gpio_no);
	void waitForRelease(void);
	void selectMode(unsigned int mode, unsigned int range);
	void buzzerOn(int miliseconds);
	void buzzerOFF(void);
	
	void GPIOB_SignalOFF(unsigned int gpio_no);
	void GPIOB_SignalOn(unsigned int gpio_no);
	void GPIOC_SignalON(unsigned int gpio_no);
	void GPIOC_SignalOFF(unsigned int gpio_no);
	void GPIOE_SignalON(unsigned int gpio_no);
	void GPIOF_SignalOFF(unsigned int gpio_no);


#endif //__COMMON_FUNCTIONS_H
