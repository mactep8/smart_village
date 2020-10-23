/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f0xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RELAY3_Pin GPIO_PIN_13
#define RELAY3_GPIO_Port GPIOC
#define RELAY4_Pin GPIO_PIN_14
#define RELAY4_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_15
#define LED4_GPIO_Port GPIOC
#define PASS_RELAY_4_Pin GPIO_PIN_0
#define PASS_RELAY_4_GPIO_Port GPIOF
#define PASS_RELAY_2_Pin GPIO_PIN_1
#define PASS_RELAY_2_GPIO_Port GPIOF
#define LED3_Pin GPIO_PIN_0
#define LED3_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOA
#define KEY_S1_Pin GPIO_PIN_3
#define KEY_S1_GPIO_Port GPIOA
#define KEY_S4_Pin GPIO_PIN_4
#define KEY_S4_GPIO_Port GPIOA
#define KEY_S2_Pin GPIO_PIN_5
#define KEY_S2_GPIO_Port GPIOA
#define KEY_S3_Pin GPIO_PIN_6
#define KEY_S3_GPIO_Port GPIOA
#define SW8_Pin GPIO_PIN_1
#define SW8_GPIO_Port GPIOB
#define SW7_Pin GPIO_PIN_2
#define SW7_GPIO_Port GPIOB
#define SW6_Pin GPIO_PIN_10
#define SW6_GPIO_Port GPIOB
#define SW5_Pin GPIO_PIN_11
#define SW5_GPIO_Port GPIOB
#define SW4_Pin GPIO_PIN_12
#define SW4_GPIO_Port GPIOB
#define SW3_Pin GPIO_PIN_13
#define SW3_GPIO_Port GPIOB
#define SW2_Pin GPIO_PIN_14
#define SW2_GPIO_Port GPIOB
#define SW1_Pin GPIO_PIN_15
#define SW1_GPIO_Port GPIOB
#define LOCK_SW_Pin GPIO_PIN_8
#define LOCK_SW_GPIO_Port GPIOA
#define CLR_BTN_Pin GPIO_PIN_11
#define CLR_BTN_GPIO_Port GPIOA
#define RF_IN_Pin GPIO_PIN_12
#define RF_IN_GPIO_Port GPIOA
#define RF_IN_EXTI_IRQn EXTI4_15_IRQn
#define PASS_RELAY_1_Pin GPIO_PIN_15
#define PASS_RELAY_1_GPIO_Port GPIOA
#define PASS_RELAY_3_Pin GPIO_PIN_3
#define PASS_RELAY_3_GPIO_Port GPIOB
#define ESP_KEY_3_Pin GPIO_PIN_4
#define ESP_KEY_3_GPIO_Port GPIOB
#define ESP_KEY_2_Pin GPIO_PIN_5
#define ESP_KEY_2_GPIO_Port GPIOB
#define ESP_KEY_1_Pin GPIO_PIN_6
#define ESP_KEY_1_GPIO_Port GPIOB
#define ESP_KEY_4_Pin GPIO_PIN_7
#define ESP_KEY_4_GPIO_Port GPIOB
#define RELAY1_Pin GPIO_PIN_8
#define RELAY1_GPIO_Port GPIOB
#define RELAY2_Pin GPIO_PIN_9
#define RELAY2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
