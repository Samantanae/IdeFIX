#ifndef UI_SERVICE_H
#define UI_SERVICE_H

#include "app/app_state.h"

/**
 * @brief Updates the LCD display with the current channel, frequency, RSSI, GPS status, and signal strength bar based on the application state.
 * @param state Pointer to the application state structure containing the current `channel`, `frequency`, `RSSI`
 * and `reference RSSI` values used to calculate the signal strength percentage for the signal strength bar.
 * 
 * @note This function should be called whenever there is a change in the channel, frequency, RSSI, or reference RSSI values to ensure that the display reflects the current state of the application.
 */
void UIService_PrintMenu(AppState *state);

#endif /* UI_SERVICE_H */