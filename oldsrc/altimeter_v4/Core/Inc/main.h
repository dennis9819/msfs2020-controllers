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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define M3_A1_Pin GPIO_PIN_0
#define M3_A1_GPIO_Port GPIOA
#define M3_A2_Pin GPIO_PIN_1
#define M3_A2_GPIO_Port GPIOA
#define M3_B1_Pin GPIO_PIN_2
#define M3_B1_GPIO_Port GPIOA
#define M3_B2_Pin GPIO_PIN_3
#define M3_B2_GPIO_Port GPIOA
#define M4_A1_Pin GPIO_PIN_4
#define M4_A1_GPIO_Port GPIOA
#define M4_A2_Pin GPIO_PIN_5
#define M4_A2_GPIO_Port GPIOA
#define M4_B1_Pin GPIO_PIN_6
#define M4_B1_GPIO_Port GPIOA
#define M4_B2_Pin GPIO_PIN_7
#define M4_B2_GPIO_Port GPIOA
#define M1_A1_Pin GPIO_PIN_12
#define M1_A1_GPIO_Port GPIOB
#define M1_A2_Pin GPIO_PIN_13
#define M1_A2_GPIO_Port GPIOB
#define M1_B1_Pin GPIO_PIN_14
#define M1_B1_GPIO_Port GPIOB
#define M1_B2_Pin GPIO_PIN_15
#define M1_B2_GPIO_Port GPIOB
#define ENC_A_Pin GPIO_PIN_8
#define ENC_A_GPIO_Port GPIOA
#define ENC_B_Pin GPIO_PIN_9
#define ENC_B_GPIO_Port GPIOA
#define M2_A1_Pin GPIO_PIN_5
#define M2_A1_GPIO_Port GPIOB
#define M2_A2_Pin GPIO_PIN_6
#define M2_A2_GPIO_Port GPIOB
#define M2_B1_Pin GPIO_PIN_7
#define M2_B1_GPIO_Port GPIOB
#define M2_B2_Pin GPIO_PIN_8
#define M2_B2_GPIO_Port GPIOB
#define HAL_1_Pin GPIO_PIN_9
#define HAL_1_GPIO_Port GPIOB
#define HAL_2_Pin GPIO_PIN_11
#define HAL_2_GPIO_Port GPIOB
#define HAL_3_Pin GPIO_PIN_10
#define HAL_3_GPIO_Port GPIOB
#define HAL_4_Pin GPIO_PIN_1
#define HAL_4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
