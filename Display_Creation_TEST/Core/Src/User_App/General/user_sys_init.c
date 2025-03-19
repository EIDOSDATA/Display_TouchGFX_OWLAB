/* USER CODE BEGIN Header */
/*
 * user_sys_init.c
 *
 *  Created on: Mar 5, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* Display Folder Header ------------------------------*/
/* Display :: User Interface */

#include "user_disp_touch.h"
#include "user_disp_lcd.h"

/* General Folder Header ------------------------------*/
/* General :: Main Ssystem */
#include "main_app.h"

/* General :: System Base */
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void User_System_Init(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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

/* USER CODE END 0 */
