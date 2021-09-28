#ifndef DELAY_H
#define DELAY_H
#include "stm32f0xx.h"

class DELAY {
public:
	static void INIT();
	static void MS(uint16_t value);
	static void US(uint16_t value);	
	static void TimeOut(uint16_t value);
	static int GetTick();
};



#endif
