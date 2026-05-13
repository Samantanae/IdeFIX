#ifndef APP_H
#define APP_H

#include <stdint.h>

/**
 * @file    : app.h
 * @authors  :code: Victor
 * @authors  :doc/refactoring: Samuel Gauthier
 * @date    : 2026-05-12
 * @brief   : Header file for the main application module.
 */

/**
 * @brief Initializes the application by setting up the following:
 *   
 * - Application state
 * - Initializing the board
 * - Initializing the radio services
 * 
 * @note This function should be called once at the beginning of the application to prepare all necessary components for operation.
 */
void App_Init(void);

/**
 * @brief Runs the main application loop, which includes the following tasks:
 *  - Processing radio interrupts
 *  - Ensuring the radio is in receive mode (if applicable)
 *  - Processing button inputs
 * 
 * @note This function should be called repeatedly in the main loop of the application to ensure that all tasks are performed in a timely manner.
 */
void App_Run(void);
void App_OnExti(uint16_t gpio_pin);

#endif /* APP_H */