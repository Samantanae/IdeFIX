/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include "GAUL_drivers/RFM22.h"
#include "GAUL_drivers/Pulse_pin.h"
#include "GAUL_drivers/i2c_lcd.h"
#include "GAUL_drivers/buzzer.h"
#include "GAUL_drivers/GEO_coordo.h"
#include "stdio.h"
#include <string.h>
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#ifdef RECEIVE
#include "GAUL_drivers/i2c_master.h"
#else
#include "GAUL_drivers/I2C_Slave.h"
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void PUSH_ISR(uint16_t GPIO_pin);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t rfm22_interrupt_flag = 0;
uint8_t pushbutton_interrupt_flag = 0;
uint8_t pushbutton_pushed[4] = { 0 };



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == RFM_IRQ_Pin)
		rfm22_interrupt_flag = 1;
	if ((GPIO_Pin == GPIO1_Pin) | (GPIO_Pin == GPIO2_Pin)
			| (GPIO_Pin == GPIO3_Pin) | (GPIO_Pin == GPIO4_Pin))
		PUSH_ISR(GPIO_Pin);
}

void PUSH_ISR(uint16_t GPIO_pin) {
	pushbutton_interrupt_flag = 1;
	pushbutton_pushed[0] |= (GPIO_pin == GPIO1_Pin);
	pushbutton_pushed[1] |= (GPIO_pin == GPIO2_Pin);
	pushbutton_pushed[2] |= (GPIO_pin == GPIO3_Pin);
	pushbutton_pushed[3] |= (GPIO_pin == GPIO4_Pin);

}


void signal_strenght_bar(char *string, int length, float percent) {
	if (percent > 100)
		percent = 100;
	int nb_square = percent * length / 100;
	int nb_dash = length - nb_square;

	for (int i = 0; i < nb_square; i++) {
		string[i] = '#';
	}
	for (int i = nb_square; i < length; i++) {
		string[i] = '-';
	}
}

void print_menu(RFM22 *dev, I2C_LCD_HandleTypeDef *lcd, uint8_t channel,
		uint8_t rssi, uint8_t ref_rssi, float latitude, float longitude) {
	char line[20] = { '-' };
	uint32_t freq = RFM22_get_frequency(dev);
	uint16_t MHz = freq / 1e6;
	uint16_t kHz = (freq / 1000) % 1000;
	uint16_t Hz = freq % 1000;

	float signal_strenght = 100 * ((float) rssi - (float) ref_rssi)
			/ ((float) ref_rssi + 1) + 50;

	lcd_clear(lcd);
	lcd_gotoxy(lcd, 0, 0); // ligne 1
	snprintf(line, 21, "CH:%-3u   %3lu.%03lu.%03lu", channel, MHz, kHz, Hz);
	lcd_puts(lcd, line);
	lcd_gotoxy(lcd, 0, 1); // ligne 2
	snprintf(line, 20, "RSSI:%u", rssi);
	lcd_puts(lcd, line);
	lcd_gotoxy(lcd, 0, 2); // ligne 3
	strcpy(line, "GPS");
	lcd_puts(lcd, line);
	lcd_gotoxy(lcd, 0, 3); // ligne 4
	signal_strenght_bar(line, 20, signal_strenght);
	lcd_puts(lcd, line);

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */

#ifdef RECEIVE    // master
i2c_data_dispo();	// TODO: put this where the first data is really dispo. it's won't send before that.
#else			  // slave
hi2c1.Init.OwnAddress1 = I2C_ADRESS_SLAVE1;    // changement de l'adr du slave.
	HAL_I2C_EnableListen_IT(&hi2c1);
#endif

	// init pulsed pins and their respective timers
	Pulse_Pin_Typedef pin1 = PulsePin_init(LED1_GPIO_Port, LED1_Pin, &htim2,
			TIM_CHANNEL_1);
	Pulse_Pin_Typedef pin2 = PulsePin_init(LED2_GPIO_Port, LED2_Pin, &htim3,
			TIM_CHANNEL_1);
	Pulse_Pin_Typedef pin3 = PulsePin_init(LED3_GPIO_Port, LED3_Pin, &htim4,
			TIM_CHANNEL_1);

	// init buzzer and its watch timer
	buzzer_init(&htim1, TIM_CHANNEL_3, &htim5, TIM_CHANNEL_1);

	// init lcd
	I2C_LCD_HandleTypeDef lcd;
	lcd.hi2c = &hi2c1;
	lcd.address = 0x27 << 1;
	lcd_init(&lcd);
	lcd_clear(&lcd);

	//init rfm22
	RFM22 rfm22 = { .SPIx = &hspi1, .cs_port = RFM_CS_GPIO_Port, .cs_pin =
			RFM_CS_Pin, .snd_port = RFM_SDN_GPIO_Port, .snd_pin = RFM_SDN_Pin,
			.nirq_port = RFM_IRQ_GPIO_Port, .nirq_pin = RFM_IRQ_Pin,
			.gpio_port_1 = RFM_GPIO1_GPIO_Port, .gpio_pin_1 = RFM_GPIO1_Pin,
			.gpio_port_2 = RFM_GPIO2_GPIO_Port, .gpio_pin_2 = RFM_GPIO2_Pin,
			.gpio_port_3 = RFM_GPIO3_GPIO_Port, .gpio_pin_3 = RFM_GPIO3_Pin };

	RFM22_init(&rfm22, &rfm22_confs);
	uint8_t channel = 0;

	// ------------ GESTION I2C (INITIALISATION MASTER-SLAVE)--------------

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	//global vars
	RFM22_channel(&rfm22, channel);
	print_menu(&rfm22, &lcd, 0, 0, 0, 0, 0);
	const char *packet_str = "VE2VQW";
	uint8_t packet[8];
	memcpy(packet, packet_str, 6);    // copy packet_str to packet without '\0'
	uint8_t rssi = 0;
	uint8_t ref_rssi = 0;
	uint32_t freq;
	float latitude = 0;
	float longitude = 0;
	uint8_t spi_rx[1] = { 0 };

	/*uniquement après que tout soit initialisé que le trensfer peux commencer*/

	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#ifdef TRANSMIT
	  RFM22_transmit(&rfm22, packet, 8);

	  uint32_t tick = HAL_GetTick();
	  while (HAL_GetTick() - tick < 1000);
#endif

#ifdef RECEIVE
		RFM22_SPI_read(&rfm22, RH_RF22_REG_07_OPERATING_MODE1, spi_rx, 1);
		if (!(spi_rx[0] & RH_RF22_RXON)) {
			RFM22_rx_mode(&rfm22);
		}
#endif

// ------- handle I2C---------
#ifdef RECEIVE    // master
		I2C_main_loop(latitude,longitude, 0);
#else			  // slave

#endif

		//handle rfm22 interrupts
		if (rfm22_interrupt_flag
				|| (!HAL_GPIO_ReadPin(RFM_IRQ_GPIO_Port, RFM_IRQ_Pin)))
#ifdef TRANSMIT
	  {
		  rfm22_interrupt_flag = 0;
		  uint8_t interrupts[] = {0, 0};
		  RFM22_SPI_read(&rfm22, RH_RF22_REG_03_INTERRUPT_STATUS1, interrupts, 2);

		  //pk sent interrupt
		  if (interrupts[0] & RH_RF22_IPKSENT)
		  {
			  PulsePin(pin1, 100);
		  }

		  //tx FIFO full
		  if (interrupts[0 & RH_RF22_ITXFFAFULL])
		  {
			  RFM22_clr_tx_FIFO(&rfm22);
		  }
	  }
#endif

#ifdef RECEIVE
				{
			rfm22_interrupt_flag = 0;
			uint8_t interrupts[] = { 0, 0 };
			RFM22_SPI_read(&rfm22, RH_RF22_REG_03_INTERRUPT_STATUS1, interrupts,
					2);

			//pk received
			if (interrupts[0] & RH_RF22_IPKVALID) {
				PulsePin(pin1, 100);
				uint8_t lenght = RFM22_available(&rfm22);

				// GPS routine
				rssi = RFM22_get_RSSI(&rfm22);
				RFM22_read_rx(&rfm22, packet, 8);
				latitude = 0; // à vérifier, supposé transformer en float direct
				longitude = 0;

				// rssi routine
				int16_t rssi_dif = rssi - ref_rssi;
				freq = 3000 + 200 * rssi_dif;
				buzzer_start(freq, 200);
				print_menu(&rfm22, &lcd, channel, rssi, ref_rssi, latitude,
						longitude);
			}

			//rx FIFO full
			if (interrupts[0] & RH_RF22_IRXFFAFULL) {
				//read last received
				//RFM22_clr_rx_FIFO(&rfm22);
				PulsePin(pin3, 100);
			}

			//valid preamble
			if (interrupts[1] & RH_RF22_IPREAVAL) {
			}

			//inval preamble
			if (interrupts[1] & RH_RF22_IPREAINVAL) {
				PulsePin(pin2, 100);
			}
		}

		if (pushbutton_interrupt_flag) {
			pushbutton_interrupt_flag = 0;

			if (pushbutton_pushed[0]) // channel up
			{
				channel++;
				RFM22_channel(&rfm22, channel);
				print_menu(&rfm22, &lcd, channel, rssi, ref_rssi, latitude,
						longitude);
			}
			if (pushbutton_pushed[1]) // channel down
			{
				channel--;
				RFM22_channel(&rfm22, channel);
				print_menu(&rfm22, &lcd, channel, rssi, ref_rssi, latitude,
						longitude);
			}
			if (pushbutton_pushed[2]) // zero
			{
				ref_rssi = rssi;
			}
			if (pushbutton_pushed[3]) {

			}

			pushbutton_pushed[0] = 0;
			pushbutton_pushed[1] = 0;
			pushbutton_pushed[2] = 0;
			pushbutton_pushed[3] = 0;
		}
#endif
	}

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */




/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
