/* USER CODE BEGIN Header */
/*
 * user_stm32_tim.h
 *
 *  Created on: Mar 6, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_stm32_tim.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */
#include "main_app.h"

#include "user_gpio_ctrl.h"
#include "user_buzzer.h"

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void TIM_Init(TIM_HandleTypeDef *htim);

/* TIM 13 LCD BackLight */
void TIM13_LCD_Backlight_Bright_Control(uint8_t bright);

/* Radial and Focus Timer Configuration */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void TIM_Init(TIM_HandleTypeDef *htim)
{
}

/*
 * Timer 13 :: Adjust LCD Backlight Brightness.
 * */
void TIM13_LCD_Backlight_Bright_Control(uint8_t bright)
{
	TFTLCD_BACKLIGHT_DUTY(40 + (6 * bright));
}

/* USER CODE END 0 */

