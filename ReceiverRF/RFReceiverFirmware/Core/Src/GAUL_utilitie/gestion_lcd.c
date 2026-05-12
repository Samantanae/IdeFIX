




#include "GAUL_utilitie/gestion_lcd.h"
#include "GAUL_utilitie/var.h"
#include "GAUL_drivers/RFM22.h"
#include "GAUL_drivers/i2c_lcd.h"
#include "stdio.h"


// function to create a signal strength bar, to be displayed on the LCD, based on the signal strength percentage (0-100%) and the length of the bar (in characters)
// the bar is made of '#' characters for the signal strength, and '-' characters for the remaining part of the bar, and is stored in the provided string (which should be pre-allocated with a length of at least 'length' + 1 for the null terminator)
//  string: pointer to the string where the bar will be stored (length should be at least 'length' + 1 for the null terminator)
//  length: length of the bar in characters
//  percent: signal strength percentage (0-100%) in float, where 0% is no signal and 100% is full signal
void signal_strenght_bar(char *string, int length, float percent)
{
  // limit percent to 100%
  if (percent > 100)
    percent = 100;
  int nb_square = percent * length / 100;

  for (int i = 0; i < nb_square; i++)
  {
    string[i] = '#';
  }
  for (int i = nb_square; i < length; i++)
  {
    string[i] = '-';
  }
  string[length] = '\0'; // null terminator
}

// function to print the main menu on the LCD, with the current channel, frequency, RSSI, reference RSSI, GPS coordinates and signal strength bar
void print_menu(RFM22 *dev, I2C_LCD_HandleTypeDef *lcd, uint8_t channel, uint8_t rssi, uint8_t ref_rssi, float latitude, float longitude)
{
  char line[21] = {'-'};
  uint32_t freq = RFM22_get_frequency(dev);
  uint16_t MHz = freq / 1e6;
  uint16_t kHz = (freq / 1000) % 1000;
  uint16_t Hz = freq % 1000;

  float signal_strenght = 100 * ((float)rssi - (float)ref_rssi) / ((float)ref_rssi + 1) + 50;

  lcd_clear(lcd);
  lcd_gotoxy(lcd, 0, 0); // ligne 1               (la fréquence)
  snprintf(line, sizeof(line), "CH:%-3u   %3lu.%03lu.%03lu", channel, MHz, kHz, Hz);
  lcd_puts(lcd, line);
  lcd_gotoxy(lcd, 0, 1); // ligne 2               (RSSI)
  snprintf(line, sizeof(line), "RSSI:%u", rssi);
  lcd_puts(lcd, line);
  lcd_gotoxy(lcd, 0, 2); // ligne 3               (GPS)
  strcpy(line, "GPS");
  lcd_puts(lcd, line);
  lcd_gotoxy(lcd, 0, 3); // ligne 4               (bare du signal)
  signal_strenght_bar(line, 20, signal_strenght);
  lcd_puts(lcd, line);
}