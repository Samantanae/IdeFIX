/*
 * buzzer.c
 *
 *  Created on: Jun 1, 2025
 *      Author: victo
 */
#include "GAUL_drivers/buzzer.h"

#include "stm32f1xx_hal.h"  // Adjust if you use a different STM32 series

// Static variables to store the buzzer’s timer handle and channel
static TIM_HandleTypeDef *buzzer_timer = NULL;
static HAL_TIM_ActiveChannel buzzer_channel = 0;
static TIM_HandleTypeDef *buzzer_watcher = NULL;
static HAL_TIM_ActiveChannel watcher_channel = 0;



void buzzer_init(TIM_HandleTypeDef *htim_pwm, HAL_TIM_ActiveChannel channel_pwm, TIM_HandleTypeDef *htim_watch, HAL_TIM_ActiveChannel channel_watch)
{
    buzzer_timer = htim_pwm;
    buzzer_channel = channel_pwm;
    buzzer_watcher = htim_watch;
    watcher_channel = channel_watch;

    TIM_OC_InitTypeDef sConfigOC = {0};

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;  // 0% duty initially
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(buzzer_timer, &sConfigOC, buzzer_channel);
    HAL_TIM_PWM_Start(buzzer_timer, buzzer_channel);

    set_oc_callback(htim_watch, channel_watch, &_buzzer_stop);
}


void buzzer_start(uint32_t frequency, uint32_t duration_ms)
{
    if (buzzer_timer == NULL) return; // not initialized

    uint32_t timer_clock = HAL_RCC_GetPCLK2Freq();  // usually APB2 for TIM1
    if (buzzer_timer->Instance != TIM1) {
        timer_clock = HAL_RCC_GetPCLK1Freq();       // fallback for other timers
    }

    // Calculate period and prescaler
    uint32_t prescaler = (timer_clock / (frequency * 1000)) - 1;
    uint32_t period = 1000 - 1;  // Fixed 1 kHz timer base, to simplify duty control

    buzzer_timer->Instance->PSC = prescaler;
    buzzer_timer->Instance->ARR = period;
    buzzer_timer->Instance->CCR1 = period / 2;  // 50% duty (for CH1; adjust if CH2,3,4)

    __HAL_TIM_SET_COMPARE(buzzer_timer, buzzer_channel, period / 2);
    __HAL_TIM_SET_AUTORELOAD(buzzer_timer, period);
    __HAL_TIM_SET_PRESCALER(buzzer_timer, prescaler);

    HAL_TIM_PWM_Start(buzzer_timer, buzzer_channel);

    //start counter
    __HAL_TIM_SET_COMPARE(buzzer_watcher, watcher_channel, duration_ms);
    HAL_TIM_OC_Stop_IT(buzzer_watcher, watcher_channel);
    __HAL_TIM_SET_COUNTER(buzzer_watcher, 0);
	HAL_TIM_OC_Start_IT(buzzer_watcher, watcher_channel);
}


void _buzzer_stop(int callback_id)
{
	HAL_TIM_PWM_Stop(buzzer_timer, buzzer_channel);
	HAL_TIM_OC_Stop_IT(buzzer_watcher, watcher_channel);
}
