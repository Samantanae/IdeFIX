// Author: Samuel Gauthier
// Date: 2026-05-12

#include "stdio.h"
#include <string.h>
#include <stdint.h>
#include "GAUL_drivers/Pulse_pin.h"
#include "GAUL_drivers/i2c_lcd.h"

#ifndef VAR_H
#define VAR_H

// ISR flags
extern volatile uint8_t rfm22_interrupt_flag;      // flag set in the ISR when the RFM22 triggers an interrupt
extern volatile uint8_t pushbutton_interrupt_flag; // flag set in the ISR when a pushbutton is pressed
extern volatile uint8_t pushbutton_pushed[4];      // array to keep track of which pushbuttons were pressed

// RF data
extern uint8_t packet[8];   // received or transmitted packet
extern uint8_t rssi;        // received signal strength indicator
extern uint8_t ref_rssi;    // reference signal strength
extern uint32_t freq;       // buzzer frequency derived from RSSI
extern float latitude;
extern float longitude;
extern uint8_t channel;

// Hardware handles
extern Pulse_Pin_Typedef pin1;
extern Pulse_Pin_Typedef pin2;
extern Pulse_Pin_Typedef pin3;
extern I2C_LCD_HandleTypeDef lcd;

#endif /* VAR_H */
