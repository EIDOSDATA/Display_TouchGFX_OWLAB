/*
 * user_stm32_tim.h
 *
 *  Created on: Mar 6, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_GENERAL_USER_STM32_TIM_H_
#define INC_USER_APP_GENERAL_USER_STM32_TIM_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
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
/* USER CODE BEGIN EFP */
extern void TIM_Init(TIM_HandleTypeDef *htim);

/* TIM 13 LCD BackLight */
extern void TIM13_LCD_Backlight_Bright_Control(uint8_t bright);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define TFTLCD_BACKLIGHT_DUTY(duty) __HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, duty)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_GENERAL_USER_STM32_TIM_H_ */

