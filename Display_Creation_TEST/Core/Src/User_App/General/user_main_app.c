/* USER CODE BEGIN Header */
/*
 * main_app.c
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
#include "user_disp_touch.h"
#include "user_disp_lcd.h"
#include "user_sys_init.h"
#include "user_buzzer.h"
#include "user_gpio_ctrl.h" /* Unused In this file */

/* General :: System Control */
#include "user_stm32_tim.h"

/* Memory Folder Header ------------------------------*/
/* Memory */
#include "fatfs_storage.h"  /* Unused In this file */
#include "n25q512a.h"  /* Unused In this file */
#include "user_mem_eeprom_data.h"

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
#define TD_SCHED_HANDLE_PERIOD		1000

#define DISPLAY_HANDLE_PERIOD		100
#define ENCODER_PARAM_READ_PERIOD	100
#define ENCODER_STATE_HANDLE_PERIOD	10

#define DEF_TIM1_PSC				5000
#define DEF_TIM1_PERIOD				6994

#define ENCODER_MAX_PARAM			5000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

extern TIM_HandleTypeDef htim1;

extern TIM_HandleTypeDef htim4;

static uint16_t encoder_cnt1, encoder_cnt2;
static uint16_t encoder_diff;
static uint32_t gSchdule_tick_disp = 0;
static uint32_t gSchdule_tick_state = 0;
static uint32_t gSchdule_tick_enc = 0;

typedef enum
{
	SYS_STATE_INIT = 0x00, SYS_STATE_CCW, SYS_STATE_CW, SYS_STATE_ERROR
} encoder_sys_state_t;
encoder_sys_state_t cur_motor_dir;
uint16_t prev_motor_dir;

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
int16_t keyValue = 0;

/* Test Value ------------------------------ */
/*
 * Touch Screen Position Test Temporary Value
 * */
__IO uint8_t iwdg_clear = 1;
uint8_t test_start_toggle_f = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* UTILITY FUNCTION */
void hex2dec(uint32_t hex);
void User_Display_Loading_Sequence(void);

/* TEST FUNCTION */
void Test_EEPROM_Read(void);
void Test_Image_Print(void);
void TEST_Encoder_State(void);
void TEST_Encoder_Parameter_Read(void);
void TEST_Encoder_Parameter_Display(void);

/* USER CODE BEGIN PFP */
void User_System_Init(void);
void User_Main_App(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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

/* LCD Display - Loading Bar */
void User_Display_Loading_Sequence(void)
{
	/* Loading Page... Page 02 */
	//BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[1]); // pQSPI->LCD_IMAGE_BUFFER[1] == 0x90200000
	HAL_Delay(1000);
	/* Loading Page : Loading BAR Gage */
	for (int x_size = 0; x_size < 692; x_size++)
	{
		//LCD_NORD_JP1_isplay(0, 592, x_size, 7, 167, 464, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[11]); // HAL_Delay(1);
	}
	HAL_Delay(1000);
	/* END OF Loading Page... */
}

/* TEST FUNCTION --------------------*/
#if 0
/* EEPROM READ TEST */
void Test_EEPROM_Read(void)
{
	for (int i = 0; i < 31; i++)
	{
		//BSP_EEPROM_ReadBuffer((uint8_t*) &radial_out_16[0 + i], (114 + i * 1), 1);   // read EEPROM
		//BSP_EEPROM_ReadBuffer((uint8_t*) &radial_sensor_8[0 + i], (145 + i * 1), 1); // read EEPROM
	}
	__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 100); /// 백나이트 켜기
}

/* Image Print Test */
void Test_Image_Print(void)
{
	while (1)
	{
		for (int i = 0; i < 4; i++)
		{
			//BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[i]);
			HAL_Delay(3000);
			HAL_IWDG_Refresh(&hiwdg1);
		}
	}
}
#endif

/* Encoder State Function */
void TEST_Encoder_State(void)
{
	if (HAL_GetTick() - gSchdule_tick_state >= ENCODER_STATE_HANDLE_PERIOD)
	{
		/* CCW :: Encoder Value -- */
		if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4))
		{
			cur_motor_dir = SYS_STATE_CCW;
			/* Exception Handling */
			if ((encoder_cnt2 > encoder_cnt1) && (encoder_cnt2 - encoder_cnt1 < 100))
			{
				cur_motor_dir = SYS_STATE_ERROR;
			}
		}

		/* CW :: Encoder Value ++ */
		else
		{
			cur_motor_dir = SYS_STATE_CW;
			/* Exception Handling */
			if ((encoder_cnt1 > encoder_cnt2) && (encoder_cnt1 - encoder_cnt2 < 100))
			{
				cur_motor_dir = SYS_STATE_ERROR;
			}
		}

		/* State Update */
		if (cur_motor_dir != prev_motor_dir)
		{
			prev_motor_dir = cur_motor_dir;
		}
		gSchdule_tick_state = HAL_GetTick();
	}
}

/* Encoder Parameter Read Function */
void TEST_Encoder_Parameter_Read(void)
{
	if (HAL_GetTick() - gSchdule_tick_enc >= ENCODER_PARAM_READ_PERIOD)
	{
		/* Encoder Value :: Under Flow */
		if (prev_motor_dir == SYS_STATE_CCW && TIM4->CNT > 60000)
		{
			TIM4->CNT = 0;
			encoder_cnt1 = 0;
			encoder_cnt2 = 0;
		}
		else if (prev_motor_dir == SYS_STATE_CW && TIM4->CNT > 60000)
		{
			TIM4->CNT = 0;
			encoder_cnt1 = 0;
			encoder_cnt2 = 0;
		}

		/* Encoder Value :: Over Flow */
		if (prev_motor_dir == SYS_STATE_CW && TIM4->CNT > ENCODER_MAX_PARAM)
		{
			TIM4->CNT = ENCODER_MAX_PARAM - 1;
			encoder_cnt1 = ENCODER_MAX_PARAM - 1;
			encoder_cnt2 = ENCODER_MAX_PARAM - 1;
		}
		else if (prev_motor_dir == SYS_STATE_CCW && TIM4->CNT > ENCODER_MAX_PARAM)
		{
			TIM4->CNT = ENCODER_MAX_PARAM - 1;
			encoder_cnt1 = ENCODER_MAX_PARAM - 1;
			encoder_cnt2 = ENCODER_MAX_PARAM - 1;
		}
		else
		{
			encoder_cnt2 = TIM4->CNT;
		}

		/* MOTOR DIRECTION */
		switch (cur_motor_dir)
		{
		case SYS_STATE_CW:
			/* Up Counting :: CW */
			if (encoder_cnt2 >= encoder_cnt1)
			{
				encoder_diff = encoder_cnt2 - encoder_cnt1;
			}
			else
			{
				encoder_diff = (TIM4->ARR + encoder_cnt2) - encoder_cnt1;
			}
			break;
		case SYS_STATE_CCW:
			/* Down Counting :: CCW */
			if (encoder_cnt1 >= encoder_cnt2)
			{
				encoder_diff = encoder_cnt1 - encoder_cnt2;
			}
			else
			{
				encoder_diff = (TIM4->ARR + encoder_cnt1) - encoder_cnt2;
			}
			break;
		default:
			break;
		}
		if (encoder_diff != 0)
		{
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
			HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);

			__HAL_TIM_SET_PRESCALER(&htim1, DEF_TIM1_PSC-1);
			__HAL_TIM_SET_AUTORELOAD(&htim1, (DEF_TIM1_PERIOD + (encoder_cnt2 * 10)) - 1);
			//TIM1->CNT = 0;
			TIM1->CCR3 = ((DEF_TIM1_PERIOD + (encoder_cnt2 * 10)) / 2) - 1;

			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
		}

		encoder_cnt1 = TIM4->CNT;
		gSchdule_tick_enc = HAL_GetTick();
	}
}

/* Encoder Parameter Display Function */
void TEST_Encoder_Parameter_Display(void)
{
	if (HAL_GetTick() - gSchdule_tick_disp >= DISPLAY_HANDLE_PERIOD)
	{
		sprintf(print_str, "Encoder Direction : %5d", cur_motor_dir);
		BSP_LCD_DisplayStringAt(20, 90, (uint8_t*) print_str, CENTER_MODE);
		sprintf(print_str, "Encoder CNT1 = %5d", encoder_cnt1);
		BSP_LCD_DisplayStringAt(20, 110, (uint8_t*) print_str, CENTER_MODE);
		sprintf(print_str, "Encoder CNT2 = %5d", encoder_cnt2);
		BSP_LCD_DisplayStringAt(20, 130, (uint8_t*) print_str, CENTER_MODE);
		sprintf(print_str, "Encoder Chennel Diff = %5d", encoder_diff);
		BSP_LCD_DisplayStringAt(20, 150, (uint8_t*) print_str, CENTER_MODE);
		gSchdule_tick_disp = HAL_GetTick();
	}
}

/* User System Initialization */
void User_System_Init(void)
{
#if 0
	/* FUCK U BSP */
	/* SDRAM Initialization */
	User_SDRAM_Initialization_Sequence(&hsdram1, &command);

	User_TFTLCD_Init();
	BUZZER_Init();

	MX_SDMMC1_SD_Init();
	MX_FATFS_Init();

	User_Touch_Init();

	/* I2C2 EEPROM Initialization */
	BSP_EEPROM_Init(); /* I2C2 */
	EEPROM_DataInit();

	/*
	 * QSPI Initialization
	 * */
	User_QSPI_SaveData_To_FlashMemory();
	MX_QUADSPI_Init();
	BSP_QSPI_Init();
	BSP_QSPI_EnableMemoryMappedMode();


	/*
	 * IWDG TEST
	 * Determines if it is a Reset by the system Watchdog.
	 * If it's Reset by the Watchdog, Rrun it.
	 * */
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDG1RST) != RESET)
	{
		/* LCD Backlight On */
		TIM13_LCD_Backlight_Bright_Control(EepData.BacklightBright);
		BSP_LCD_DisplayStringAt(20, 150, (uint8_t*) "IWDG reset", CENTER_MODE);
		HAL_Delay(2000);
	}
	/* Clear reset flags anyway */
	__HAL_RCC_CLEAR_RESET_FLAGS();
	MX_IWDG1_Init();
#endif
}

/**
 * @brief  The application entry point.
 * @retval N/A
 */
void User_Main_App(void)
{
	/* System Init */
#if UNUSED
	User_System_Init();
#endif

#if (!TOUCHGFX_ENABLED_MODE)
	/* QSPI_NOR Memory -> Read -> LCD Image Print */
	/* LCD Display - Initialization Page 01 */
	BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[0]); // pQSPI->LCD_IMAGE_BUFFER[0] == 0x90000000
#endif
#if UNUSED
	/* LCD Display - Loading Page... Page 02 */
	HAL_Delay(3000);
	User_Display_Loading_Sequence();

	/* LCD Display - Main Page(Control) 03 */
	BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[2]);
#endif

#if 1
	/* PWN Start */
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

	/* Encoder Start */
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_2);
	encoder_cnt1 = TIM4->CNT;
	prev_motor_dir = SYS_STATE_INIT;
	cur_motor_dir = SYS_STATE_INIT;
#endif

#if UNUSED
	while (1)
	{
		if (iwdg_clear == 1)
		{
			/* Watch Dog Clear */
			HAL_IWDG_Refresh(&hiwdg1);
		}

		/* Touch Key Detection Function*/
		User_MainKey();

		HAL_Delay(10);
		GPIO_MOTOR_OFF();

		switch (keyValue)
		{
		case 1:
			/* RET -> CET Image Print */
			BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[3]);
			HAL_Delay(200);
			break;
		case 2:
			/* CET -> RET Image Print */
			BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[2]);
			HAL_Delay(200);
			break;
		case 3:
			/* Time Button Pressed */
			sprintf(print_str, "Time Button Pressed.");
			BSP_LCD_DisplayStringAt(500, 0, (uint8_t*) print_str, LEFT_MODE);
			HAL_Delay(200);
			break;
		case 4:
			/* START and STOP BTN Action */
			if (test_start_toggle_f == 1)
			{
				sprintf(print_str, "Start Button Pressed.");
				BSP_LCD_DisplayStringAt(500, 20, (uint8_t*) print_str, LEFT_MODE);
			}
			else
			{
				sprintf(print_str, "Stop Button Pressed.");
				BSP_LCD_DisplayStringAt(500, 20, (uint8_t*) print_str, LEFT_MODE);
			}

			HAL_Delay(200);
			break;

		default:
			break;
		}

	}/* END OF while (1) */
#endif
}

/* USER CODE BEGIN 0 */

