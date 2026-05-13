#include "app/services/input_service.h"

#include "gpio.h"

#include "app/services/ui_service.h"

void InputService_OnExti(AppState *state, uint16_t gpio_pin)
{
	if ((gpio_pin == GPIO1_Pin) || (gpio_pin == GPIO2_Pin) || (gpio_pin == GPIO3_Pin) || (gpio_pin == GPIO4_Pin))
	{
		state->pushbutton_interrupt_flag = 1;
		state->pushbutton_pushed[0] |= (gpio_pin == GPIO1_Pin);
		state->pushbutton_pushed[1] |= (gpio_pin == GPIO2_Pin);
		state->pushbutton_pushed[2] |= (gpio_pin == GPIO3_Pin);
		state->pushbutton_pushed[3] |= (gpio_pin == GPIO4_Pin);
	}

	if (gpio_pin == RFM_IRQ_Pin)
	{
		state->rfm22_interrupt_flag = 1;
	}
}

void InputService_ProcessButtons(AppState *state)
{
	if (!state->pushbutton_interrupt_flag)
	{
		return;
	}

	state->pushbutton_interrupt_flag = 0;

	if (state->pushbutton_pushed[0])
	{
		state->channel++;
		RFM22_channel(&state->rfm22, state->channel);
		UIService_PrintMenu(state);
	}

	if (state->pushbutton_pushed[1])
	{
		state->channel--;
		RFM22_channel(&state->rfm22, state->channel);
		UIService_PrintMenu(state);
	}

	if (state->pushbutton_pushed[2])
	{
		state->ref_rssi = state->rssi;
	}

	state->pushbutton_pushed[0] = 0;
	state->pushbutton_pushed[1] = 0;
	state->pushbutton_pushed[2] = 0;
	state->pushbutton_pushed[3] = 0;
}

