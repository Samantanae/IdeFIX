/*
 * timer.h
 *
 *  Created on: Jul 29, 2025
 *      Author: victo
 */

#ifndef INC_GAUL_DRIVERS_TIMER_H_
#define INC_GAUL_DRIVERS_TIMER_H_

#include "stm32f1xx_hal.h"

#define MAX_OC_TIMERS 4
#define MAX_OC_CHANNELS 4

// pointer to timer output compare event routine
typedef void (*TIM_OC_Callback)(int);

// Maps each timer channel to its output compare routine
typedef struct {
	TIM_HandleTypeDef *htim;
	HAL_TIM_ActiveChannel channel;
	TIM_OC_Callback timer_callback;
} TIM_Callback_Mapping;

int find_callback_idx(TIM_HandleTypeDef *htim);
int set_oc_callback(TIM_HandleTypeDef *htim, HAL_TIM_ActiveChannel channel, TIM_OC_Callback callback);

#endif /* INC_GAUL_DRIVERS_TIMER_H_ */
