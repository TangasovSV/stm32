#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stm32f0xx.h>

class InterruptHandler {
public:
	virtual void onInterrupt(uint8_t) = 0;
};

bool registerHandler(int pin_number, InterruptHandler* handler);

#endif
