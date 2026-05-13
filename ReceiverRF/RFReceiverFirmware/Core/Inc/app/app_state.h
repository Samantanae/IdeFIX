#ifndef APP_STATE_H
#define APP_STATE_H

#include <stdint.h>

#include "gaul/drivers/RFM22.h"
#include "gaul/drivers/Pulse_pin.h"
#include "gaul/drivers/i2c_lcd.h"

/**
 * @brief The AppState structure holds the entire state of the application,
 *  including flags for interrupts, RF data, hardware handles,
 *  and other relevant information needed for the operation of the application.
 * It is designed to be a single global structure that can be accessed and modified
 *  by different parts of the application, such as the main loop, interrupt handlers,
 *  and service modules.
 */
typedef struct
{
    volatile uint8_t rfm22_interrupt_flag;      /**< Flag indicating an interrupt from the RFM22 module */
    volatile uint8_t pushbutton_interrupt_flag; /**< Flag indicating an interrupt from the pushbutton */
    volatile uint8_t pushbutton_pushed[4];      /**< Array indicating which pushbuttons have been pushed */

    uint8_t packet[8];    /**< Array holding the received RF packet data */
    uint8_t rssi;         /**< Received Signal Strength Indicator (RSSI) value */
    uint8_t ref_rssi;     /**< Reference RSSI value for signal strength calculations */
    uint32_t buzzer_freq; /**< Frequency of the buzzer */
    float latitude;       /**< Current latitude */
    float longitude;      /**< Current longitude */
    uint8_t channel;      /**< Current RF channel */

    Pulse_Pin_Typedef pin1;    /**< Pulse pin structure for controlling an output pin */
    Pulse_Pin_Typedef pin2;    /**< Pulse pin structure for controlling an output pin */
    Pulse_Pin_Typedef pin3;    /**< Pulse pin structure for controlling an output pin */
    I2C_LCD_HandleTypeDef lcd; /**< I2C LCD handle structure */
    RFM22 rfm22;               /**< RFM22 radio module structure */

#ifdef TRANSMIT
    uint32_t last_tx_tick; /**< Timestamp of the last transmission, used for timing transmissions */
#endif
} AppState;

/**
 * @brief Retrieves a pointer to the global application state structure.
 * @return Pointer to the global AppState structure.
 *
 * @note This function provides access to the global application state,
 *  allowing different parts of the application to read and modify the state as needed.
 *  It should be used whenever access to the application state is required,
 *  rather than directly accessing a global variable,
 *  to maintain encapsulation and allow for potential future changes
 *  in how the state is stored or accessed.
 */
AppState *AppState_Get(void);

/**
 * @brief Initializes the application state structure by setting all fields to their default values.
 * @param state Pointer to the AppState structure to be initialized.
 *
 * @note This function should be called at the beginning of the application,
 *  before any other functions that access the application state,
 *  to ensure that all fields are properly initialized to their default values.
 *  It sets the RF packet data to a default sequence and clears all flags and hardware handles.
 */
void AppState_Init(AppState *state);

#endif /* APP_STATE_H */