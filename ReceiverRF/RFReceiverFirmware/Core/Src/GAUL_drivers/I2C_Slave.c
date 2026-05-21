/*
 * I2C_Slave.c
 *
 *  Created on: 18 May 2026
 *      Author: Samuel
 */
#include "GAUL_drivers/I2C_Slave.h"
#include "stdio.h"
#include <string.h>
#include "main.h"
int count = 0;
uint8_t RxData[6] = {0};
// interupt trigger when the master tries to comm. with the slave
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c){
	HAL_I2C_EnableListen_IT(hi2c);
}

// interupt trigger when the device adr. send by master == adr. of the slave.
void HAL_I2C_AddrCallback(I2C_HandleTypeDef* hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode){
	if(TransferDirection == I2C_DIRECTION_TRANSMIT){    // if master which to transmit data.
		HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData, 6, I2C_FIRST_AND_LAST_FRAME);    //TODO: change the XferOptions !!!!!!!!!
	}
	else{ // master request data (not superted for now)
		Error_Handler();
	}
}

// callback call after the completion of the reception.
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef* hi2c){
	count++;
}

// interupt triger after an error during reception
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
	HAL_I2C_EnableListen_IT(hi2c); // for now, juste reactivate the reception.
}
