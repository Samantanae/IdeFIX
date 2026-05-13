#include "app/services/ui_service.h"

#include <stdio.h>
#include <string.h>

static void UIService_BuildSignalStrengthBar(char *string, int length, float percent)
{
	if (percent > 100.0f)
	{
		percent = 100.0f;
	}

	if (percent < 0.0f)
	{
		percent = 0.0f;
	}

	int nb_square = (int)((percent * (float)length) / 100.0f);

	for (int i = 0; i < nb_square; i++)
	{
		string[i] = '#';
	}

	for (int i = nb_square; i < length; i++)
	{
		string[i] = '-';
	}

	string[length] = '\0';
}

void UIService_PrintMenu(AppState *state)
{
	char line[21] = {'-'};
	float freq = RFM22_get_frequency(&state->rfm22);
	uint32_t freq_u32 = (uint32_t)freq;
	uint16_t mhz = (uint16_t)(freq_u32 / 1000000U);
	uint16_t khz = (uint16_t)((freq_u32 / 1000U) % 1000U);
	uint16_t hz = (uint16_t)(freq_u32 % 1000U);

	float signal_strength = 100.0f * ((float)state->rssi - (float)state->ref_rssi) / ((float)state->ref_rssi + 1.0f) + 50.0f;

	lcd_clear(&state->lcd);
	lcd_gotoxy(&state->lcd, 0, 0);
	snprintf(line, sizeof(line), "CH:%-3u   %3u.%03u.%03u", state->channel, mhz, khz, hz);
	lcd_puts(&state->lcd, line);

	lcd_gotoxy(&state->lcd, 0, 1);
	snprintf(line, sizeof(line), "RSSI:%u", state->rssi);
	lcd_puts(&state->lcd, line);

	lcd_gotoxy(&state->lcd, 0, 2);
	strcpy(line, "GPS");
	lcd_puts(&state->lcd, line);

	lcd_gotoxy(&state->lcd, 0, 3);
	UIService_BuildSignalStrengthBar(line, 20, signal_strength);
	lcd_puts(&state->lcd, line);
}

