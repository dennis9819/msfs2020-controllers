/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BOARD_LED_Pin GPIO_PIN_13
#define BOARD_LED_GPIO_Port GPIOC
#define POT1_Pin GPIO_PIN_0
#define POT1_GPIO_Port GPIOA
#define POT2_Pin GPIO_PIN_1
#define POT2_GPIO_Port GPIOA
#define KEY_OFF_Pin GPIO_PIN_3
#define KEY_OFF_GPIO_Port GPIOA
#define KEY_MAG_L_Pin GPIO_PIN_4
#define KEY_MAG_L_GPIO_Port GPIOA
#define KEY_MAG_R_Pin GPIO_PIN_5
#define KEY_MAG_R_GPIO_Port GPIOA
#define KEY_MAG_BOTH_Pin GPIO_PIN_6
#define KEY_MAG_BOTH_GPIO_Port GPIOA
#define KEY_START_Pin GPIO_PIN_7
#define KEY_START_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_14
#define SW1_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_15
#define SW2_GPIO_Port GPIOA
#define SW3_Pin GPIO_PIN_3
#define SW3_GPIO_Port GPIOB
#define SW4_Pin GPIO_PIN_4
#define SW4_GPIO_Port GPIOB
#define SW5_Pin GPIO_PIN_5
#define SW5_GPIO_Port GPIOB
#define SW6_Pin GPIO_PIN_6
#define SW6_GPIO_Port GPIOB
#define SW7_Pin GPIO_PIN_7
#define SW7_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
