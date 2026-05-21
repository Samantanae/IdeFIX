/*
 * Pulse_pin.h
 *
 *  Created on: May 15, 2025
 *      Author: victo
 */

#ifndef INC_GAUL_DRIVERS_PULSE_PIN_H_
#define INC_GAUL_DRIVERS_PULSE_PIN_H_

#include "stm32f1xx_hal.h"
#include "GAUL_drivers/timer.h"

#define MAX_PINS 3

typedef struct {
	int pin_id;
	GPIO_TypeDef *pin_port;
	uint16_t pin;
	TIM_HandleTypeDef *htim;
	HAL_TIM_ActiveChannel channel;
} Pulse_Pin_Typedef;


Pulse_Pin_Typedef PulsePin_init(GPIO_TypeDef *pin_port, uint16_t pin, TIM_HandleTypeDef *htim, HAL_TIM_ActiveChannel tim_channel);
void PulsePin(Pulse_Pin_Typedef pin, uint16_t time);
void _Stop_Pulse(int pin_id);
Pulse_Pin_Typedef _find_pulse_pin(int pin_id);


#endif /* INC_GAUL_DRIVERS_PULSE_PIN_H_ */
