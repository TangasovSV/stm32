#include "delay.h"

__IO int Sys_Ticks;

void DELAY::INIT() {
	SystemCoreClockUpdate();
	SysTick->LOAD = SystemCoreClock / 1000 - 1;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk; 									// Disable IRQ
	NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
	SysTick->VAL = 0;	
}

void DELAY::MS(uint16_t value) {
	TimeOut(value);
	while(Sys_Ticks > 0) __NOP();               // Do nothing while SysTick_Handler() work with Tick value.
}

void DELAY::TimeOut(uint16_t value) {
	Sys_Ticks = value;
	SysTick->VAL = 0;
	SysTick->LOAD = SystemCoreClock / 1000 - 1;  // 1KHz Frequency
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   // Enable IRQ
}

int DELAY::GetTick() {
	return Sys_Ticks;
}

extern "C" void SysTick_Handler(void) {
	Sys_Ticks--;
	if (Sys_Ticks < 1) SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;    // Disable IRQ
}



//__ASM volatile ("cpsid i" : : : "memory");   // Disable ALL IRQ
//SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;  // Disable SysTick IRQ

//__ASM volatile ("cpsie i" : : : "memory");   // Enable ALL IRQ
//SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   // Enable SysTick IRQ	
