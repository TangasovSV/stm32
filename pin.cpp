#include "pin.h"

PIN::PIN(GPIO_TypeDef* GPIOx, int pinNUM, pinSpeed speed, pinMode mode, pinType type, pinPuPd pull, pinState state):pin(pinNUM) {
	this->port = GPIOx;
	//this->pin = pin;
	int index = 0;
	while (port != rcc_lines[index].port) index++;
	* rcc_lines[index].rcc_portRegister |= rcc_lines[index].value;	
	set(state);
	writeREGISTERS(speed, mode, type, pull);
}


PIN::PIN(const char * portPin, pinSpeed speed, pinMode mode, pinType type, pinPuPd pull, pinState state):pin(setPinNum(portPin)) {
	int index = portPin[0] - 65;		
	this->port = rcc_lines[index].port;	
	
	* rcc_lines[index].rcc_portRegister |= rcc_lines[index].value;
	set(state);
	writeREGISTERS(speed, mode, type, pull);
}


int PIN::setPinNum(const char* portPin) {
	int pos = 1, num = 0;
	while (portPin[pos]) num = num * 10 + portPin[pos++] - 48;
	return num;
}


void PIN::writeREGISTERS(pinSpeed speed, pinMode mode, pinType type, pinPuPd pull)
{
	(this->port)->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR0 << (this->pin * 2));
	(this->port)->OSPEEDR |= ((uint32_t)(speed) << (this->pin * 2));
	
	(this->port)->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)this->pin));
	(this->port)->OTYPER |= (uint16_t)(((uint16_t)type) << ((uint16_t)this->pin));
	
	(this->port)->MODER  &= ~(GPIO_MODER_MODER0 << (this->pin * 2));
	(this->port)->MODER |= (((uint32_t)mode) << (this->pin * 2));	
	
	(this->port)->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)this->pin * 2));
	(this->port)->PUPDR |= (((uint32_t)pull) << (this->pin * 2));
}

void PIN::extiConfig() {
	__ASM volatile ("cpsie i" : : : "memory");
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	switch (pin) {
	case 0: { NVIC_EnableIRQ(EXTI0_1_IRQn);  NVIC_SetPriority(EXTI0_1_IRQn, 3); break; }
	case 1: { NVIC_EnableIRQ(EXTI0_1_IRQn);  NVIC_SetPriority(EXTI0_1_IRQn, 3); break; }
	case 2: { NVIC_EnableIRQ(EXTI2_3_IRQn);  NVIC_SetPriority(EXTI2_3_IRQn, 3); break; }
	case 3: { NVIC_EnableIRQ(EXTI2_3_IRQn);  NVIC_SetPriority(EXTI2_3_IRQn, 3); break; }
	default: { NVIC_EnableIRQ(EXTI4_15_IRQn); NVIC_SetPriority(EXTI4_15_IRQn, 3); break; }
	}
	EXTI->IMR |= 1 << pin;
	EXTI->RTSR |= 1 << pin;


	int gpio_num = 0;
	if (port == GPIOB) gpio_num = 1;
	if (port == GPIOC) gpio_num = 2;
	if (port == GPIOF) gpio_num = 5;

	int pin_mask = (0x0001 << (pin % 4)) * gpio_num;
	SYSCFG->EXTICR[pin / 4] &= ~((uint16_t)pin_mask);
}

void PIN::extiConfig( void(*func)(PIN*) ) {
	extiConfig();
	callbackFunc = func;
	registerHandler(this->pin, this);
}

// ������������ ���������� ��� �������� ������. �������, 1-Wire � ��.
void PIN::extiConfig(InterruptHandler* ih) {
	extiConfig();
	registerHandler(this->pin, ih);
}

// ������������ ���������� �� ������� �������� ������ ���� ������.
void PIN::onInterrupt(uint8_t num) {
	if (callbackFunc != 0) callbackFunc(this);
}

void PIN::AFConfig(uint8_t GPIO_AF) {
	uint32_t temp = 0x00;

	temp = ((uint32_t)(GPIO_AF) << ((uint32_t)((uint32_t)pin & (uint32_t)0x07) * 4));
	port->AFR[pin >> 0x03] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)pin & (uint32_t)0x07) * 4));
	port->AFR[pin >> 0x03] = port->AFR[pin >> 0x03] | temp;
}

void PIN::set(pinState state) {
	if (state == pinState::LOW)	port->BRR  = 1 << pin;
	else          	            port->BSRR = 1 << pin; 
}

void PIN::set(uint8_t state) {
	if (state == 0)	port->BRR  = 1 << pin;
	else          	port->BSRR = 1 << pin; 
}

uint8_t PIN::get() {
	return (this->port->IDR & (1 << this->pin)) ? 1 : 0;
}




