#include <datetime.h>
#include <pin.h>


void datetime::HAL_I2C_MspInit(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_I2C1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	__HAL_RCC_I2C1_CLK_ENABLE();

	HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C1_IRQn);
}

void datetime::HAL_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00200C28;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);
  HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE);
  HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0);
}

datetime::datetime() {
	for (int i = 0; i < 8; i++) DT[i] = 0;
	HAL_I2C_MspInit();
	HAL_I2C1_Init();
}

void datetime::Write(uint8_t address, uint8_t *data, uint8_t size) {
	// Ждем освобождения шины, если она занята.
	while ((hi2c1.Instance->ISR & I2C_FLAG_BUSY) == I2C_FLAG_BUSY);

	// Отправляем сигнал СТАРТ и прочие нужные вещи.
	MODIFY_REG(hi2c1.Instance->CR2, 0x03FF67FF,
	(uint32_t)(((uint32_t)address & I2C_CR2_SADD) | (((uint32_t)size << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_AUTOEND_MODE | (uint32_t)I2C_GENERATE_START_WRITE)
	);

	// Отправляем сообщение.
    while (size > 0){
    	// Ждем завершения отправки каждого байта.
    	while ((hi2c1.Instance->ISR & I2C_FLAG_TXIS) != I2C_FLAG_TXIS);
    	hi2c1.Instance->TXDR = *data;
    	data++;
    	size--;
    }

    // Отправляем сигнал СТОП
	while ((hi2c1.Instance->ISR & I2C_FLAG_STOPF) != I2C_FLAG_STOPF);
    // и тут же подтираем за собой
    hi2c1.Instance->ICR = I2C_FLAG_STOPF;

    // Очищаем регистр CR2
    hi2c1.Instance->CR2 &= (uint32_t)~(0x01FF14FF);
}

void datetime::Read(uint8_t address, uint8_t *data, uint8_t size){
	// Ждем освобождения шины, если она занята.
	while ((hi2c1.Instance->ISR & I2C_FLAG_BUSY) == I2C_FLAG_BUSY);

	// Отправляем сигнал СТАРТ и прочие нужные вещи.
	MODIFY_REG(hi2c1.Instance->CR2, 0x03FF67FF,
	(uint32_t)(((uint32_t)address & I2C_CR2_SADD) | (((uint32_t)size << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_AUTOEND_MODE | (uint32_t)I2C_GENERATE_START_READ)
	);

	while (size > 0){
		while ((hi2c1.Instance->ISR & I2C_FLAG_RXNE) != I2C_FLAG_RXNE);
		*data = (uint8_t)hi2c1.Instance->RXDR;
		data++;
		size--;
	}

    // Отправляем сигнал СТОП
	while ((hi2c1.Instance->ISR & I2C_FLAG_STOPF) != I2C_FLAG_STOPF);
    // и тут же подтираем за собой
    hi2c1.Instance->ICR = I2C_FLAG_STOPF;

    // Очищаем регистр CR2
    hi2c1.Instance->CR2 &= (uint32_t)~(0x01FF14FF);
}

void datetime::MemWrite(uint8_t address, uint8_t *data, uint8_t data_size, uint8_t start, uint8_t start_size){
	// Ждем освобождения шины, если она занята.
	while ((hi2c1.Instance->ISR & I2C_FLAG_BUSY) == I2C_FLAG_BUSY);

	// Отправляем сигнал СТАРТ и прочие нужные вещи.
	MODIFY_REG(hi2c1.Instance->CR2, 0x03FF67FF,
	(uint32_t)(((uint32_t)address & I2C_CR2_SADD) | (((uint32_t)start_size << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_RELOAD_MODE | (uint32_t)I2C_GENERATE_START_WRITE)
	);

	while ((hi2c1.Instance->ISR & I2C_FLAG_TXIS) != I2C_FLAG_TXIS);
	hi2c1.Instance->TXDR = ((uint8_t)((uint16_t)((start) & (uint16_t)(0x00FFU))));

	while ((hi2c1.Instance->ISR & I2C_FLAG_TCR) != I2C_FLAG_TCR);




	////////////////////////////////////////////////////////////////////////////////////////////////////




	// Отправляем сигнал СТАРТ и прочие нужные вещи.
	MODIFY_REG(hi2c1.Instance->CR2, 0x03FF67FF,
	(uint32_t)(((uint32_t)address & I2C_CR2_SADD) | (((uint32_t)data_size << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_AUTOEND_MODE | (uint32_t)I2C_NO_STARTSTOP)
	);

	// Отправляем сообщение.
    while (data_size > 0){
    	// Ждем завершения отправки каждого байта.
    	while ((hi2c1.Instance->ISR & I2C_FLAG_TXIS) != I2C_FLAG_TXIS);
    	hi2c1.Instance->TXDR = *data;
    	data++;
    	data_size--;
    }

    // Отправляем сигнал СТОП
	while ((hi2c1.Instance->ISR & I2C_FLAG_STOPF) != I2C_FLAG_STOPF);
    // и тут же подтираем за собой
    hi2c1.Instance->ICR = I2C_FLAG_STOPF;

    // Очищаем регистр CR2
    hi2c1.Instance->CR2 &= (uint32_t)~(0x01FF14FF);
}

void datetime::Now(void){
	uint8_t temp = 0;
	Write(0xD0, &(temp), 1);
	Read(0xD0, DT, 7);

	DT[0] = ((DT[0] & 0xf0) >> 4) * 10 + (DT[0] & 0x0f);
	DT[1] = ((DT[1] & 0xf0) >> 4) * 10 + (DT[1] & 0x0f);
	DT[2] = ((DT[2] & 0xf0) >> 4) * 10 + (DT[2] & 0x0f);
	DT[3] = ((DT[3] & 0xf0) >> 4) * 10 + (DT[3] & 0x0f);
	DT[4] = ((DT[4] & 0xf0) >> 4) * 10 + (DT[4] & 0x0f);
	DT[5] = ((DT[5] & 0xf0) >> 4) * 10 + (DT[5] & 0x0f);
	DT[6] = ((DT[6] & 0xf0) >> 4) * 10 + (DT[6] & 0x0f);
}

void datetime::Set(int s, int m, int h, int w, int d, int M, int y){
		DT[0] = (s % 10 + ((s / 10) << 4));
		DT[1] = (m % 10 + ((m / 10) << 4));
		DT[2] = (h % 10 + ((h / 10) << 4));
		DT[3] = (w % 10 + ((w / 10) << 4));
		DT[4] = (d % 10 + ((d / 10) << 4));
		DT[5] = (M % 10 + ((M / 10) << 4));
		DT[6] = (y % 10 + ((y / 10) << 4));
		MemWrite(0xD0, DT, 7, 0, 1);
		Now();
}

datetime::~datetime() {
	// TODO Auto-generated destructor stub
}

