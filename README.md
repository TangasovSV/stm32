# stm32
Simple C++ lib for stm32

Now realized pin, interrupts and delay\timeout classes.

Functions DELAY and TIMEOUT is my pain and suffering.
0. HAL+DELAY_TIM = everything is OK. Very simple.
1. HAL+DELAY_SYS = I can't guarantee perfect work without problem. It can became a problem or It can work fine. Depends on many other factors. This decision is on your conscience.
2. Do not use DELAY_SYS::MS or DELAY_SYS::US inside DELAY_SYS::TimeOut! Newer!!!





EXAMPLE Code:
_____________________________________________________________________________________________________


#include "delay.h"
#include "pin.h"

#ifdef __cplusplus
extern "C"
#endif


int main(void) {

	// Init pin and SysTick	
	
	DELAY::INIT();
	PIN *A0 = new PIN("A0", pinSpeed::NORMAL, pinMode::OUT, pinType::PushPull, pinPuPd::NOPULL);

	// TimeOut: Blink 6 times OR 6 seconds
	int i = 0;
	DELAY::TimeOut(6000);
	while (DELAY::GetTick() > 0) {
		A0->set(pinState::LOW);
		for (int i = 0; i < 1000000; i++) __NOP(); // DO NOT USE DELAY_SYS::MS/US HERE!!!!
		A0->set(pinState::HIGH);
		for (int i = 0; i < 1000000; i++) __NOP(); // DO NOT USE DELAY_SYS::MS/US HERE!!!!
		i++;
		if (i > 5) break;
	}

	// Delay
	while (1) {
		A0->set(pinState::HIGH);
		DELAY::US(6);
		A0->set(pinState::LOW);
		DELAY::MS(1000);
	}
}
