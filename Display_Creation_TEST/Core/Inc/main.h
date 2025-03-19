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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
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
void MX_I2C1_Init(void);
void MX_IWDG1_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define uSD_Detect_Pin GPIO_PIN_13
#define uSD_Detect_GPIO_Port GPIOC
#define Buzzer_Pin GPIO_PIN_10
#define Buzzer_GPIO_Port GPIOF
#define LTDC_BL_CTRL_Pin GPIO_PIN_6
#define LTDC_BL_CTRL_GPIO_Port GPIOA
#define TIM1_CATHODE_PWM_Pin GPIO_PIN_1
#define TIM1_CATHODE_PWM_GPIO_Port GPIOB
#define TOUCH_INT_Pin GPIO_PIN_0
#define TOUCH_INT_GPIO_Port GPIOJ
#define EEPROM_I2C_SCL_Pin GPIO_PIN_10
#define EEPROM_I2C_SCL_GPIO_Port GPIOB
#define EEPROM_I2C_SDA_Pin GPIO_PIN_11
#define EEPROM_I2C_SDA_GPIO_Port GPIOB
#define TIM4_CH2_ENCODER1_Pin GPIO_PIN_12
#define TIM4_CH2_ENCODER1_GPIO_Port GPIOD
#define TIM4_CH2_ENCODER2_Pin GPIO_PIN_13
#define TIM4_CH2_ENCODER2_GPIO_Port GPIOD
#define TIM1_ANODE_PWM_Pin GPIO_PIN_10
#define TIM1_ANODE_PWM_GPIO_Port GPIOA
#define TOUCH_RESET_Pin GPIO_PIN_5
#define TOUCH_RESET_GPIO_Port GPIOB
#define TOUCH_I2C1_SCL_Pin GPIO_PIN_6
#define TOUCH_I2C1_SCL_GPIO_Port GPIOB
#define TOUCH_I2C1_SDA_Pin GPIO_PIN_7
#define TOUCH_I2C1_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define TOUCHGFX_ENABLED_MODE false /* true or false */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
