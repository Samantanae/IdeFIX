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

#define RxSIZE 16
#define I2C_NREGISTRE 10
uint16_t rxcount = 0;	/** le numéro du bytes où la trensmision est rendu. permet de trensmettre un nombre indéterminer de donné (temps que cela ne dépasse pas la taille du bufer)*/
uint8_t RxData[RxSIZE] = {0};
uint8_t I2C_REGISTERS[I2C_NREGISTRE] = {0,0,0,0,0,0,0,0,0,0};
//------------------------------------------------------------
// interupt trigger when the master tries to comm. with the slave
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c){
	HAL_I2C_EnableListen_IT(hi2c);
}

// interupt trigger when the device adr. send by master == adr. of the slave.
void HAL_I2C_AddrCallback(I2C_HandleTypeDef* hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode){
	if(TransferDirection == I2C_DIRECTION_TRANSMIT){    // if master which to transmit data.
			rxcount = 0;
			HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData+rxcount, 1, I2C_FIRST_FRAME);
	}
	else{ // master request data (not superted for now)
		Error_Handler();
	}
}

// callback call after the completion of the reception.
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	rxcount++;
	if (rxcount < RxSIZE)
	{
		if (rxcount == RxSIZE-1)
		{
			HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData+rxcount, 1, I2C_LAST_FRAME);
		}
		else
		{
			HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData+rxcount, 1, I2C_NEXT_FRAME);
		}
	}

	if (rxcount == RxSIZE)
	{
		process_data();
	}

}

// interupt triger after an error during reception
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
	// if the master stop sending before the number of bytes is receved. c'est géré ici.
	uint32_t errorcode = HAL_I2C_GetError(hi2c);
	if(errorcode == 4)    // AF error
	{
		process_data();
	}
	HAL_I2C_EnableListen_IT(hi2c); // for now, juste reactivate the reception.
}


/**do somting with the data*/
void process_data (void)
{
	/*
	 * First bit is the adress
	 * */
	int8_t startREG = RxData[0];
	int8_t numREG = rxcount-1;
	int8_t endREG = startREG + numREG -1;
	if (endREG>I2C_NREGISTRE)
	{
		Error_Handler();
	}

	int8_t indx = 1;
	for (int i=0; i<numREG; i++)
	{
		I2C_REGISTERS[startREG++] = RxData[indx++];
	}
}

