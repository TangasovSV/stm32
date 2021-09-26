/*
 * datetime.h
 *
 *  Created on: Sep 10, 2021
 *      Author: admin
 */

#ifndef INC_DATETIME_H_
#define INC_DATETIME_H_

#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_i2c.h"



class datetime {
private:
	I2C_HandleTypeDef hi2c1;
	//States State;
	uint8_t EncodeBCD(uint8_t dec);
	void HAL_I2C1_Init(void);
	void HAL_I2C_MspInit(void);

	void Write(uint8_t address, uint8_t *data, uint8_t size);
	void Read(uint8_t address, uint8_t *data, uint8_t size);
	void MemWrite(uint8_t address, uint8_t *data, uint8_t data_size, uint8_t start, uint8_t start_size);
public:
	uint8_t DT[7];
	datetime();
	virtual ~datetime();
	void Now(void);
	void Set(int s, int m, int h, int w, int d, int M, int y);
};

#endif /* INC_DATETIME_H_ */
