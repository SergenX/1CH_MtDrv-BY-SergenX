/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define RUN_Pin GPIO_PIN_14
#define RUN_GPIO_Port GPIOC
#define FAULT_Pin GPIO_PIN_15
#define FAULT_GPIO_Port GPIOC
#define ENC_B_Pin GPIO_PIN_0
#define ENC_B_GPIO_Port GPIOA
#define ENC_A_Pin GPIO_PIN_1
#define ENC_A_GPIO_Port GPIOA
#define CURRENT_Pin GPIO_PIN_2
#define CURRENT_GPIO_Port GPIOA
#define VOLTAGE_Pin GPIO_PIN_3
#define VOLTAGE_GPIO_Port GPIOA
#define TEMP_Pin GPIO_PIN_4
#define TEMP_GPIO_Port GPIOA
#define BRAKE_Pin GPIO_PIN_12
#define BRAKE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
