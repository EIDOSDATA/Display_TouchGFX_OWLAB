/* USER CODE BEGIN Header */
/*
 * user_mem_eeprom_system.c
 *
 *  Created on: Mar 5, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_mem_eeprom_data.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "user_main_app.h"
#include "user_disp_lcd.h"
#include "string.h"

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

/*
 * Read selected memory address group :: EEPROM
 * */
void User_Main_Read_Settings_From_EEPROM(void);

void User_Save_HandpieceUsageCountToEEPROM(void);
void User_Read_EEPROM_Settings(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
 * Read selected memory address group :: EEPROM
 * */
void User_Main_Read_Settings_From_EEPROM(void)
{
	/* 선택된 메모리 주소 그룹 읽어오기 */
	/* read sensor EEPROM */
#if 0
	for (int r_addr = 0; r_addr < 31; r_addr++)
	{
		BSP_EEPROM_ReadBuffer((uint8_t*) &radial_sensor_8[0 + r_addr], (114 + r_addr * 1), 1);
		HAL_Delay(1);
	}
	/* read out EEPROM */
	for (int r_addr_y = 0; r_addr_y < 15; r_addr_y++)
	{
		int base_index = 31 * r_addr_y;
		int eeprom_base_addr = (145 + base_index) * 2;

		for (int r_addr_x = 0; r_addr_x < 31; r_addr_x++)
		{
			int index = r_addr_x + base_index;
			BSP_EEPROM_ReadBuffer((uint8_t*) &radial_out_16[index], (eeprom_base_addr + r_addr_x * 2), 2);
			HAL_Delay(1);
		}
	}
#endif
}

/*
 * Handpiece Usage Counter :: Save To EEPROM
 * */
void User_Save_HandpieceUsageCountToEEPROM(void)
{
	uint32_t total_count = 0;
	char str_init[4] =
	{ 0, };

	BSP_EEPROM_ReadBuffer((uint8_t*) str_init, 8008, 4); // Read EEPROM

	// 8008~8011번지 값이 "CCCC"값과 맞지 않으면 0으로 초기화.
	if (strncmp((const char*) str_init, "CCCC", 4) != 0)
	{
		BSP_EEPROM_WriteBuffer((uint8_t*) &total_count, (4 * 0), 4);
		BSP_EEPROM_WriteBuffer((uint8_t*) &total_count, (4 * 1), 4);
		BSP_EEPROM_WriteBuffer((uint8_t*) &total_count, (4 * 2), 4);
		BSP_EEPROM_WriteBuffer((uint8_t*) &total_count, (4 * 3), 4);

		strncpy(str_init, "CCCC", 4);
		BSP_EEPROM_WriteBuffer((uint8_t*) str_init, 8008, 4); // Write EEPROM
	}
}

/*
 * Read the value set in EEPROM
 * */
void User_Read_EEPROM_Settings(void)
{
#if 0
	uint16_t disp_y_pos = 0, disp_x_pos = 0;

	for (int r_addr_x = 0; r_addr_x < 31; r_addr_x++)
	{
		BSP_EEPROM_ReadBuffer((uint8_t*) &radial_sensor_8[0 + r_addr_x], (114 + r_addr_x) * 1, 1); // read sensor EEPROM
		if (radial_sensor_8[r_addr_x] > 250)
		{
			radial_sensor_8[r_addr_x] = 250;
		}

		if (r_addr_x < 10)
		{
			disp_y_pos = r_addr_x;
			disp_x_pos = 0;
		}
		else if (r_addr_x < 20)
		{
			disp_y_pos = r_addr_x - 10;
			disp_x_pos = 280;
		}
		else if (r_addr_x < 30)
		{
			disp_y_pos = r_addr_x - 20;
			disp_x_pos = 560;
		}
		else
		{
			disp_y_pos = r_addr_x - 30 + 1;
			disp_x_pos = 730;
		}

#if 0
		if (radial_hz >= 6 && radial_hz <= 16 && r_addr_x > (31 - radial_hz))
		{
			sprintf(print_str, "      ");
		}
		else
		{
			sprintf(print_str, "S=%4d", radial_sensor_8[r_addr_x]);
		}
#else
		sprintf(print_str, "S=%4d", radial_sensor_8[r_addr_x]);
#endif
		BSP_LCD_DisplayStringAt(165 + disp_x_pos, 81 + 52 * disp_y_pos, (uint8_t*) print_str, LEFT_MODE);
		HAL_Delay(1);
	} /* End of for() */

	for (int r_addr_x = 0; r_addr_x < 31; r_addr_x++)
	{
		int base_index = 31;

#if 0
		int r_addr_y = base_index * (radial_hz - 1);
#else
		int r_addr_y = base_index;
#endif
		int index = r_addr_x + r_addr_y;
		int read_addr = 145 + index;

		BSP_EEPROM_ReadBuffer((uint8_t*) &radial_out_16[index], (read_addr) * 2, 2); // read out EEPROM

		radial_out_16[index] = (radial_out_16[index] > 1000) ? 1000 : radial_out_16[index];

		if (r_addr_x < 10)
		{
			disp_y_pos = r_addr_x;
			disp_x_pos = 0;
		}
		else if (r_addr_x < 20)
		{
			disp_y_pos = r_addr_x - 10;
			disp_x_pos = 280;
		}
		else if (r_addr_x < 30)
		{
			disp_y_pos = r_addr_x - 20;
			disp_x_pos = 560;
		}
		else
		{
			disp_y_pos = r_addr_x - 30 + 1;
			disp_x_pos = 730;
		}

#if 0
		if (radial_hz >= 6 && radial_hz <= 16 && r_addr_x > (31 - radial_hz))
		{
			sprintf(print_str, "      ");
		}
		else
		{
			sprintf(print_str, "O=%4d", radial_out_16[r_addr_x + r_addr_y]);
		}
#else
		sprintf(print_str, "O=%4d", radial_out_16[r_addr_x + r_addr_y]);
#endif

		BSP_LCD_DisplayStringAt(165 + disp_x_pos, 59 + 52 * disp_y_pos, (uint8_t*) print_str, LEFT_MODE);
		HAL_Delay(1);
	} /* End of for() */

#endif
}

/* USER CODE END 0 */
