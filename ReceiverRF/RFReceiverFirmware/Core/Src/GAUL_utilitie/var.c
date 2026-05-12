// Author: Samuel Gauthier
// Date: 2026-05-12

#include "GAUL_utilitie/var.h"

// ISR flags
volatile uint8_t rfm22_interrupt_flag = 0;
volatile uint8_t pushbutton_interrupt_flag = 0;
volatile uint8_t pushbutton_pushed[4] = {0};

// RF data
uint8_t packet[8] = {1, 2, 3, 4, 5, 6, 7, 8};
uint8_t rssi = 0;
uint8_t ref_rssi = 0;
uint32_t freq = 0;
float latitude = 0;
float longitude = 0;
uint8_t channel = 0;

// Hardware handles
Pulse_Pin_Typedef pin1;
Pulse_Pin_Typedef pin2;
Pulse_Pin_Typedef pin3;
I2C_LCD_HandleTypeDef lcd;
