/*
 * buzzer.c
 *
 *  Created on: Jun 1, 2025
 *      Author: victo
 */
#include "gaul/drivers/buzzer.h"

#include "stm32f1xx_hal.h"  // Adjust if you use a different STM32 series
#include <stdint.h>
// Static variables to store the buzzer’s timer handle and channel
static TIM_HandleTypeDef *buzzer_timer = NULL;      /** Pointer to the timer handle used for generating PWM signals to drive the buzzer */
static HAL_TIM_ActiveChannel buzzer_channel = 0;    /** The specific timer channel used for PWM output to the buzzer */
static TIM_HandleTypeDef *buzzer_watcher = NULL;   /** Pointer to the timer handle used for timing the duration of the buzzer sound */
static HAL_TIM_ActiveChannel watcher_channel = 0;  /** The specific timer channel used for timing the duration of the buzzer sound */


/**
 * @brief Initializes the buzzer by configuring the specified timer channels for PWM output and timing.
 * @param htim_pwm Pointer to the timer handle used for generating PWM signals to drive the buzzer.
 * @param channel_pwm The specific timer channel used for PWM output to the buzzer.
 * @param htim_watch Pointer to the timer handle used for timing the duration of the buzzer sound.
 * @param channel_watch The specific timer channel used for timing the duration of the buzzer sound.
 *
 * @note This function must be called before using the buzzer to ensure proper configuration of the timers.
 */
void buzzer_init(TIM_HandleTypeDef *htim_pwm, HAL_TIM_ActiveChannel channel_pwm, TIM_HandleTypeDef *htim_watch, HAL_TIM_ActiveChannel channel_watch)
{
    // Store the timer handles and channels for later use
    buzzer_timer = htim_pwm;
    buzzer_channel = channel_pwm;
    buzzer_watcher = htim_watch;
    watcher_channel = channel_watch;

    // Configure the timer for PWM output with initial settings (frequency and duty cycle will be set in buzzer_start)
    TIM_OC_InitTypeDef sConfigOC = {0};

    // Common configuration for PWM mode
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    // Configure the timer channel for PWM output
    HAL_TIM_PWM_ConfigChannel(buzzer_timer, &sConfigOC, buzzer_channel);
    HAL_TIM_PWM_Start(buzzer_timer, buzzer_channel);

    // Configure the timer channel for output compare mode to use as a watchdog for buzzer duration
    set_oc_callback(htim_watch, channel_watch, &_buzzer_stop);
}


void buzzer_start(uint32_t frequency, uint32_t duration_ms)
{
    if (buzzer_timer == NULL) return; // buzzer not initialized

    uint32_t timer_clock = HAL_RCC_GetPCLK2Freq();  // usually APB2 for TIM1
    if (buzzer_timer->Instance != TIM1) {
        timer_clock = HAL_RCC_GetPCLK1Freq();       // fallback for other timers
    }

    // Calculate period and prescaler
    // TODO: avoid magic number 1000 by using a #define for the timer base frequency (e.g., 1 kHz)
    uint32_t prescaler = (timer_clock / (frequency * 1000)) - 1;                // Prescaler to get a timer tick of 1 ms (1 kHz timer base)
    uint32_t period = 1000 - 1;                                                 // Fixed 1 kHz timer base, to simplify duty control

    // Configure timer for PWM output
    buzzer_timer->Instance->PSC = prescaler;
    buzzer_timer->Instance->ARR = period;
    buzzer_timer->Instance->CCR1 = period / 2;  // 50% duty (for CH1; adjust if CH2,3,4)    TODO: use shift for divion by 2 instead of actual division for better performance

    // Update the timer registers
    __HAL_TIM_SET_COMPARE(buzzer_timer, buzzer_channel, period / 2);            // Set duty cycle to 50%
    __HAL_TIM_SET_AUTORELOAD(buzzer_timer, period);                             // Set the period for the desired frequency 
    __HAL_TIM_SET_PRESCALER(buzzer_timer, prescaler);                           // Set the prescaler for the desired frequency

    // Start PWM output
    HAL_TIM_PWM_Start(buzzer_timer, buzzer_channel);

    //start counter (TODO:if not do, make this non-blocking by using the timer interrupt)
    __HAL_TIM_SET_COMPARE(buzzer_watcher, watcher_channel, duration_ms);        // Set the compare value to the desired duration in ms
    HAL_TIM_OC_Stop_IT(buzzer_watcher, watcher_channel);                        // Ensure the timer is stopped before resetting the counter
    __HAL_TIM_SET_COUNTER(buzzer_watcher, 0);                                   // Reset the counter
	HAL_TIM_OC_Start_IT(buzzer_watcher, watcher_channel);                       // Start the timer in output compare mode with interrupt    
}


void _buzzer_stop(int callback_id)
{
	HAL_TIM_PWM_Stop(buzzer_timer, buzzer_channel);             // Stop PWM output
	HAL_TIM_OC_Stop_IT(buzzer_watcher, watcher_channel);        // Stop the timer used for timing the buzzer duration
}
