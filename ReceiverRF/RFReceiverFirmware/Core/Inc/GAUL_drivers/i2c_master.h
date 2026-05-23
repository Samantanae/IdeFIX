/*
 * ic2_master.c
 *
 *  Created on: 21 May 2026
 *      Author: Samuel
 */

#ifndef INC_GAUL_DRIVERS_I2C_MASTER_H_
#define INC_GAUL_DRIVERS_I2C_MASTER_H_
#include "main.h"
void I2C_main_loop(const float val1, const float val2, const float val3);
extern void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void start_i2c_loop(void);
void i2c_data_dispo(void);
void i2c_data_indispo(void);
void i2c_stop_loop(void);
#endif /* INC_GAUL_DRIVERS_I2C_MASTER_H_ */
