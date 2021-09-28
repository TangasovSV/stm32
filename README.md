# stm32
Simple C++ lib for stm32

Now realized pin, interrupts and delay\timeout classes.

Do not use DELAY::MS or DELAY::US inside DELAY::TimeOut!!!!



EXAMPLE Code:
_____________________________________________________________________________________________________


#include "delay.h"
#include "pin.h"

#ifdef __cplusplus
extern "C"
#endif


int main(void) 

{
	// Init pin and SysTick	
	DELAY::INIT();
	PIN *A0 = new PIN("A0", pinSpeed::NORMAL, pinMode::OUT, pinType::PushPull, pinPuPd::NOPULL);

	// TimeOut: Blink 6 times OR 6 seconds
	int i = 0;
	DELAY::TimeOut(6000);
	while (DELAY::GetTick() > 0) {
		A0->set(pinState::LOW);
		for (int i = 0; i < 1000000; i++) __NOP();
		A0->set(pinState::HIGH);
		for (int i = 0; i < 1000000; i++) __NOP();
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
