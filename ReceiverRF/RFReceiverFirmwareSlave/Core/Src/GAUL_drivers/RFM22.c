/*
 * RFM22.c
 *
 *  Created on: Apr 15, 2025
 *      Author: victo
 */

#include "Gaul_drivers/RFM22.h"
#include <math.h>

// config GFSK, 2.4kbs, 20kHz freq dev, 50kHz channel step, CRC off, no header, carrier @ 433MHz
RFM22_configs rfm22_confs = {
	.registers = {0x1C, 0x1D, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x2A, 0x2C, 0x2D, 0x2E, 0x30, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x75, 0x76, 0x77, 0x7A},
	.register_settings = {0x27, 0x40, 0xA1, 0x20, 0x4E, 0xA5, 0x00, 0x28, 0x1D, 0x2A, 0x08, 0x2A, 0x88, 0x00, 0x02, 0x08, 0x22, 0x2D, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x13, 0xA9, 0x2C, 0x23, 0x10, 0x53, 0x4B, 0x00, 0x05}};

/**
 * @brief Initializes the RFM22 module with the specified configurations.
 * @param dev Pointer to the RFM22 device structure.
 * @param confs Pointer to the RFM22 configurations structure containing the register settings to be applied during initialization.
 * @return 0 if the initialization was successful, or a non-zero error code if it failed.
 * @note configures the RFM22 module by writing the specified register settings from the provided configuration structure.
 */
uint8_t RFM22_init(RFM22 *dev, RFM22_configs *confs)
{
	// reset RFM
	HAL_GPIO_WritePin(dev->snd_port, dev->snd_pin, 1);
	HAL_Delay(100);
	HAL_GPIO_WritePin(dev->snd_port, dev->snd_pin, 0);
	HAL_Delay(100);

	// première lecture port SPI
	uint8_t rx_buffer[] = {0, 0};
	RFM22_SPI_read(dev, RH_RF22_REG_00_DEVICE_TYPE, rx_buffer, 1);
	if (rx_buffer[0] != 8)
		return 0; // connection non établie
	// wait for chip ready

	do
	{
		RFM22_SPI_read(dev, RH_RF22_REG_04_INTERRUPT_STATUS2, rx_buffer, 1);
	} while (!((rx_buffer[0] & RH_RF22_ICHIPRDY) >> 1)); // check for ichiprdy

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

	// set thresholds registres
	tx_buffer[0] = 56; // à un packet d'overflow
	tx_buffer[1] = 0;
	tx_buffer[2] = 56;
	RFM22_SPI_write(dev, RH_RF22_REG_7C_TX_FIFO_CONTROL1, tx_buffer, 3);

	// écrit toutes les valeurs dans les registres
	for (int i = 0; i < sizeof(confs->registers); i++)
	{
		tx_buffer[0] = (confs->register_settings)[i];
		RFM22_SPI_write(dev, (confs->registers)[i], tx_buffer, 1);
	}

	return 1;
}

/**
 * @brief Transmet les bits dans tx_buffer et retourne en mode standby
 * @param dev Pointer to the RFM22 device structure.
 * @param tx_buffer Pointer to the buffer containing the data to transmit.
 * @param lenght The number of bytes to transmit.
 * @return 0 if the transmission was successful, or a non-zero error code if it failed.
 */
uint8_t RFM22_transmit(RFM22 *dev, const uint8_t *tx_buffer, const uint8_t lenght)
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

/**
 * @brief commence à écouter pour des packets. Retourne en mode standy une fois qu'un packet est reçu
 * @param dev Pointer to the RFM22 device structure.
 * @return 0 if the operation was successful, or a non-zero error code if it failed.
 * @note In RX mode, the RFM22 module is actively listening for incoming signals
 *  and can receive data packets. This function configures the necessary
 *  registers to enable RX mode, allowing the module to capture and process incoming data.
 *  The caller can then use other functions to check for available data
 *  and read it from the RX FIFO.
 */
uint8_t RFM22_rx_mode(RFM22 *dev)
{
	// enable ipvalid interrupt
	// mode rx
	uint8_t reg_value = RH_RF22_RXON;
	uint8_t *reg_set = &reg_value;
	RFM22_SPI_write(dev, RH_RF22_REG_07_OPERATING_MODE1, reg_set, 1);
	return 1;
}

/**
 * @brief Puts the RFM22 module into standby mode.
 * @param dev Pointer to the RFM22 device structure.
 * @return 0 if the operation was successful, or a non-zero error code if it failed.
 * @note In standby mode, the RFM22 module is not actively transmitting or receiving,
 *  but it can quickly transition back to active mode when needed.
 *  This function is typically used to save power when the module is not in use,
 *  while still allowing for a fast wake-up time when communication is required.
 */
uint8_t RFM22_standby(RFM22 *dev)
{

	uint8_t reg_value = 0;
	uint8_t *reg_set = &reg_value;
	RFM22_SPI_write(dev, RH_RF22_REG_07_OPERATING_MODE1, reg_set, 1);
	return 1;
}

/**
 * @brief gets the number of bytes available in the RX FIFO of the RFM22 module.
 * @param dev Pointer to the RFM22 device structure.
 * @return nbr d'octets disponnibles
 * @note This function reads the interrupt status register of the RFM22
 *  module to determine if there is data available in the RX FIFO.
 *  If the IRXFFAFULL bit is set, it indicates that there is at least
 *  one complete packet of data available for reading. The function
 *  returns 1 in this case, allowing the caller to proceed with reading
 *  the data from the RX FIFO. If the RX FIFO is empty, the function returns 0,
 *  indicating that there is no data to read at the moment.
 */
uint8_t RFM22_available(RFM22 *dev)
{
	uint8_t lenght;
	uint8_t *ptr = &lenght;
	RFM22_SPI_read(dev, RH_RF22_REG_4B_RECEIVED_PACKET_LENGTH, ptr, 1);
	return lenght;
}

/**
 * @brief Gets the Received Signal Strength Indicator (RSSI) value from the RFM22 module.
 * @param dev Pointer to the RFM22 device structure.
 * @return The RSSI value as an unsigned 8-bit integer.
 *  The actual RSSI value can be calculated using the formula:
 *  RSSI (dBm) = -RSSI_value / 2,
 *  where RSSI_value is the value read from the RFM22_REG_26_RSSI register.
 * @note The RSSI value indicates the strength of the received signal,
 *  with higher values representing stronger signals.
 *  This function reads the RSSI value from the RFM22 module
 *  and returns it for further processing or display.
 */
uint8_t RFM22_get_RSSI(RFM22 *dev)
{
	uint8_t rssi;
	uint8_t *ptr = &rssi;
	RFM22_SPI_read(dev, RH_RF22_REG_26_RSSI, ptr, 1);
	return rssi;
}

/**
 * @brief Clears the TX FIFO of the RFM22 module.
 * @param dev Pointer to the RFM22 device structure.
 * @note This function is typically used to discard any data that may be present in the TX FIFO, ensuring that the next transmission starts with an empty buffer.
 */
void RFM22_clr_tx_FIFO(RFM22 *dev)
{
	uint8_t reg_value = RH_RF22_FFCLRTX;
	uint8_t *reg_set = &reg_value;
	RFM22_SPI_write(dev, RH_RF22_REG_08_OPERATING_MODE2, reg_set, 1);
}

/**
 * @brief Clears the RX FIFO of the RFM22 module.
 * @param dev Pointer to the RFM22 device structure.
 * @note This function is typically used to discard any received data that may be present in the RX FIFO, ensuring that the next reception starts with an empty buffer.
 */
void RFM22_clr_rx_FIFO(RFM22 *dev)
{
	uint8_t reg_value = RH_RF22_FFCLRRX;
	uint8_t *reg_set = &reg_value;
	RFM22_SPI_write(dev, RH_RF22_REG_08_OPERATING_MODE2, reg_set, 1);
}

/**
 * @brief Reads received data from the RFM22 module's RX FIFO.
 * @param dev Pointer to the RFM22 device structure.
 * @param rx_data Pointer to the buffer where the received data will be stored.
 * @param size The number of bytes to read from the RX FIFO.
 * @note Reads data from the RFM22 module's RX FIFO and stores it in the provided buffer. The size parameter specifies how many bytes to read, which should not exceed the maximum packet length configured in the RFM22 module. The function assumes that there is data available in the RX FIFO (as indicated by the RFM22_available function) before calling this function to read the data.
 */
void RFM22_read_rx(RFM22 *dev, uint8_t *rx_data, uint8_t size)
{
	RFM22_SPI_read(dev, RH_RF22_REG_7F_FIFO_ACCESS, rx_data, size);
}

/**
 * @brief Sets the channel of the RFM22 module.
 * @param dev Pointer to the RFM22 device structure.
 * @param channel The desired channel.
 * @note The actual frequency corresponding to the channel will depend on the frequency band and channel spacing configured in the RFM22 module.
 */
void RFM22_channel(RFM22 *dev, uint8_t channel)
{
	RFM22_SPI_write(dev, RH_RF22_REG_79_FREQUENCY_HOPPING_CHANNEL_SELECT, &channel, 1);
}

/**
 * @brief Writes data to the RFM22 module via SPI.
 * @param dev Pointer to the RFM22 device structure.
 * @param addr The address of the register to write to.
 * @param tx_buffer Pointer to the buffer containing the data to write.
 * @param size The number of bytes to write.
 * @example
 * uint8_t config_value = 0x1A;
 * RFM22_SPI_write(&rfm22_dev, RH_RF22_REG_0B_GPIO_CONFIGURATION0, &config_value, 1);
 * // This example writes the value 0x1A to the GPIO_CONFIGURATION0 register of the RFM22 module.
 * @example
 * uint8_t config_values[] = {0x1A, 0x2B, 0x3C};
 * RFM22_SPI_write(&rfm22_dev, RH_RF22_REG_0B_GPIO_CONFIGURATION0, config_values, 3);
 * // This example writes the values 0x1A, 0x2B, and 0x3C to the GPIO_CONFIGURATION0, GPIO_CONFIGURATION1, and GPIO_CONFIGURATION2 registers of the RFM22 module, respectively.
 */
void RFM22_SPI_write(RFM22 *dev, const uint8_t addr, const uint8_t *tx_buffer, const uint8_t size)
{
	uint8_t write_addr = 0x80 | addr;
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, 0);
	HAL_SPI_Transmit(dev->SPIx, &write_addr, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->SPIx, tx_buffer, size, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, 1);
}

/**
 * @brief Reads data from the RFM22 module via SPI.
 * @param dev: Pointer to the RFM22 device structure.
 * @param addr: The address of the register to read from.
 * @param rx_buffer: Pointer to the buffer where the read data will be stored.
 * @param size: The number of bytes to read.
 * @example
 * uint8_t rssi_value;
 * RFM22_SPI_read(&rfm22_dev, RH_RF22_REG_26_RSSI, &rssi_value, 1);
 * // Now rssi_value contains the RSSI value read from the RFM22 module.
 */
void RFM22_SPI_read(RFM22 *dev, uint8_t addr, uint8_t *rx_buffer, uint8_t size)
{
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, 0);
	HAL_SPI_TransmitReceive(dev->SPIx, &addr, rx_buffer, 1, HAL_MAX_DELAY);
	HAL_SPI_TransmitReceive(dev->SPIx, rx_buffer, rx_buffer, size, HAL_MAX_DELAY); // vérifier qu'envoyer le rx buffer ok
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

	float carrier = 10e6 * (hbsel + 1) * ((float)fb + 24.0 + ((float)fc) / 64000.0);
	float channel_dev = 10e3 * (float)fhs * (float)channel;
	return carrier + channel_dev;
}

/**
 * @brief Sets the frequency of the RFM22 module.
 * @param rfm Pointer to the RFM22 device structure.
 * @param frc The desired carrier frequency (en MHz)
 */
void RFM22_set_frequency(RFM22 *rfm, const double frc)
{
	// RFM22_SPI_write(rfm,RH_RF22_REG_76_NOMINAL_CARRIER_FREQUENCY1, )

	// RH_RF22_HBSEL
	uint8_t hbsel;
	uint8_t fb;
	uint8_t fc[2];
	uint8_t channel = 0; // TODO: ajouter channel si besoin, sinon laisser à 0
	const uint16_t XtalFreq = 30000;

	// ------------ variable pour calcule -------
	double _sub_f;
	uint16_t fc_;

	// ---------- calcul de hbsel, fb et fc ---------
	if (frc >= 480)
	{
		hbsel = 1;
	}
	else
	{
		hbsel = 0;
	};

	_sub_f = frc / (10 * (XtalFreq / 30000) * (1 + hbsel));
	fb = ((2 + hbsel) << 5) + (uint8_t)floor(_sub_f) - 24; // '32*hbsel' correspond à un décalage binaire pour correspondre à la forme attendue. (64 pour sbsel=1)
														   // ((2 + hbsel) << 5) == 64 + 32*hbsel
	fc_ = (uint16_t)(round(fmod(_sub_f, 1) * 64000));
	fc[0] = (uint8_t)(fc_ >> 8);
	fc[1] = (uint8_t)(fc_ & 0xFF);

	// ---regroupement des registres à écrire---
	uint8_t ptr[4] = {fb, fc[0], fc[1], channel};
	// ---------- écriture des registres ---------
	RFM22_SPI_write(rfm, RH_RF22_REG_75_FREQUENCY_BAND_SELECT, ptr, 4);
};
