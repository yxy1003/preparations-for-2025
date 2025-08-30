/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FDV_Pin GPIO_PIN_4
#define FDV_GPIO_Port GPIOA
#define FD2_Pin GPIO_PIN_5
#define FD2_GPIO_Port GPIOA
#define FD1_Pin GPIO_PIN_6
#define FD1_GPIO_Port GPIOA
#define FD0_Pin GPIO_PIN_7
#define FD0_GPIO_Port GPIOA
#define CLK_Pin GPIO_PIN_0
#define CLK_GPIO_Port GPIOB
#define LE_Pin GPIO_PIN_1
#define LE_GPIO_Port GPIOB
#define CE_Pin GPIO_PIN_10
#define CE_GPIO_Port GPIOB
#define CTL_Pin GPIO_PIN_13
#define CTL_GPIO_Port GPIOB
#define DAT_Pin GPIO_PIN_14
#define DAT_GPIO_Port GPIOB
#define MUX_Pin GPIO_PIN_15
#define MUX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
