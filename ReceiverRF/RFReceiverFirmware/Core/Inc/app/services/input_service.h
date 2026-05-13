/**
 * @file    : input_service.h
 * @authors  : Victor
 * @authors  : Samuel Gauthier
 * @date    : 2026-05-12
 * @brief   : Header file for the input service module.
 */

#ifndef INPUT_SERVICE_H
#define INPUT_SERVICE_H

#include <stdint.h>

#include "app/app_state.h"

/**
 * @brief Handles external interrupts for pushbuttons and the RFM22 module.
 * Sets appropriate flags in the application state to indicate which buttons were pressed and if the RFM22 triggered an interrupt.
 * @param state Pointer to the application state structure where the interrupt flags will be set.
 * @param gpio_pin The GPIO pin number that triggered the interrupt, used to determine which button was pressed or if the RFM22 triggered the interrupt.
 *
 * @note This function is intended to be called from the EXTI interrupt handler when a GPIO pin triggers an interrupt.
 *     It should not be called directly from the main application code, as it is designed to be used in an interrupt context.
 */
void InputService_OnExti(AppState *state, uint16_t gpio_pin);

/**
 * @brief Processes the pushbutton interrupts by checking the flags set in the application state.
 * If a button was pressed, it performs the corresponding action (e.g., changing the channel, setting the reference RSSI) and updates the display accordingly.
 * @param state Pointer to the application state structure where the pushbutton interrupt flags are checked and cleared.
 *
 * @note This function should be called regularly in the main application loop to ensure that button presses are processed in a timely manner.
 */
void InputService_ProcessButtons(AppState *state);

#endif /* INPUT_SERVICE_H */