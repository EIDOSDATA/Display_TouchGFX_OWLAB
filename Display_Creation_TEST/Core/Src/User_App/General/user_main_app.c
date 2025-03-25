/* USER CODE BEGIN Header */
/*
 * user_main_app.c
 *
 *  Created on: Mar 6, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_main_app.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"

/* Display :: Touch Screen */
#include "user_disp_touch.h"
#include "user_disp_lcd.h"
#include "user_disp_touch_key.h"

/* General :: System Control */
#include "user_buzzer.h"
#include "user_encoder.h"
#include "user_gpio_ctrl.h" /* Unused In this file */
#include "user_stm32_tim.h"

/*
 * Memory :: External EEPROM, NOR Flash, SDRAM, SDCARD
 * 	- EEPROM : I2C
 * 	- NOR FLASH : QSPI
 * 	- SDRAM : FMC
 * 	- SDCARD : SDMMC
 * */
#include "n25q512a.h" /* Unused In this file */
#include "user_mem_sdcard.h" /* Unused In this file */
#include "user_mem_eeprom.h"
#include "user_mem_sdram.h"
#include "user_mem_qspi.h"

/* Stimulation Folder Header ------------------------------*/
/* Stimulation */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TD_SCHED_HANDLE_PERIOD				1000

#define TOUCH_KEY_DETECTION_HANDLE_PERIOD					20
#define TOUCH_KEY_IMAGE_DISPLAY_CHANGE_HANDLE_PERIOD		200

#define IMAGE_DISPLAY_HANDLE_PERIOD			2000

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;

static uint32_t gSchdule_tick_touch_detect = 0;
static uint32_t gSchdule_tick_touch_image_change = 0;
static uint32_t gSchdule_tick_image = 0;

#if 0
_SDRAM_ADDRESS_t* pSD;
#endif

/*
 * Message Print Stinrg
 * */
char print_str[100];

/*
 * NOTE : Hexa to Decimal / 1 to 100M
 * Decimal Value
 * */
DecimalDigits digits =
{ 0 };

/*
 * Touch Screen Control Key Value
 * */
extern uint8_t g_key_start_toggle_f;

/* Test Value ------------------------------ */
/*
 * Touch Screen Position Test Temporary Value
 * */
__IO uint8_t iwdg_clear = 1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* TEST FUNCTION */
void Test_EEPROM_Read(void);
void Test_Touch_Key_Event(void);
void Test_Image_Print(void);

/* UTILITY FUNCTION */
void hex2dec(uint32_t hex);
void User_Display_Loading_Sequence(void);

void User_System_Init(void);
void User_Main_App(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* TEST FUNCTION --------------------*/
#if 0
/* EEPROM READ TEST */
void Test_EEPROM_Read(void)
{
	for (int i = 0; i < 31; i++)
	{
		BSP_EEPROM_ReadBuffer((uint8_t*) &radial_out_16[0 + i], (114 + i * 1), 1);   // read EEPROM
		BSP_EEPROM_ReadBuffer((uint8_t*) &radial_sensor_8[0 + i], (145 + i * 1), 1); // read EEPROM
	}
	__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 100); /// 백나이트 켜기
}
#endif

/* Test the BMP image touch output on the display. :: NON Touch GFX */
void Test_Touch_Key_Event(void)
{
	/* Touch Key Detection Function*/

	if (HAL_GetTick() - gSchdule_tick_touch_detect >= TOUCH_KEY_DETECTION_HANDLE_PERIOD)
	{
		/* Parameter Format Time Schedule */
		for (int i = 0; i < USER_TOUCH_BTN_MAX; i++)
		{
			touch_btns[i].prev_state = TOUCH_BTN_STATE_RELEASED;
		}
		User_Main_Touch_Key();
		gSchdule_tick_touch_detect = HAL_GetTick();
	}

	if (HAL_GetTick() - gSchdule_tick_touch_image_change >= TOUCH_KEY_IMAGE_DISPLAY_CHANGE_HANDLE_PERIOD)
	{
		User_ProcessButton_Actions();
		gSchdule_tick_touch_image_change = HAL_GetTick();
	}
}

/* Image Print Test */
void Test_Image_Print(void)
{
	static uint8_t i = 0;

	if (HAL_GetTick() - gSchdule_tick_image >= IMAGE_DISPLAY_HANDLE_PERIOD)
	{
		BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[i]);
		i++;
		if (i == 4)
		{
			i = 0;
		}
		gSchdule_tick_image = HAL_GetTick();
	}
}

/* Encoder State Function */

/* Utility Function --------------------*/
/* Hexa to Decimal  */
void hex2dec(uint32_t hex)
{
	DEC_100M = (hex) / 100000000;
	DEC_10M = (hex % 100000000) / 10000000;
	DEC_1M = (hex % 10000000) / 1000000;
	DEC_100K = (hex % 1000000) / 100000;
	DEC_10K = (hex % 100000) / 10000;
	DEC_1K = (hex % 10000) / 1000;
	DEC_100 = (hex % 1000) / 100;
	DEC_10 = (hex % 100) / 10;
	DEC_1 = hex % 10;
}

/* LCD Display - Loading Sequence */
void User_Display_Loading_Sequence(void)
{
	/* QSPI_NOR Memory -> Read -> LCD Image Print */
	/* LCD Display - Initialization Page 01 */
	BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[0]); // pQSPI->LCD_IMAGE_BUFFER[0] == 0x90000000
	HAL_Delay(1000);

	/* LCD Display - Loading Page... Page 02 */
	BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[1]); // pQSPI->LCD_IMAGE_BUFFER[1] == 0x90200000
	HAL_Delay(1000);
	/* Loading Page : Loading BAR Gage */
	for (int x_size = 0; x_size < 692; x_size++)
	{
		LCD_NORD_JP1_isplay(0, 592, x_size, 7, 167, 464, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[11]); // HAL_Delay(1);
	}
	HAL_Delay(1000);

	/* LCD Display - Main Page(Control) 03 */
	BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[2]);
}

/* User System Initialization */
void User_System_Init(void)
{
}

/**
 * @brief  The application entry point.
 * @retval N/A
 */
void User_Main_App(void)
{
	/* System Init */
	User_System_Init();

#if (!TOUCHGFX_ENABLED_MODE)
	/* QSPI_NOR Memory -> Read -> LCD Image Print */
	User_Display_Loading_Sequence();
#endif

	/* PWN Start */
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

	User_Encoder_Start();

	/* Key Parameter Init */
	for (int i = 0; i < USER_TOUCH_BTN_MAX; i++)
	{
		touch_btns[i].prev_state = touch_btns[i].cur_state;
	}

}

/* USER CODE BEGIN 0 */

