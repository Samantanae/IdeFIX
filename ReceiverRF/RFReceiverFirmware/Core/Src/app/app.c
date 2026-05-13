#include "app/app.h"

#include "app/app_state.h"
#include "app/services/input_service.h"
#include "app/services/radio_service.h"
#include "bsp/board_init.h"

AppState *app_state = 0;

void App_Init(void)
{
	app_state = AppState_Get();
	AppState_Init(app_state);

	Board_Init(app_state);
	RadioService_Init(app_state);
}

void App_Run(void)
{
#ifdef TRANSMIT
	if ((HAL_GetTick() - app_state->last_tx_tick) >= 1000U)
	{
		app_state->last_tx_tick = HAL_GetTick();
		RFM22_transmit(&app_state->rfm22, app_state->packet, 8);
	}
#endif

#ifdef RECEIVE
	RadioService_EnsureRxMode(app_state);
#endif

	RadioService_ProcessInterrupts(app_state);

#ifdef RECEIVE
	InputService_ProcessButtons(app_state);
#endif
}

void App_OnExti(uint16_t gpio_pin)
{
	if (app_state != 0)
	{
		InputService_OnExti(app_state, gpio_pin);
	}
}


