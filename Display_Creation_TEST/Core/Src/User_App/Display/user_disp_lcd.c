/* USER CODE BEGIN Header */
/*
 * user_disp.c
 *
 *  Created on: Mar 5, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_disp_lcd.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
/* USER CODE END Includes */

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
extern TIM_HandleTypeDef htim13;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void User_LCD_Init(void);
void User_LCD_BackLight_On(void);
void User_LCD_BackLight_Off(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void User_LCD_Init(void)
{
	/* Background Layer Initialization */
	BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER_LAYER0);

	/* Set Foreground Layer */
	BSP_LCD_SelectLayer(0);
	/* Clear the LCD Foreground layer */
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SetLayerVisible(1, DISABLE);
	BSP_LCD_SetLayerVisible(0, ENABLE);

	/* Configure the transparency for background : Increase the transparency */
	BSP_LCD_SetTransparency(0, 255);
	BSP_LCD_SetTransparency(1, 0);

	// Touch Test
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetFont(&Font24);
}

void User_LCD_BackLight_On(void)
{
	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
	TIM13->CCR1 = 99;
}

void User_LCD_BackLight_Off(void)
{
	HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);
	TIM13->CCR1 = 0;
}

/* USER CODE END 0 */
