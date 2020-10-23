/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#define RELAY4_Pin GPIO_PIN_13
#define RELAY4_GPIO_Port GPIOC
#define RELAY1_Pin GPIO_PIN_14
#define RELAY1_GPIO_Port GPIOC
#define RFLED1_Pin GPIO_PIN_15
#define RFLED1_GPIO_Port GPIOC
#define PASS_RELAY4_Pin GPIO_PIN_0
#define PASS_RELAY4_GPIO_Port GPIOF
#define PASS_RELAY2_Pin GPIO_PIN_1
#define PASS_RELAY2_GPIO_Port GPIOF
#define RFLED4_Pin GPIO_PIN_0
#define RFLED4_GPIO_Port GPIOA
#define RFLED3_Pin GPIO_PIN_1
#define RFLED3_GPIO_Port GPIOA
#define RFLED2_Pin GPIO_PIN_2
#define RFLED2_GPIO_Port GPIOA
#define KEY_S1_Pin GPIO_PIN_3
#define KEY_S1_GPIO_Port GPIOA
#define KEY_S4_Pin GPIO_PIN_4
#define KEY_S4_GPIO_Port GPIOA
#define KEY_S2_Pin GPIO_PIN_5
#define KEY_S2_GPIO_Port GPIOA
#define KEY_S3_Pin GPIO_PIN_6
#define KEY_S3_GPIO_Port GPIOA
#define DOUT_Pin GPIO_PIN_9
#define DOUT_GPIO_Port GPIOA
#define DIN_Pin GPIO_PIN_12
#define DIN_GPIO_Port GPIOA
#define DIN_EXTI_IRQn EXTI4_15_IRQn
#define PASS_RELAY1_Pin GPIO_PIN_15
#define PASS_RELAY1_GPIO_Port GPIOA
#define PASS_RELAY3_Pin GPIO_PIN_3
#define PASS_RELAY3_GPIO_Port GPIOB
#define KEY3_Pin GPIO_PIN_4
#define KEY3_GPIO_Port GPIOB
#define KEY2_Pin GPIO_PIN_5
#define KEY2_GPIO_Port GPIOB
#define KEY1_Pin GPIO_PIN_6
#define KEY1_GPIO_Port GPIOB
#define KEY4_Pin GPIO_PIN_7
#define KEY4_GPIO_Port GPIOB
#define RELAY2_Pin GPIO_PIN_8
#define RELAY2_GPIO_Port GPIOB
#define RELAY3_Pin GPIO_PIN_9
#define RELAY3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
