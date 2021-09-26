#ifndef PIN_H
#define PIN_H

#include <stm32f0xx.h>
#include "interrupts.h"

#define GPIO_AF_0            ((uint8_t)0x00) /* WKUP, EVENTOUT, TIM15, SPI1, TIM17, MCO, SWDAT, SWCLK, TIM14, BOOT, USART1, 
												CEC, IR_OUT, SPI2, TS, TIM3, USART4, CAN, TIM3, USART2, USART3, CRS, TIM16, TIM1 */
#define GPIO_AF_1            ((uint8_t)0x01) /* USART2, CEC, TIM3, USART1, IR, EVENTOUT, I2C1, I2C2, TIM15, SPI2, USART3, TS, SPI1 */
#define GPIO_AF_2            ((uint8_t)0x02) /* TIM2, TIM1, EVENTOUT, TIM16, TIM17, USB */
#define GPIO_AF_3            ((uint8_t)0x03) /* TS, I2C1, TIM15, EVENTOUT */
#define GPIO_AF_4            ((uint8_t)0x04) /* TIM14, USART4, USART3, CRS, CAN, I2C1 */
#define GPIO_AF_5            ((uint8_t)0x05) /* TIM16, TIM17, TIM15, SPI2, I2C2, MCO, I2C1, USB */
#define GPIO_AF_6            ((uint8_t)0x06) /* EVENTOUT */
#define GPIO_AF_7            ((uint8_t)0x07) /* COMP1 OUT and COMP2 OUT */

typedef enum class pinSpeed {
	SLOW = 0x00,
	NORMAL = 0x01, 
	FAST = 0x03
} pinSpeed;

typedef enum class pinMode {
	IN   = 0x00,
	OUT  = 0x01,
	AF   = 0x02,
	AN   = 0x03 
} pinMode;

typedef enum class pinType {
	PushPull = 0x00,
	OpenDrain = 0x01
} pinType;

typedef enum class pinPuPd {
	NOPULL = 0x00,
	UP     = 0x01,
	DOWN   = 0x02
} pinPuPd;

typedef enum class pinState {
	LOW = 0,
	HIGH
} pinState;

typedef struct {
	GPIO_TypeDef * port;
	volatile uint32_t * rcc_portRegister;
	uint32_t value;
} rcc_struct;

rcc_struct const rcc_lines[] = {                 
	{ GPIOA, &RCC->AHBENR, RCC_AHBENR_GPIOAEN },
	{ GPIOB, &RCC->AHBENR, RCC_AHBENR_GPIOBEN },
	{ GPIOC, &RCC->AHBENR, RCC_AHBENR_GPIOCEN },
	{ GPIOF, &RCC->AHBENR, RCC_AHBENR_GPIOFEN }		
};

class PIN:InterruptHandler {
private:
	GPIO_TypeDef* port;
	void (*callbackFunc)(PIN*);
	static int setPinNum(const char* portPin);
	void extiConfig();
	void onInterrupt(uint8_t num);
	void writeREGISTERS(pinSpeed speed, pinMode mode, pinType type, pinPuPd pull);
public:
	const uint8_t pin;

	// Конструкторы
	PIN(GPIO_TypeDef* GPIOx,  int pin, pinSpeed speed, pinMode mode, pinType type, pinPuPd pull, pinState state = pinState::LOW);
	PIN(const char * portPin, pinSpeed speed, pinMode mode, pinType type, pinPuPd pull, pinState state  = pinState::LOW);

	// Простешие методы установки\проверки состояния пинов.
	void set(pinState state);
	void set(uint8_t state);
	uint8_t get(void);

	// Установка льтернативной функции пина. Типа USART, SPI и т.д.
	void AFConfig(uint8_t GPIO_AF);
	
	// Все что связано с прерываниями. 
	void extiConfig( void(*func)(PIN*) );
	void extiConfig( InterruptHandler *ih );
};


#endif