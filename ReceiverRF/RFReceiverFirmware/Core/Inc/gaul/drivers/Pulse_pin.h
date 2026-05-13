/*
 * Pulse_pin.h
 *
 *  Created on: May 15, 2025
 *      Author: victo
 */

#ifndef INC_GAUL_DRIVERS_PULSE_PIN_H_
#define INC_GAUL_DRIVERS_PULSE_PIN_H_

#include "stm32f1xx_hal.h"
#include "gaul/drivers/timer.h"

#define MAX_PINS 3

/**
 * @brief Structure representing a pulse pin configuration
 */
typedef struct 
{
	int pin_id;						/**< Unique identifier for the pulse pin */
	GPIO_TypeDef *pin_port;			/**< GPIO port for the pulse pin */
	uint16_t pin;					/**< GPIO pin number */
	TIM_HandleTypeDef *htim;		/**< Timer handle associated with the pulse pin */
	HAL_TIM_ActiveChannel channel;	/**< Timer channel associated with the pulse pin */
} Pulse_Pin_Typedef;

/**
 * @brief Initializes a pulse pin with the specified configuration.
 * @param pin_port GPIO port for the pulse pin.
 * @param pin GPIO pin number.
 * @param htim Timer handle associated with the pulse pin.
 * @param tim_channel Timer channel associated with the pulse pin.
 * @return A Pulse_Pin_Typedef structure containing the configuration of the initialized pulse pin.
 * 
 * @note This function should be called to set up a pulse pin before using it to generate pulses.
 *  It configures the specified GPIO pin and timer channel for use as a pulse output.
 */
Pulse_Pin_Typedef PulsePin_init(GPIO_TypeDef *pin_port, uint16_t pin, TIM_HandleTypeDef *htim, HAL_TIM_ActiveChannel tim_channel);

/**
 * @brief Generates a pulse on the specified pulse pin for a given duration.
 * @param pin The Pulse_Pin_Typedef structure representing the pulse pin to generate the pulse on.
 * @param time The duration of the pulse in milliseconds.
 * 
 * 	@note This function starts the timer associated with the pulse pin to generate a pulse of the specified duration on the configured GPIO pin.
 *  The pulse will automatically stop after the specified time has elapsed, and the pin will return to its default state.
 */
void PulsePin(Pulse_Pin_Typedef pin, uint16_t time);
/**
 * @brief Stops the pulse on the specified pulse pin immediately.
 * @param pin The Pulse_Pin_Typedef structure representing the pulse pin to stop the pulse
 * 
 * @note This function can be called to stop the pulse on the specified pin immediately, regardless of the remaining time for the pulse. It will stop the timer associated with the pulse pin and reset the GPIO pin to its default state.
 */
void _Stop_Pulse(int pin_id);
/**
 * @brief Internal function to find a pulse pin configuration by its unique identifier.
 * @param pin_id The unique identifier of the pulse pin to find.
 * 
 * @note This function is used internally to retrieve the configuration of a pulse pin based on its unique identifier. It searches through the configured pulse pins and returns the corresponding Pulse_Pin_Typedef structure. It is not intended to be called directly from application code.
 */
Pulse_Pin_Typedef _find_pulse_pin(int pin_id);


#endif /* INC_GAUL_DRIVERS_PULSE_PIN_H_ */
