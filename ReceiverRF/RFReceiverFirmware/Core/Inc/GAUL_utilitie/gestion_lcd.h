/**
 * @file    gestion_lcd.h
 * @author  Samuel Gauthier
 */

#ifndef __GESTION_LCD_H
#define __GESTION_LCD_H
#include "GAUL_drivers/RFM22.h"
#include "GAUL_drivers/i2c_lcd.h"

/**
 * @brief Function to create a signal strength bar, to be displayed on the LCD, based on the signal strength percentage (0-100%) and the length of the bar (in characters)
 * The bar is made of '#' characters for the signal strength, and '-' characters for the remaining
 * part of the bar, and is stored in the provided string (which should be pre-allocated with a length of at least 'length' + 1 for the null terminator)
 * @param string pointer to the string where the bar will be stored (length should be at least 'length' + 1 for the null terminator)
 * @param length length of the bar in characters
 * @param percent signal strength percentage (0-100%) in float, where 0% is no signal and 100% is full signal
 */
void signal_strenght_bar(char *string, int length, float percent);

/**
 * @brief Function to print the main menu on the LCD, with the current channel, frequency, RSSI, reference RSSI, GPS coordinates and signal strength bar
 * @param dev pointer to the RFM22 device structure, used to get the current frequency
 * @param lcd pointer to the I2C_LCD_HandleTypeDef structure, used to control the LCD
 * @param channel current channel
 * @param rssi current RSSI (Received Signal Strength Indicator)
 * @param ref_rssi reference RSSI
 * @param latitude current latitude
 * @param longitude current longitude
 */
void print_menu(RFM22 *dev, I2C_LCD_HandleTypeDef *lcd, uint8_t channel, uint8_t rssi, uint8_t ref_rssi, float latitude, float longitude);


#endif /* __GESTION_LCD_H */









