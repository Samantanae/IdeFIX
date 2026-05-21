/*
 * timer.c
 *
 *  Created on: Jul 29, 2025
 *      Author: victo
 */
#include "GAUL_drivers/timer.h"

// An unordered list of the callback mappings
static TIM_Callback_Mapping callback_map_list[MAX_OC_CHANNELS*MAX_OC_TIMERS];
static uint16_t nbr_callbacks = 0;


/*
 * This is the STM32 output compare callback. We override it
 * here and assign a callback to each channel.
 */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	int callback_idx = find_callback_idx(htim);
	TIM_OC_Callback callback = callback_map_list[callback_idx].timer_callback;
	if (callback != NULL)
		(*callback)(callback_idx);
}


/*
 * Finds the index of the callback corresponding to a specific Timer handle.
 * It searches the callback map until it finds the right callback.
 * If no callback is found, -1 is returned.
 */
int find_callback_idx(TIM_HandleTypeDef *htim)
{
	TIM_Callback_Mapping mapping;
	for (int i = 0; i < nbr_callbacks; i++)
	{
		mapping = callback_map_list[i];
		if ((htim->Instance == mapping.htim->Instance) &&
		    (htim->Channel == mapping.htim->Channel))
		{
			return i;
		}
	}
	return -1;
}


/*
 * Maps the given callback to a timer channel. Will then add
 * the mapping to the callback map list.
 * Returns -1 if the operation is successful
 * Else returns the index of the callback (can be used as an identifier)
 */
int set_oc_callback(TIM_HandleTypeDef *htim, HAL_TIM_ActiveChannel channel, TIM_OC_Callback callback)
{
	if (nbr_callbacks >= (MAX_OC_CHANNELS*MAX_OC_TIMERS))
			return -1;
	TIM_Callback_Mapping mapping = {htim, channel, callback};
	callback_map_list[nbr_callbacks] = mapping;
	nbr_callbacks++;
	return nbr_callbacks-1;
}

