#include <encoder.h>

void ENCODER::onInterrupt(uint8_t num) {
	if (num == A->pin)	onRotate(B->get() ? 1 : -1); // Left or Right
	else				onSwitch(SW->get()? 1 : 0);  // Rise or Fall
}


ENCODER::ENCODER(const char* chA, const char* chB, const char* sw ){
	B = new PIN(chB, pinSpeed::FAST, pinMode::IN, pinType::PushPull, pinPuPd::NOPULL);
	A = new PIN(chA, pinSpeed::FAST, pinMode::IN, pinType::PushPull, pinPuPd::NOPULL);
	A->extiConfig(this);
	if (sw != 0) {
		SW = new PIN(sw, pinSpeed::FAST, pinMode::IN, pinType::PushPull, pinPuPd::NOPULL);
		SW->extiConfig(this);
	}
}

