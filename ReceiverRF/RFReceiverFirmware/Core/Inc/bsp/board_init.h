#ifndef BOARD_INIT_H
#define BOARD_INIT_H

#include "app/app_state.h"

/**
 * @brief Initializes the board by setting up the output pins and the LCD display.
 * @param state Pointer to the application state structure where the board-related state will be stored.
 *
 * @note This function should be called at the beginning of the application after initializing the application state to ensure that all necessary hardware components are properly configured before use.
 */
void Board_Init(AppState *state);

#endif /* BOARD_INIT_H */