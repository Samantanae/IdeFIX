/*
 * RFM22.c
 *
 *  Created on: Apr 15, 2025
 *      Author: victo
 */

#include "gaul/drivers/RFM22.h"
#include <math.h>

// config GFSK, 2.4kbs, 20kHz freq dev, 50kHz channel step, CRC off, no header, carrier @ 433MHz
RFM22_configs rfm22_confs ={
		.registers = 		 {0x1C, 0x1D, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x2A, 0x2C, 0x2D, 0x2E, 0x30, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x75, 0x76, 0x77, 0x7A},
		.register_settings = {0x27, 0x40, 0xA1, 0x20, 0x4E, 0xA5, 0x00, 0x28, 0x1D, 0x2A, 0x08, 0x2A, 0x88, 0x00, 0x02, 0x08, 0x22, 0x2D, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x13, 0xA9, 0x2C, 0x23, 0x10, 0x53, 0x4B, 0x00, 0x05}
};


uint8_t RFM22_init(RFM22 *dev, RFM22_configs *confs)
{
	//reset RFM
	HAL_GPIO_WritePin(dev->snd_port, dev->snd_pin, 1);
	HAL_Delay(100);
	HAL_GPIO_WritePin(dev->snd_port, dev->snd_pin, 0);
	HAL_Delay(100);

	//première lecture port SPI
	uint8_t rx_buffer[] = {0, 0};
	RFM22_SPI_read(dev, RH_RF22_REG_00_DEVICE_TYPE, rx_buffer, 1);
	if (rx_buffer[0] != 8) return 0; //connection non établie
	//wait for chip ready

	do {
		RFM22_SPI_read(dev, RH_RF22_REG_04_INTERRUPT_STATUS2, rx_buffer, 1);
	}
	while (!((rx_buffer[0] & RH_RF22_ICHIPRDY) >> 1)); //check for ichiprdy


	uint8_t tx_buffer[4] = {0};
	// lire registres interrupt
	RFM22_SPI_read(dev, RH_RF22_REG_03_INTERRUPT_STATUS1, rx_buffer, 2);
	// désactiver GPIO
	RFM22_SPI_write(dev, RH_RF22_REG_0B_GPIO_CONFIGURATION0, tx_buffer, 4);
	// mettre en mode standby
	RFM22_SPI_write(dev, RH_RF22_REG_07_OPERATING_MODE1, tx_buffer, 2);
	// active toutes interruptions
	tx_buffer[0] = 0xFF;
	tx_buffer[1] = 0xFF;
	RFM22_SPI_write(dev, RH_RF22_REG_05_INTERRUPT_ENABLE1, tx_buffer, 2);

	//set thresholds registres
	tx_buffer[0] = 56; // à un packet d'overflow
	tx_buffer[1] = 0;
	tx_buffer[2] = 56;
	RFM22_SPI_write(dev, RH_RF22_REG_7C_TX_FIFO_CONTROL1, tx_buffer, 3);

	// écrit toutes les valeurs dans les registres
	for (int i=0; i<sizeof(confs->registers); i++)
	{
		tx_buffer[0] = (confs->register_settings)[i];
		RFM22_SPI_write(dev, (confs->registers)[i], tx_buffer, 1);
	}


	return 1;
}


//transmet les bits dans tx_buffer et retourne en mode standby
uint8_t RFM22_transmit(RFM22 *dev, uint8_t *tx_buffer, uint8_t lenght)
{
	if (lenght > 64)
	{
		return 0; // pas assez de place dans FIFO
	}
	uint8_t ptr[] = {0};
	RFM22_SPI_read(dev, RH_RF22_REG_07_OPERATING_MODE1, ptr, 1);
	if (ptr[0] & RH_RF22_TXON)
	{
		return 0; // already transmitting
	}


	// send dans la FIFO et set longueur packet
	RFM22_SPI_write(dev, RH_RF22_REG_7F_FIFO_ACCESS, tx_buffer, lenght);
	ptr[0] = lenght;
	RFM22_SPI_write(dev, RH_RF22_REG_3E_PACKET_LENGTH, ptr, 1);

	// mode tx
	ptr[0] = RH_RF22_TXON;
	RFM22_SPI_write(dev, RH_RF22_REG_07_OPERATING_MODE1, ptr, 1);

	return 1;
}

// commence à écouter pour des packets. Retourne en mode standy une fois qu'un packet est reçu
uint8_t RFM22_rx_mode(RFM22 *dev)
{
	// enable ipvalid interrupt
	// mode rx
	uint8_t reg_value = RH_RF22_RXON;
	uint8_t *reg_set = &reg_value;
	RFM22_SPI_write(dev, RH_RF22_REG_07_OPERATING_MODE1, reg_set, 1);
	return 1;
}


uint8_t RFM22_standby(RFM22 *dev)
{

	uint8_t reg_value = 0;
	uint8_t *reg_set = &reg_value;
	RFM22_SPI_write(dev, RH_RF22_REG_07_OPERATING_MODE1, reg_set, 1);
	return 1;
}


// retourn nbr d'octets disponnibles
uint8_t RFM22_available(RFM22 *dev)
{
	uint8_t lenght;
	uint8_t *ptr = &lenght;
	RFM22_SPI_read(dev, RH_RF22_REG_4B_RECEIVED_PACKET_LENGTH, ptr, 1);
	return lenght;
}


uint8_t RFM22_get_RSSI(RFM22 *dev)
{
	uint8_t rssi;
	uint8_t *ptr = &rssi;
	RFM22_SPI_read(dev, RH_RF22_REG_26_RSSI, ptr, 1);
	return rssi;
}


void RFM22_clr_tx_FIFO(RFM22 *dev)
{
	uint8_t reg_value = RH_RF22_FFCLRTX;
	uint8_t *reg_set = &reg_value;
	RFM22_SPI_write(dev, RH_RF22_REG_08_OPERATING_MODE2, reg_set, 1);
}


void RFM22_clr_rx_FIFO(RFM22 *dev)
{
	uint8_t reg_value = RH_RF22_FFCLRRX;
	uint8_t *reg_set = &reg_value;
	RFM22_SPI_write(dev, RH_RF22_REG_08_OPERATING_MODE2, reg_set, 1);
}


void RFM22_read_rx(RFM22 *dev, uint8_t *rx_data, uint8_t size)
{
	RFM22_SPI_read(dev, RH_RF22_REG_7F_FIFO_ACCESS, rx_data, size);
}


void RFM22_channel(RFM22 *dev, uint8_t channel)
{
	RFM22_SPI_write(dev, RH_RF22_REG_79_FREQUENCY_HOPPING_CHANNEL_SELECT, &channel, 1);
}


void RFM22_SPI_write(RFM22 *dev, uint8_t addr, uint8_t *tx_buffer, uint8_t size)
{
	uint8_t write_addr = 0x80 | addr;
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, 0);
	HAL_SPI_Transmit(dev->SPIx, &write_addr, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->SPIx, tx_buffer, size, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, 1);
}


void RFM22_SPI_read(RFM22 *dev, uint8_t addr, uint8_t *rx_buffer, uint8_t size)
{
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, 0);
	HAL_SPI_TransmitReceive(dev->SPIx, &addr, rx_buffer, 1, HAL_MAX_DELAY);
	HAL_SPI_TransmitReceive(dev->SPIx, rx_buffer, rx_buffer, size, HAL_MAX_DELAY); //vérifier qu'envoyer le rx buffer ok
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, 1);
}


float RFM22_get_frequency(RFM22 *dev)
{
	uint8_t ptr[5];
	RFM22_SPI_read(dev, RH_RF22_REG_75_FREQUENCY_BAND_SELECT, ptr, 5);
	uint8_t hbsel = (RH_RF22_HBSEL & ptr[0]) > 0;
	uint8_t fb = (0b11111 & ptr[0]);
	uint16_t fc = (((uint16_t)ptr[1]) << 8) | ptr[2];
	uint8_t channel = ptr[3];
	uint8_t fhs = ptr[4];

	float carrier = 10e6 * (hbsel+1) * ((float)fb + 24.0 + ((float)fc)/64000.0);
	float channel_dev = 10e3 * (float)fhs * (float)channel;
	return carrier + channel_dev;
}
