#ifndef ENCODER_H
#define ENCODER_H

#include <stm32l0xx.h>
#include "pin.h"


class ENCODER :InterruptHandler {
public:
	ENCODER(const char* chA, const char* chB, const char* sw = 0);
	void (*onRotate)(int8_t);
	void (*onSwitch)(int8_t);
private:
	PIN *A;
	PIN *B;
	PIN *SW;
	void onInterrupt(uint8_t num);
};
#endif


