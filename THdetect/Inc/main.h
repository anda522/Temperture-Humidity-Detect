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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
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
#define SCK_LCD_Pin GPIO_PIN_13
#define SCK_LCD_GPIO_Port GPIOB
#define DC_Pin GPIO_PIN_14
#define DC_GPIO_Port GPIOB
#define SDA_LCD_Pin GPIO_PIN_15
#define SDA_LCD_GPIO_Port GPIOB
#define BL_Pin GPIO_PIN_8
#define BL_GPIO_Port GPIOC
#define SCL_Pin GPIO_PIN_11
#define SCL_GPIO_Port GPIOA
#define SDA_Pin GPIO_PIN_12
#define SDA_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_6
#define CS_GPIO_Port GPIOB

#define D0_Pin GPIO_PIN_0
#define D0_GPIO_Port GPIOC
#define D1_Pin GPIO_PIN_1
#define D1_GPIO_Port GPIOC
#define D2_Pin GPIO_PIN_2
#define D2_GPIO_Port GPIOC
#define D3_Pin GPIO_PIN_3
#define D3_GPIO_Port GPIOC
#define RS_Pin GPIO_PIN_1
#define RS_GPIO_Port GPIOA
#define RW_Pin GPIO_PIN_2
#define RW_GPIO_Port GPIOA
#define E_Pin GPIO_PIN_3
#define E_GPIO_Port GPIOA
#define D4_Pin GPIO_PIN_4
#define D4_GPIO_Port GPIOC
#define D5_Pin GPIO_PIN_5
#define D5_GPIO_Port GPIOC
#define D6_Pin GPIO_PIN_6
#define D6_GPIO_Port GPIOC
#define D7_Pin GPIO_PIN_7
#define D7_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
typedef  	uint8_t u8;
typedef  	uint16_t u16;
typedef  	uint32_t u32;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
