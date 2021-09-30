#include "delay.h"

__IO int Sys_Ticks;

void DELAY_SYS::INIT() {
	SystemCoreClockUpdate();
	SysTick->LOAD = SystemCoreClock / 1000 - 1;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk; 									// Disable IRQ
	NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
	SysTick->VAL = 0;	
}

void DELAY_SYS::MS(uint16_t value) {
	TimeOut(value);
	while(Sys_Ticks > 0) __NOP();               // Do nothing while SysTick_Handler() work with Tick value.
}

void DELAY_SYS::TimeOut(uint16_t value) {
	Sys_Ticks = value;
	SysTick->VAL = 0;
	SysTick->LOAD = SystemCoreClock / 1000 - 1;  // 1KHz Frequency
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   // Enable IRQ
}

int DELAY_SYS::GetTick() {
	return Sys_Ticks;
}

DELAY_TIM::DELAY_TIM(TIM_TypeDef *tim) {
	t = tim;
	int index = 0;
	while (t != tim_rcc_lines[index].tim) index++;
	* tim_rcc_lines[index].REG |= tim_rcc_lines[index].value;	
	
	t->ARR = 65535;
	t->PSC = 0;						// Prescaler
	t->CR1  &= ~TIM_CR1_ARPE;     	// Disable buffer mode for ARR.	
	t->EGR  |= TIM_EGR_UG;    		// Update registers PSC (and m.b. ARR) from buffer
	//t->SMCR &= ~TIM_SMCR_SMS;
	t->DIER &= ~TIM_DIER_UIE;  		// Disable TIM IRQ
	t->CR1  &= ~TIM_CR1_DIR;      	// UP Counter.  |= TIM_CR1_DIR - DOWN counter.
	t->CR1  |=  TIM_CR1_OPM;   		// One pulse mode	
	t->CR1  &= ~TIM_CR1_CEN;	
}

extern "C" void SysTick_Handler(void) {
	Sys_Ticks--;
	if (Sys_Ticks < 1) SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;    // Disable IRQ
}

