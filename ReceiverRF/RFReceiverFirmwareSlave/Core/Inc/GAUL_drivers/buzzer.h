/*
 * buzzer.h
 *
 *  Created on: Jun 1, 2025
 *      Author: victo
 */

#ifndef INC_GAUL_DRIVERS_BUZZER_H_
#define INC_GAUL_DRIVERS_BUZZER_H_

#include "stm32f1xx_hal.h"
#include "GAUL_drivers/timer.h"

void buzzer_init(TIM_HandleTypeDef *htim_pwm, HAL_TIM_ActiveChannel channel_pwm, TIM_HandleTypeDef *htim_watch, HAL_TIM_ActiveChannel channel_watch);
void buzzer_start(uint32_t frequency, uint32_t duration_ms);
void _buzzer_stop(int callback_id);


#endif /* INC_GAUL_DRIVERS_BUZZER_H_ */
