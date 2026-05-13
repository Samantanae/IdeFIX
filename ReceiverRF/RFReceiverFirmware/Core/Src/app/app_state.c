#include "app/app_state.h"

#include <string.h>

static AppState g_app_state;

AppState *AppState_Get(void)
{
	return &g_app_state;
}

void AppState_Init(AppState *state)
{
	memset(state, 0, sizeof(*state));

	state->packet[0] = 1;
	state->packet[1] = 2;
	state->packet[2] = 3;
	state->packet[3] = 4;
	state->packet[4] = 5;
	state->packet[5] = 6;
	state->packet[6] = 7;
	state->packet[7] = 8;
}