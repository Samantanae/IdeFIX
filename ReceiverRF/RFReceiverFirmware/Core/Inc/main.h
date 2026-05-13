/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOA
#define RFM_SDN_Pin GPIO_PIN_3
#define RFM_SDN_GPIO_Port GPIOA
#define RFM_IRQ_Pin GPIO_PIN_4
#define RFM_IRQ_GPIO_Port GPIOA
#define RFM_IRQ_EXTI_IRQn EXTI4_IRQn
#define RFM_CS_Pin GPIO_PIN_4
#define RFM_CS_GPIO_Port GPIOC
#define RFM_GPIO3_Pin GPIO_PIN_5
#define RFM_GPIO3_GPIO_Port GPIOC
#define RFM_GPIO2_Pin GPIO_PIN_0
#define RFM_GPIO2_GPIO_Port GPIOB
#define RFM_GPIO1_Pin GPIO_PIN_1
#define RFM_GPIO1_GPIO_Port GPIOB
#define RFM_GPIO5_Pin GPIO_PIN_2
#define RFM_GPIO5_GPIO_Port GPIOB
#define RFM_GPIO4_Pin GPIO_PIN_10
#define RFM_GPIO4_GPIO_Port GPIOB
#define GPIO1_Pin GPIO_PIN_6
#define GPIO1_GPIO_Port GPIOC
#define GPIO1_EXTI_IRQn EXTI9_5_IRQn
#define GPIO2_Pin GPIO_PIN_7
#define GPIO2_GPIO_Port GPIOC
#define GPIO2_EXTI_IRQn EXTI9_5_IRQn
#define GPIO3_Pin GPIO_PIN_8
#define GPIO3_GPIO_Port GPIOC
#define GPIO3_EXTI_IRQn EXTI9_5_IRQn
#define GPIO4_Pin GPIO_PIN_9
#define GPIO4_GPIO_Port GPIOC
#define GPIO4_EXTI_IRQn EXTI9_5_IRQn
#define MLX_INT_TRIG_Pin GPIO_PIN_11
#define MLX_INT_TRIG_GPIO_Port GPIOA
#define MLX_INT_Pin GPIO_PIN_12
#define MLX_INT_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define RECEIVE // TRANSMIT or RECEIVE

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
