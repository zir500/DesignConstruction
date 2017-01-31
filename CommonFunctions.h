#ifndef __COMMON_FUNCTIONS_H
#define __COMMON_FUNCTIONS_H

	void SysTick_Handler(void);
	void Delay (uint32_t dlyTicks);
	int DelayForButton (uint32_t dlyTicks);
	uint32_t BTN_Get(void);

#endif //__COMMON_FUNCTIONS_H
