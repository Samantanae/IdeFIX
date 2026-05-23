/*
 * I2C_Slave.c
 *
 *  Created on: 18 May 2026
 *      Author: Samuel
 */
#include "GAUL_drivers/I2C_Slave.h"
#include "GAUL_drivers/GEO_coordo.h"
#include "stdio.h"
#include <string.h>
#include "main.h"

#define RxSIZE 16					/** Taille du buffer de réception */
#define I2C_NREGISTRE 12			/** Nombre de registre que le master peut écrire. (doit être inférieur à RxSIZE-1)*/
uint8_t RxData[RxSIZE] = {0};		/** Buffer de réception. le master doit envoyer au moins 2 bytes, le premier étant l'adresse du registre à écrire et les suivants les données à écrire dans les registres. */

uint8_t rxcount = 0;	/** le numéro du bytes où la reception est rendu. permet de recevoir un nombre indéterminer de donné (temps que cela ne dépasse pas la taille du bufer)*/
uint8_t txcount = 0;	/** le numéro du bytes où la trensmision est rendu. permet de trensmettre un nombre indéterminer de donné (temps que cela ne dépasse pas la taille du bufer)*/

/* interupt trigger when the master send the stop condition or a new start condition. */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c){
	HAL_I2C_EnableListen_IT(hi2c);
}

// interupt trigger when the device adr. send by master == adr. of the slave.
/**
 * @brief Slave Address Match callback.
 * @param hi2c: I2C handle.
 * @param TransferDirection: Master request transfer direction (Write/Read).
 * @param AddrMatchCode: Address Match Code.
 * @retval None
 */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef* hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode){
	if(TransferDirection == I2C_DIRECTION_TRANSMIT){    // if master which to transmit data.
		rxcount = 0;
		HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData + rxcount, 1, I2C_FIRST_FRAME);
	}
	else{ // master request data (not superted for now)
		Error_Handler();
	}
}

/**
 * @brief Rx Transfer completed callback.
 * @param hi2c: I2C handle.
 * @retval None
 */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	rxcount++;
	if (rxcount < RxSIZE){
		if (rxcount == RxSIZE-1){
			HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData+rxcount, 1, I2C_LAST_FRAME);
		}
		else{
			HAL_I2C_Slave_Seq_Receive_IT(hi2c, RxData+rxcount, 1, I2C_NEXT_FRAME);
		}
	}
	if (rxcount == RxSIZE){
		process_data();
	}
}

/**
 * @brief I2C Error callback.
 * @param hi2c: I2C handle.
 * @retval None
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
	uint32_t errorcode = HAL_I2C_GetError(hi2c);
	if (errorcode == 4)  // AF error
	{
		process_data();
	}
	// reinit the slave to be able to receive new data.
	HAL_I2C_EnableListen_IT(hi2c);
}

void process_data(void){
	/** change the data in the registre(s).*/
	//TODO: verif if int8_t is enough for the number of registre we want to write.
	int8_t startREG = RxData[0];				/**< The starting register address */
	int8_t numREG = rxcount-1;					/**< The number of register to write.*/
	int8_t endREG = startREG + numREG -1;		/**< The ending register address */

	/** S'il y a dépassement du nombre de registres disponibles */
	if (endREG>I2C_NREGISTRE)
	{
		Error_Handler();    /*pas encore implémenté*/
	}

	uint8_t indx = 1;
	for (uint8_t i=0; i<numREG; i++)
	{
		I2C_GPS.I2C_REGISTERS[startREG++] = RxData[indx++];
	}
}

