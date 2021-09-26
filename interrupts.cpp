#include "interrupts.h"


InterruptHandler* handlers[16];


bool registerHandler(int pin_number, InterruptHandler* handler) {
	if (handlers[pin_number] != 0) return false; // обработчик уже установлен
	handlers[pin_number] = handler;
	return true;
}


extern "C" void EXTI0_1_IRQHandler() {
	for (int i = 0; i < 2; i++) if (EXTI->PR & 1 << (i)) { EXTI->PR |= 1 << (i);   if (handlers[i] != 0) handlers[i]->onInterrupt(i); }
}


extern "C" void EXTI2_3_IRQHandler() {
	for (int i = 2; i < 4; i++) if (EXTI->PR & 1 << (i)) { EXTI->PR |= 1 << (i);   if (handlers[i] != 0) handlers[i]->onInterrupt(i); }
}


extern "C" void EXTI4_15_IRQHandler() {
	for (int i = 4; i < 16; i++) if (EXTI->PR & 1 << (i)) { EXTI->PR |= 1 << (i);   if (handlers[i] != 0) handlers[i]->onInterrupt(i); }
}
