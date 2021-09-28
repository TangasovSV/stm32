# stm32
Simple C++ lib for stm32

Now realized pin, interrupts and delay\timeout classes.

Do not use DELAY::MS or DELAY::US inside DELAY::TimeOut!!!!


EXAMPLE Code:

#include "delay.h"
#include "pin.h"

#ifdef __cplusplus
extern "C"
#endif


int main(void) {
	DELAY::INIT();
	PIN *A0 = new PIN("A0", pinSpeed::NORMAL, pinMode::OUT, pinType::PushPull, pinPuPd::NOPULL);

	DELAY::TimeOut(6000);
	while (DELAY::GetTick() > 0) {
		A0->set(pinState::LOW);
		for (int i = 0; i < 1000000; i++) __NOP();
		A0->set(pinState::HIGH);
		for (int i = 0; i < 1000000; i++) __NOP();
	}
	DELAY::MS(5000);

	
	while (1) {
		A0->set(pinState::HIGH);
		DELAY::US(6);
		A0->set(pinState::LOW);
		DELAY::MS(1000);
	}
}
