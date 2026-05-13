#ifndef RADIO_SERVICE_H
#define RADIO_SERVICE_H

#include "app/app_state.h"


/**
 * @brief Initializes the radio service by configuring the RFM22 module
 *  with the appropriate settings and preparing it for operation.
 * @param state Pointer to the application state structure where the radio
 *  service state will be stored.
 *
 * @note This function must be called before using any other radio service functions to ensure proper initialization of the RFM22 module.
 *  It also updates the user interface to reflect the initial state of the radio service.
 */
void RadioService_Init(AppState *state);

/**
 * @brief Ensures that the radio service is in receive mode.
 * @param state Pointer to the application state structure where the radio service state is stored.
 */
void RadioService_EnsureRxMode(AppState *state);

/**
 * @brief Processes radio interrupts by checking the flags set in the application state.
 * @param state Pointer to the application state structure where the radio interrupt flags are checked and cleared.
 *
 * @note This function should be called regularly in the main application loop
 *  to ensure that radio interrupts are processed in a timely manner.
 */
void RadioService_ProcessInterrupts(AppState *state);

#endif /* RADIO_SERVICE_H */