#include "bsp/board_init.h"

#include "gpio.h"
#include "i2c.h"
#include "tim.h"

#include "gaul/drivers/Pulse_pin.h"
#include "gaul/drivers/i2c_lcd.h"

static void Board_InitOutputs(AppState *state)
{
	state->pin1 = PulsePin_init(LED1_GPIO_Port, LED1_Pin, &htim2, TIM_CHANNEL_1);
	state->pin2 = PulsePin_init(LED2_GPIO_Port, LED2_Pin, &htim3, TIM_CHANNEL_1);
	state->pin3 = PulsePin_init(LED3_GPIO_Port, LED3_Pin, &htim4, TIM_CHANNEL_1);
}

static void Board_InitDisplay(AppState *state)
{
	state->lcd.hi2c = &hi2c1;
	state->lcd.address = 0x27 << 1;
	lcd_init(&state->lcd);
	lcd_clear(&state->lcd);
}

void Board_Init(AppState *state)
{
	Board_InitOutputs(state);
	Board_InitDisplay(state);
	state->channel = 0;
}

