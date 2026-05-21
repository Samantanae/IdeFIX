/*
 * I2C_Slave.h
 *
 *  Created on: 18 May 2026
 *      Author: Samuel
 */

#ifndef INC_GAUL_DRIVERS_I2C_SLAVE_H_
#define INC_GAUL_DRIVERS_I2C_SLAVE_H_
#include "stm32f1xx_hal_i2c.h"
#include <stdint.h>
extern uint8_t RxData[6];

extern void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c);
extern void HAL_I2C_AddrCallback(I2C_HandleTypeDef* hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);
extern void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef* hi2c);

#endif /* INC_GAUL_DRIVERS_I2C_SLAVE_H_ */
