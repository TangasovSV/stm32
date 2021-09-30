#ifndef DELAY_H
#define DELAY_H
#include "stm32f0xx.h"

class DELAY_SYS {
public:
	static void INIT();
	static void MS(uint16_t value);
	__attribute__((always_inline)) void US(uint_fast16_t value) {
		SysTick->VAL = 0;
		SysTick->LOAD = (SystemCoreClock / 1000000 /*-1 must be here. But "minus" operation is so long...*/) * value;    // 1MHz Frequency
		while(!(SysTick->CTRL & 0x10000)) __NOP();
	}
	static void TimeOut(uint16_t value);
	static int GetTick();
};


typedef struct {
	TIM_TypeDef *tim;
	__IO uint32_t *REG;
	uint32_t value;
} tim_rcc_struct;

tim_rcc_struct const tim_rcc_lines[] = {                 
	{ TIM1,  &RCC->APB2ENR, RCC_APB2ENR_TIM1EN },
	{ TIM2,  &RCC->APB1ENR, RCC_APB1ENR_TIM2EN },
	{ TIM3,  &RCC->APB1ENR, RCC_APB1ENR_TIM3EN },
	{ TIM14, &RCC->APB1ENR, RCC_APB1ENR_TIM14EN },
	{ TIM16, &RCC->APB2ENR, RCC_APB2ENR_TIM16EN },
	{ TIM17, &RCC->APB2ENR, RCC_APB2ENR_TIM17EN },
};


class DELAY_TIM {
private:
	TIM_TypeDef *t;
public:
	DELAY_TIM(TIM_TypeDef *tim);
	__attribute__((always_inline)) void US(uint16_t value) {
		t->CR1  |=  TIM_CR1_CEN;	
		while (t->CNT < value) __NOP();
	}
};




#endif
