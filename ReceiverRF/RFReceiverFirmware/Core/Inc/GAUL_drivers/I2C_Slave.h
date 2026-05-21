/*
 * I2C_Slave.h
 *
 *  Created on: 18 May 2026
 *      Author: Samuel
 */

#ifndef INC_GAUL_DRIVERS_I2C_SLAVE_H_
#define INC_GAUL_DRIVERS_I2C_SLAVE_H_
#include "main.h"
#include <stdint.h>

extern void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c);
extern void HAL_I2C_AddrCallback(I2C_HandleTypeDef* hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);
extern void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef* hi2c);

//TODO: vérifier s'il y a vraiment besoins que les fonctions suivantes soient externes. peux-être possibilité de sauvé de la mémoire en les mettent en static.
extern void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
extern void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
extern void HAL_I2C_AddrCallback(I2C_HandleTypeDef* hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);
#endif /* INC_GAUL_DRIVERS_I2C_SLAVE_H_ */
