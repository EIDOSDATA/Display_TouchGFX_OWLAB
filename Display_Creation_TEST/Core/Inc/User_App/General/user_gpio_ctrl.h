/*
 * user_gpio_ctrl.h
 *
 *  Created on: Mar 5, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_USER_GPIO_CTRL_H_
#define INC_USER_APP_USER_GPIO_CTRL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32h7xx_hal.h"

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#if 0
#define GPIO_MOTOR_OFF()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET)
#define GPIO_MOTOR_ON()    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET)

#define GPIO_OUT_V_ON()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET)
#define GPIO_OUT_V_OFF()    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET)

#define GPIO_A_G_ON()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)    // 라디알 핸드피스 솔벨브 ON
#define GPIO_A_G_OFF()    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)  // 라디알 핸드피스 솔벨브 OFF

#define GPIO_MOTOR_RELAY_ON()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET)
#define GPIO_MOTOR_RELAY_OFF()    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET)

/* -------------------------------------------------- What is This? -------------------------------------------------- */
#define GPIO_PULSE_ON()     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET)
#define GPIO_PULSE_OFF()    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET)

#define GPIO_PULSE_ATV_ON()     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET)
#define GPIO_PULSE_ATV_OFF()    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET)

#define GPIO_PULSE_RELAY_ON()     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET)
#define GPIO_PULSE_RELAY_OFF()    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET)

/*
 * stm32_gpio.h Copy
 * */
#if 0
#define GPIO1_HIGH()     HAL_GPIO_WritePin(GPIO_OUT_1_GPIO_Port, GPIO_OUT_1_Pin, GPIO_PIN_SET)
#define GPIO1_LOW()      HAL_GPIO_WritePin(GPIO_OUT_1_GPIO_Port, GPIO_OUT_1_Pin, GPIO_PIN_RESET)
#define GPIO1_TOGGLE()   HAL_GPIO_TogglePin(GPIO_OUT_1_GPIO_Port, GPIO_OUT_1_Pin)
#endif
#define GPIO2_HIGH()     HAL_GPIO_WritePin(GPIO_OUT_2_GPIO_Port, GPIO_OUT_2_Pin, GPIO_PIN_SET)
#define GPIO2_LOW()      HAL_GPIO_WritePin(GPIO_OUT_2_GPIO_Port, GPIO_OUT_2_Pin, GPIO_PIN_RESET)
#define GPIO2_TOGGLE()   HAL_GPIO_TogglePin(GPIO_OUT_2_GPIO_Port, GPIO_OUT_2_Pin)

#define GPIO3_HIGH()     HAL_GPIO_WritePin(GPIO_OUT_3_GPIO_Port, GPIO_OUT_3_Pin, GPIO_PIN_SET)
#define GPIO3_LOW()      HAL_GPIO_WritePin(GPIO_OUT_3_GPIO_Port, GPIO_OUT_3_Pin, GPIO_PIN_RESET)
#define GPIO3_TOGGLE()   HAL_GPIO_TogglePin(GPIO_OUT_3_GPIO_Port, GPIO_OUT_3_Pin)

#define GPIO4_HIGH()     HAL_GPIO_WritePin(GPIO_OUT_4_GPIO_Port, GPIO_OUT_4_Pin, GPIO_PIN_SET)
#define GPIO4_LOW()      HAL_GPIO_WritePin(GPIO_OUT_4_GPIO_Port, GPIO_OUT_4_Pin, GPIO_PIN_RESET)
#define GPIO4_TOGGLE()   HAL_GPIO_TogglePin(GPIO_OUT_4_GPIO_Port, GPIO_OUT_4_Pin)

#define GPIO5_HIGH()     HAL_GPIO_WritePin(GPIO_OUT_5_GPIO_Port, GPIO_OUT_5_Pin, GPIO_PIN_SET)
#define GPIO5_LOW()      HAL_GPIO_WritePin(GPIO_OUT_5_GPIO_Port, GPIO_OUT_5_Pin, GPIO_PIN_RESET)
#define GPIO5_TOGGLE()   HAL_GPIO_TogglePin(GPIO_OUT_5_GPIO_Port, GPIO_OUT_5_Pin)

#if 0
#define BUZZER_ON()      HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET)
#define BUZZER_OFF()     HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET)
#endif

#define TP1_HIGH()       HAL_GPIO_WritePin(GPIOI, GPIO_PIN_8, GPIO_PIN_SET)
#define TP1_LOW()        HAL_GPIO_WritePin(GPIOI, GPIO_PIN_8, GPIO_PIN_RESET)
#define TP1_TOGGLE()     HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_8)

#define FAN_ON()         HAL_GPIO_WritePin(GPIO_OUT_1_GPIO_Port, GPIO_OUT_1_Pin, GPIO_PIN_SET)
#define FAN_OFF()        HAL_GPIO_WritePin(GPIO_OUT_1_GPIO_Port, GPIO_OUT_1_Pin, GPIO_PIN_RESET)
#define GPIO1_TOGGLE()   HAL_GPIO_TogglePin(GPIO_OUT_1_GPIO_Port, GPIO_OUT_1_Pin)

//방전 저항 SCR
#define SCR2_HIGH()      GPIOA->BSRR = GPIO_PIN_7;
#define SCR2_LOW()       GPIOA->BSRR = (uint32_t)GPIO_PIN_7 << (16U);

#endif
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_USER_GPIO_CTRL_H_ */
