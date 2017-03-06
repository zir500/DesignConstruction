#ifndef __COMMON_FUNCTIONS_H
#define __COMMON_FUNCTIONS_H

	//initializers 
	void init_GIPOB(void);
	void init_GPIOE(void);
	void init_TIM7(void);

	void SysTick_Handler(void);
	void Delay (uint32_t dlyTicks);
	int DelayForButton (uint32_t dlyTicks, int buttons[], int size);
	uint32_t BTN_Get(void);
	void outputSignalON(unsigned int Switch);
  void outputSignalOFF(unsigned int Switch);
	void waitForRelease(void);
	void selectMode(unsigned int mode);
	void buzzerOn(void);
	void buzzerOFF(void);

#endif //__COMMON_FUNCTIONS_H
