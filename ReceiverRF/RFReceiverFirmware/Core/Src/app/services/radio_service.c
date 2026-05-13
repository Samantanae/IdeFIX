#include "app/services/radio_service.h"

#include "gpio.h"
#include "spi.h"
#include "tim.h"

#include "gaul/drivers/buzzer.h"
#include "gaul/drivers/Pulse_pin.h"

#include "app/services/ui_service.h"

void RadioService_Init(AppState *state)
{
	buzzer_init(&htim1, TIM_CHANNEL_3, &htim5, TIM_CHANNEL_1);

	state->rfm22.SPIx = &hspi1;
	state->rfm22.cs_port = RFM_CS_GPIO_Port;
	state->rfm22.cs_pin = RFM_CS_Pin;
	state->rfm22.snd_port = RFM_SDN_GPIO_Port;
	state->rfm22.snd_pin = RFM_SDN_Pin;
	state->rfm22.nirq_port = RFM_IRQ_GPIO_Port;
	state->rfm22.nirq_pin = RFM_IRQ_Pin;
	state->rfm22.gpio_port_1 = RFM_GPIO1_GPIO_Port;
	state->rfm22.gpio_pin_1 = RFM_GPIO1_Pin;
	state->rfm22.gpio_port_2 = RFM_GPIO2_GPIO_Port;
	state->rfm22.gpio_pin_2 = RFM_GPIO2_Pin;
	state->rfm22.gpio_port_3 = RFM_GPIO3_GPIO_Port;
	state->rfm22.gpio_pin_3 = RFM_GPIO3_Pin;

	RFM22_init(&state->rfm22, &rfm22_confs);
	RFM22_channel(&state->rfm22, state->channel);
	UIService_PrintMenu(state);

#ifdef TRANSMIT
	state->last_tx_tick = HAL_GetTick();
#endif
}

void RadioService_EnsureRxMode(AppState *state)
{
	uint8_t spi_rx[1] = {0};
	RFM22_SPI_read(&state->rfm22, RH_RF22_REG_07_OPERATING_MODE1, spi_rx, 1);
	if (!(spi_rx[0] & RH_RF22_RXON))
	{
		RFM22_rx_mode(&state->rfm22);
	}
}

void RadioService_ProcessInterrupts(AppState *state)
{
	if (!(state->rfm22_interrupt_flag || (!HAL_GPIO_ReadPin(RFM_IRQ_GPIO_Port, RFM_IRQ_Pin))))
	{
		return;
	}

	state->rfm22_interrupt_flag = 0;
	uint8_t interrupts[] = {0, 0};
	RFM22_SPI_read(&state->rfm22, RH_RF22_REG_03_INTERRUPT_STATUS1, interrupts, 2);

#ifdef TRANSMIT
	if (interrupts[0] & RH_RF22_IPKSENT)
	{
		PulsePin(state->pin1, 100);
	}
	if (interrupts[0] & RH_RF22_ITXFFAFULL)
	{
		RFM22_clr_tx_FIFO(&state->rfm22);
	}
#endif

#ifdef RECEIVE
	if (interrupts[0] & RH_RF22_IPKVALID)
	{
		PulsePin(state->pin1, 100);
		(void)RFM22_available(&state->rfm22);

		state->rssi = RFM22_get_RSSI(&state->rfm22);
		RFM22_read_rx(&state->rfm22, state->packet, 8);
		state->latitude = 0;
		state->longitude = 0;

		int16_t rssi_dif = (int16_t)state->rssi - (int16_t)state->ref_rssi;
		state->buzzer_freq = 3000U + (uint32_t)(200 * rssi_dif);
		buzzer_start(state->buzzer_freq, 200);
		UIService_PrintMenu(state);
	}

	if (interrupts[0] & RH_RF22_IRXFFAFULL)
	{
		PulsePin(state->pin3, 100);
	}

	if (interrupts[1] & RH_RF22_IPREAINVAL)
	{
		PulsePin(state->pin2, 100);
	}
#endif
}