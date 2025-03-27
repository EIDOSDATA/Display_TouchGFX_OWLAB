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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void MX_I2C1_Init(void);
void MX_QUADSPI_Init(void);
void MX_IWDG1_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CON_JF2_PE2_GPIO_Analog_Pin GPIO_PIN_2
#define CON_JF2_PE2_GPIO_Analog_GPIO_Port GPIOE
#define CON_JF2_PE3_GPIO_Analog_Pin GPIO_PIN_3
#define CON_JF2_PE3_GPIO_Analog_GPIO_Port GPIOE
#define CON_JF2_PE4_GPIO_Analog_Pin GPIO_PIN_4
#define CON_JF2_PE4_GPIO_Analog_GPIO_Port GPIOE
#define CON_JF2_PE5_GPIO_Analog_Pin GPIO_PIN_5
#define CON_JF2_PE5_GPIO_Analog_GPIO_Port GPIOE
#define CON_JF2_PE6_GPIO_Analog_Pin GPIO_PIN_6
#define CON_JF2_PE6_GPIO_Analog_GPIO_Port GPIOE
#define CON_JF1_PI8_GPIO_Analog_Pin GPIO_PIN_8
#define CON_JF1_PI8_GPIO_Analog_GPIO_Port GPIOI
#define uSD_Detect_Pin GPIO_PIN_13
#define uSD_Detect_GPIO_Port GPIOC
#define CON_JF1_PC14_GPIO_Analog_Pin GPIO_PIN_14
#define CON_JF1_PC14_GPIO_Analog_GPIO_Port GPIOC
#define CON_JF1_PC15_GPIO_Analog_Pin GPIO_PIN_15
#define CON_JF1_PC15_GPIO_Analog_GPIO_Port GPIOC
#define CON_JF1_PI11_GPIO_Analog_Pin GPIO_PIN_11
#define CON_JF1_PI11_GPIO_Analog_GPIO_Port GPIOI
#define Buzzer_Pin GPIO_PIN_10
#define Buzzer_GPIO_Port GPIOF
#define CON_JF2_PC1_GPIO_Analog_Pin GPIO_PIN_1
#define CON_JF2_PC1_GPIO_Analog_GPIO_Port GPIOC
#define CON_JF2_PC2_C_GPIO_Analog_Pin GPIO_PIN_2
#define CON_JF2_PC2_C_GPIO_Analog_GPIO_Port GPIOC
#define CON_JF2_PC3_C_GPIO_Analog_Pin GPIO_PIN_3
#define CON_JF2_PC3_C_GPIO_Analog_GPIO_Port GPIOC
#define CON_JF1_PA1_GPIO_Analog_Pin GPIO_PIN_1
#define CON_JF1_PA1_GPIO_Analog_GPIO_Port GPIOA
#define CON_JF1_PH2_GPIO_Analog_Pin GPIO_PIN_2
#define CON_JF1_PH2_GPIO_Analog_GPIO_Port GPIOH
#define CON_JF1_PH3_GPIO_Analog_Pin GPIO_PIN_3
#define CON_JF1_PH3_GPIO_Analog_GPIO_Port GPIOH
#define CON_JF1_PH4_GPIO_Analog_Pin GPIO_PIN_4
#define CON_JF1_PH4_GPIO_Analog_GPIO_Port GPIOH
#define CON_JF1_PA3_GPIO_Analog_Pin GPIO_PIN_3
#define CON_JF1_PA3_GPIO_Analog_GPIO_Port GPIOA
#define CON_JF2_PA5_GPIO_Analog_Pin GPIO_PIN_5
#define CON_JF2_PA5_GPIO_Analog_GPIO_Port GPIOA
#define LTDC_BL_CTRL_Pin GPIO_PIN_6
#define LTDC_BL_CTRL_GPIO_Port GPIOA
#define CON_JF1_PA7_GPIO_Analog_Pin GPIO_PIN_7
#define CON_JF1_PA7_GPIO_Analog_GPIO_Port GPIOA
#define CON_JF2_PC5_GPIO_Analog_Pin GPIO_PIN_5
#define CON_JF2_PC5_GPIO_Analog_GPIO_Port GPIOC
#define CON_JF1_PB0_GPIO_Analog_Pin GPIO_PIN_0
#define CON_JF1_PB0_GPIO_Analog_GPIO_Port GPIOB
#define TIM1_CATHODE_PWM_Pin GPIO_PIN_1
#define TIM1_CATHODE_PWM_GPIO_Port GPIOB
#define TOUCH_INT_Pin GPIO_PIN_0
#define TOUCH_INT_GPIO_Port GPIOJ
#define EEPROM_I2C_SCL_Pin GPIO_PIN_10
#define EEPROM_I2C_SCL_GPIO_Port GPIOB
#define EEPROM_I2C_SDA_Pin GPIO_PIN_11
#define EEPROM_I2C_SDA_GPIO_Port GPIOB
#define CON_JF2_PB12_GPIO_Analog_Pin GPIO_PIN_12
#define CON_JF2_PB12_GPIO_Analog_GPIO_Port GPIOB
#define CON_JF2_PB13_GPIO_Analog_Pin GPIO_PIN_13
#define CON_JF2_PB13_GPIO_Analog_GPIO_Port GPIOB
#define CON_JF2_PB14_GPIO_Analog_Pin GPIO_PIN_14
#define CON_JF2_PB14_GPIO_Analog_GPIO_Port GPIOB
#define CON_JF2_PB15_GPIO_Analog_Pin GPIO_PIN_15
#define CON_JF2_PB15_GPIO_Analog_GPIO_Port GPIOB
#define CON_JF2_PD11_GPIO_Analog_Pin GPIO_PIN_11
#define CON_JF2_PD11_GPIO_Analog_GPIO_Port GPIOD
#define TIM4_CH2_ENCODER1_Pin GPIO_PIN_12
#define TIM4_CH2_ENCODER1_GPIO_Port GPIOD
#define TIM4_CH2_ENCODER2_Pin GPIO_PIN_13
#define TIM4_CH2_ENCODER2_GPIO_Port GPIOD
#define CON_JF1_PG7_GPIO_Analog_Pin GPIO_PIN_7
#define CON_JF1_PG7_GPIO_Analog_GPIO_Port GPIOG
#define CON_JF1_CON_J2_PC6_USART6_TX_Pin GPIO_PIN_6
#define CON_JF1_CON_J2_PC6_USART6_TX_GPIO_Port GPIOC
#define CON_J2_PC7_USART6_RX_Pin GPIO_PIN_7
#define CON_J2_PC7_USART6_RX_GPIO_Port GPIOC
#define CON_JF1_PA9_GPIO_Analog_Pin GPIO_PIN_9
#define CON_JF1_PA9_GPIO_Analog_GPIO_Port GPIOA
#define TIM1_ANODE_PWM_Pin GPIO_PIN_10
#define TIM1_ANODE_PWM_GPIO_Port GPIOA
#define CON_JF2_PD3_GPIO_Analog_Pin GPIO_PIN_3
#define CON_JF2_PD3_GPIO_Analog_GPIO_Port GPIOD
#define CON_JF2_PD4_GPIO_Analog_Pin GPIO_PIN_4
#define CON_JF2_PD4_GPIO_Analog_GPIO_Port GPIOD
#define CON_JF2_PD5_GPIO_Analog_Pin GPIO_PIN_5
#define CON_JF2_PD5_GPIO_Analog_GPIO_Port GPIOD
#define CON_JF2_PD6_GPIO_Analog_Pin GPIO_PIN_6
#define CON_JF2_PD6_GPIO_Analog_GPIO_Port GPIOD
#define CON_JF2_PD7_GPIO_Analog_Pin GPIO_PIN_7
#define CON_JF2_PD7_GPIO_Analog_GPIO_Port GPIOD
#define CON_JF1_PG9_GPIO_Analog_Pin GPIO_PIN_9
#define CON_JF1_PG9_GPIO_Analog_GPIO_Port GPIOG
#define CON_J1_PG10_GPIO_Analog_Pin GPIO_PIN_10
#define CON_J1_PG10_GPIO_Analog_GPIO_Port GPIOG
#define CON_J1_PG11_GPIO_Analog_Pin GPIO_PIN_11
#define CON_J1_PG11_GPIO_Analog_GPIO_Port GPIOG
#define CON_JF1_PG12_SPI6_MISO_Pin GPIO_PIN_12
#define CON_JF1_PG12_SPI6_MISO_GPIO_Port GPIOG
#define CON_JF1_PG13_SPI6_SCK_Pin GPIO_PIN_13
#define CON_JF1_PG13_SPI6_SCK_GPIO_Port GPIOG
#define CON_JF1_PG14_SPI6_MOSI_Pin GPIO_PIN_14
#define CON_JF1_PG14_SPI6_MOSI_GPIO_Port GPIOG
#define TOUCH_RESET_Pin GPIO_PIN_5
#define TOUCH_RESET_GPIO_Port GPIOB
#define TOUCH_I2C1_SCL_Pin GPIO_PIN_6
#define TOUCH_I2C1_SCL_GPIO_Port GPIOB
#define TOUCH_I2C1_SDA_Pin GPIO_PIN_7
#define TOUCH_I2C1_SDA_GPIO_Port GPIOB
#define CON_JF2_PB8_GPIO_Analog_Pin GPIO_PIN_8
#define CON_JF2_PB8_GPIO_Analog_GPIO_Port GPIOB
#define CON_JF2_PB9_GPIO_Analog_Pin GPIO_PIN_9
#define CON_JF2_PB9_GPIO_Analog_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define TOUCHGFX_ENABLED_MODE		false /* true or false */
#define LCD_POSITION_REVERSED		true
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
