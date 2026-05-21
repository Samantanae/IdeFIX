/*
 * Pulse_pin.c
 *
 *  Created on: May 15, 2025
 *      Author: victo
 */
#include "GAUL_drivers/Pulse_pin.h"

static Pulse_Pin_Typedef pin_list[MAX_PINS];
static int nb_pins = 0;


Pulse_Pin_Typedef PulsePin_init(GPIO_TypeDef *pin_port, uint16_t pin, TIM_HandleTypeDef *htim, HAL_TIM_ActiveChannel tim_channel)
{
	  HAL_TIM_OC_Stop_IT(htim, tim_channel);
	  int pin_id = set_oc_callback(htim, tim_channel, &_Stop_Pulse);

	  Pulse_Pin_Typedef pin_struct = {
		  .pin_id = pin_id,
		  .pin_port = pin_port,
		  .pin = pin,
		  .htim = htim,
		  .channel = tim_channel
	  };
	  pin_list[nb_pins] = pin_struct;
	  nb_pins++;

	  return pin_struct;
}

void PulsePin(Pulse_Pin_Typedef pin, uint16_t time)
{
	  __HAL_TIM_SET_COMPARE(pin.htim, pin.channel, time);
	  HAL_GPIO_WritePin(pin.pin_port, pin.pin, 1);
	  __HAL_TIM_SET_COUNTER(pin.htim, 0);
	  HAL_TIM_OC_Start_IT(pin.htim, pin.channel);
}


void _Stop_Pulse(int pin_id)
{
	Pulse_Pin_Typedef pin = _find_pulse_pin(pin_id);
	HAL_GPIO_WritePin(pin.pin_port, pin.pin, 0);
	HAL_TIM_OC_Stop_IT(pin.htim, pin.channel);
}



Pulse_Pin_Typedef _find_pulse_pin(int pin_id)
{
	Pulse_Pin_Typedef pin;
	for (int i=0; i < MAX_PINS; i++)
	{
		pin = pin_list[i];
		if (pin.pin_id == pin_id)
		{
			return pin;
		}
	}
	return pin;
}
