#ifndef DELAY_H
#define DELAY_H
#include "stm32f0xx.h"

class DELAY {
public:
	static void INIT();
	static void MS(uint16_t value);
	inline static void US(uint_fast16_t value) {
		SysTick->VAL = 0;
		SysTick->LOAD = (SystemCoreClock / 1000000 /*-1 must be here. But "minus" operation is so long...*/) * value;    // 1MHz Frequency
		while(!(SysTick->CTRL & 0x10000)) __NOP();
	}
	static void TimeOut(uint16_t value);
	static int GetTick();
};



#endif
