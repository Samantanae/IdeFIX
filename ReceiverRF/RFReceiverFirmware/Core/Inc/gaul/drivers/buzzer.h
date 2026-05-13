/*
 * buzzer.h
 *
 *  Created on: Jun 1, 2025
 *      Author: victo
 */

#ifndef INC_GAUL_DRIVERS_BUZZER_H_
#define INC_GAUL_DRIVERS_BUZZER_H_

#include "stm32f1xx_hal.h"
#include "gaul/drivers/timer.h"
#include <stdint.h>

/**
 * @brief Initializes the buzzer by configuring the specified timer channels for PWM output and timing.
 * @param htim_pwm Pointer to the timer handle used for generating PWM signals to drive the buzzer.
 * @param channel_pwm The specific timer channel used for PWM output to the buzzer.
 * @param htim_watch Pointer to the timer handle used for timing the duration of the buzzer sound.
 * @param channel_watch The specific timer channel used for timing the duration of the buzzer sound.
 *
 * @note This function must be called before using the buzzer to ensure proper configuration of the timers.
 */
void buzzer_init(TIM_HandleTypeDef *htim_pwm, HAL_TIM_ActiveChannel channel_pwm, TIM_HandleTypeDef *htim_watch, HAL_TIM_ActiveChannel channel_watch);

/**
 * @brief Starts the buzzer at the specified frequency for a given duration.
 * @param frequency The frequency in Hz at which the buzzer should sound.
 * @param duration_ms The duration in milliseconds for which the buzzer should sound.
 *
 * @note This function must be called after buzzer_init() to ensure proper configuration of the timers.
 */
void buzzer_start(uint32_t frequency, uint32_t duration_ms);

/**
 * @brief Stops the buzzer immediately.
 * @param callback_id An integer identifier for the callback, which can be used to differentiate between multiple callbacks if necessary (not used in the current implementation).
 *
 * @note This function is intended to be called as a callback when the timer used for timing the buzzer duration expires, but it can also be called directly to stop the buzzer immediately.
 */
void _buzzer_stop(int callback_id);

#endif /* INC_GAUL_DRIVERS_BUZZER_H_ */
