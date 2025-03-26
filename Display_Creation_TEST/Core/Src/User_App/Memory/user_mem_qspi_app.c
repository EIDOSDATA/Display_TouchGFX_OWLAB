/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    BSP/Src/qspi.c
 * @author  MCD Application Team
 * @brief   This example code shows how to use the QSPI Driver
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/*
 * user_mem_qspi_app.c
 *
 *  Created on: Mar 25, 2025
 *      Author: user
 */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "user_mem_qspi.h"
#include "fatfs.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user_main_app.h"
#include "user_stm32_tim.h"

#include "user_disp_lcd.h"
#include "user_disp_touch.h"

#include "user_mem_sdcard.h"
#include "user_mem_sdram.h"
#include "user_mem_eeprom.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#if 0
#define BUFFER_SIZE         ((uint32_t)0x0200)
#define WRITE_READ_ADDR     ((uint32_t)0x0050)
#endif
#define QSPI_DEVICE_ADDR ((uint32_t)0x90000000)

/*
 * TODO : CHECK PLZ
 * MAXIMUM_NUM_OF_FILES : 32
 * */
#define MAXIMUM_NUM_OF_FILES		4
#define MAXIMUM_MEM_SECTOR_SIZE		32

#define MAXIMUM_FILE_NAME_LENGTH	100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern IWDG_HandleTypeDef hiwdg1;
extern QSPI_HandleTypeDef hqspi;

_QSPI_ADDRESS_t *pQSPI = (_QSPI_ADDRESS_t*) QSPI_DEVICE_ADDR;

#if 0
uint8_t qspi_aTxBuffer[BUFFER_SIZE];
uint8_t qspi_aRxBuffer[BUFFER_SIZE];
#endif

/*
 * NOTE :
 * 	Responsible for the file name to be saved,
 * 	the number of files,
 * 	and the number of characters in the file name.
 * */
static char sd_name[MAXIMUM_NUM_OF_FILES][MAXIMUM_FILE_NAME_LENGTH] =
{
#if 1
		{ "OW4400_TEST/00_page0.bmp" },                         // 0
		{ "OW4400_TEST/01_page1_Loading.bmp" },                 // 1
		{ "OW4400_TEST/02_page2_RET.bmp" },                     // 2
		{ "OW4400_TEST/03_page2_CET.bmp" },                     // 3
#else

		{ "JISAN_IMAGES/image00.bmp" },           // 0
		{ "JISAN_IMAGES/image01.bmp" },           // 1
		{ "JISAN_IMAGES/image02.bmp" },           // 2
		{ "JISAN_IMAGES/image03.bmp" },           // 3
		{ "JISAN_IMAGES/image04.bmp" },           // 4
		{ "JISAN_IMAGES/image05.bmp" },           // 5
#endif

		};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void User_QSPI_Init(void);

void User_QSPI_SaveData_To_FlashMemory(void);
static void User_QSPI_Update_FlashMemory_Data(void);
static void QSPI_SetHint(void);
static uint8_t User_QSPI_Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint32_t BufferLength);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * User QSPI Initialization Sequence
 * */
void User_QSPI_Init(void)
{
#if 0
	/* OLD CODE */
	QSPI_ChkDataSave();
	BSP_QSPI_Init();
	BSP_QSPI_EnableMemoryMappedMode();
#endif
	User_QSPI_SaveData_To_FlashMemory();
	MX_QUADSPI_Init();
	User_QSPI_EnableMemoryMappedMode(&hqspi);
}

/*
 * External Memory(NOR Flash) :: Data Save Check
 * */
void User_QSPI_SaveData_To_FlashMemory(void)
{
	char print_str[100];
	/*
	 * NOTE :
	 * 	QSPI(NOR Flash) Save Test.
	 * 	When the LCD is touched, it is stored in external memory by QSPI communication.
	 * */
	uint16_t cnt = 0;

	if (BSP_SD_IsDetected() == SD_PRESENT)
	{
		/* Backlight ON */
		User_LCD_BackLight_On();
#if 0
		LCD_ON();
		TIM13_LCD_Backlight_Bright_Control(EepData.BacklightBright);
#endif
		BSP_LCD_DisplayStringAt(20, 150, (uint8_t*) "Touch to save QSPI Flash", CENTER_MODE);

		while (cnt < 100)
		{
			HAL_IWDG_Refresh(&hiwdg1);
			HAL_Delay(50);

			if (User_TS_GetState(&Touch_State) == 1)
			{
				User_QSPI_Update_FlashMemory_Data();
				break;
			}

			sprintf((char*) print_str, "Waiting for %2dsec", (5 - (cnt / 20)));
			BSP_LCD_DisplayStringAt(20, 200, (uint8_t*) print_str, CENTER_MODE);

			++cnt;
		}
	}
}

/**
 * @brief  QSPI DataSave
 * @param  None
 * @retval None
 */
static void User_QSPI_Update_FlashMemory_Data(void)
{
	/* QSPI info structure */
	static User_QSPI_Info pQSPI_Info;
	uint32_t status;

	QSPI_SetHint();

	while (User_TS_GetState(&Touch_State) == 1)
	{
		/* Watchdog clear  */
		HAL_IWDG_Refresh(&hiwdg1);
	}

	/*##-1- Configure the QSPI device ##########################################*/
	/* QSPI device configuration */
#if 1
	status = User_QSPI_Startup(0);
#else
	status = BSP_QSPI_Init();
#endif

	if (status == QSPI_MEMORY_NOT_SUPPORTED)
	{
		BSP_LCD_DisplayStringAt(20, 100, (uint8_t*) "QSPI Initialization : FAILED.", LEFT_MODE);
		BSP_LCD_DisplayStringAt(20, 125, (uint8_t*) "QSPI Writing Aborted.", LEFT_MODE);
	}
	else if (status == MEMORY_ERROR)
	{
		BSP_LCD_DisplayStringAt(20, 100, (uint8_t*) "QSPI Initialization : FAILED.", LEFT_MODE);
		BSP_LCD_DisplayStringAt(20, 125, (uint8_t*) "QSPI Writing Aborted.", LEFT_MODE);
	}
	else
	{
		BSP_LCD_DisplayStringAt(20, 100, (uint8_t*) "QSPI Initialization : OK.", LEFT_MODE);

		/*##-2- Read & check the QSPI info #######################################*/
		/* Initialize the structure */
		pQSPI_Info.FlashSize = (uint32_t) 0x00;
		pQSPI_Info.EraseSectorSize = (uint32_t) 0x00;
		pQSPI_Info.EraseSectorsNumber = (uint32_t) 0x00;
		pQSPI_Info.ProgPageSize = (uint32_t) 0x00;
		pQSPI_Info.ProgPagesNumber = (uint32_t) 0x00;

		/* Read the QSPI memory info */
		User_QSPI_GetInfo(&pQSPI_Info);

		/* Test the correctness */
		if ((pQSPI_Info.FlashSize != 0x4000000) || (pQSPI_Info.EraseSectorSize != 0x1000) || (pQSPI_Info.ProgPageSize != 0x100) || (pQSPI_Info.EraseSectorsNumber != 16384) || (pQSPI_Info.ProgPagesNumber != 262144))
		{
			BSP_LCD_DisplayStringAt(20, 125, (uint8_t*) "QSPI GET INFO : FAILED.", LEFT_MODE);
			BSP_LCD_DisplayStringAt(20, 150, (uint8_t*) "QSPI Writing Aborted.", LEFT_MODE);
		}
		else
		{
			BSP_LCD_DisplayStringAt(20, 125, (uint8_t*) "QSPI GET INFO : OK.   ", LEFT_MODE);

			uint8_t fail = 0;
			uint8_t qspi_print_str[100];

			if (fail != 0)
			{
				BSP_LCD_DisplayStringAt(20, 150, (uint8_t*) "QSPI ERASE : FAILED.", LEFT_MODE);
				BSP_LCD_DisplayStringAt(20, 175, (uint8_t*) "QSPI Writing Aborted.", LEFT_MODE);
			}
			else
			{
				/*##-4- QSPI memory read/write access  #################################*/
				/* Fill the buffer to write */
				_QSPI_ADDRESS_t *pQSPI_Addr = 0x0;

				fail = 0;

				/*
				 * NOTE:
				 * 	file_num : Number of files to save.
				 * 	Maximum : 32
				 * */
				for (uint8_t file_num = 0; file_num < MAXIMUM_NUM_OF_FILES; file_num++)
				{
					memset((uint8_t*) pSD->INTERNAL_BUFFER, 0xFF, sizeof(pSD->INTERNAL_BUFFER));

					/* Read File */
					if (User_SDCARD_OpenRead_BMP_File((uint8_t*) pSD->INTERNAL_BUFFER, sd_name[file_num]) == 0) //;
					{
						sprintf((char*) qspi_print_str, "SDCARD READ[%02d] : FAILED   ", (file_num + 1));
						BSP_LCD_DisplayStringAt(20, 125, (uint8_t*) qspi_print_str, LEFT_MODE);
						fail++;
						break;
					}

					sprintf((char*) qspi_print_str, "QSPI COMPARE[%02d]  : Comparing", (file_num + 1));
					BSP_LCD_DisplayStringAt(20, 125, (uint8_t*) qspi_print_str, LEFT_MODE);

					/* Read external memory by QSPI Communication. */
					if (User_QSPI_Read((uint8_t*) pSD->LCD_IMAGE_BUFFER[0], (uint32_t) pQSPI_Addr->LCD_IMAGE_BUFFER[file_num], sizeof(pSD->INTERNAL_BUFFER)) != MEMORY_OK)
					{
						fail++;
						break;
					}

					/* Compare buffers read by QSPI */
					if (User_QSPI_Buffercmp((uint8_t*) pSD->INTERNAL_BUFFER, (uint8_t*) pSD->LCD_IMAGE_BUFFER[0], sizeof(pSD->INTERNAL_BUFFER)) != 0)
					{
						/*
						 * NOTE :
						 * 	If the image is not the same, update the image.
						 * */

						sprintf((char*) qspi_print_str, "QSPI COMPARE[%02d]  : FAILED   ", (file_num + 1));
						BSP_LCD_DisplayStringAt(20, 125, (uint8_t*) qspi_print_str, LEFT_MODE);

						/*
						 * Clear QSPI external memory
						 * 	- Specify the memory area to delete.
						 * 	- Specified in <<< 4Kbyte >>> units
						 *
						 * 	Example
						 * 	 - sector = 1024 >> 4096(64Kbyte) * (32 * 32) = 64MByte
						 * 	 - 2MByte (64Kbyte * 32) allocated per image(one).
						 * */
						for (uint32_t sector = 0; sector < MAXIMUM_MEM_SECTOR_SIZE; sector++)
						{
							if (User_QSPI_Erase_Block(((file_num * MAXIMUM_MEM_SECTOR_SIZE) + sector) * N25Q512A_SECTOR_SIZE) != MEMORY_OK)
							{
								fail++;
								sprintf((char*) qspi_print_str, "QSPI ERASED[%02d]  : FAILED  ", (file_num + 1));
								BSP_LCD_DisplayStringAt(20, 150, (uint8_t*) qspi_print_str, LEFT_MODE);
								break;
							}
							else
							{
#if 0
								/* TEST */
								sprintf((char*) print_str, "sector : %5d_erased", sector);
								BSP_LCD_DisplayStringAt(20, 150, (uint8_t*) print_str, LEFT_MODE);
#endif
								if (sector == 31)
								{
									sprintf((char*) qspi_print_str, "QSPI ERASED[%02d]  : OK.     ", (file_num + 1));
									BSP_LCD_DisplayStringAt(20, 150, (uint8_t*) qspi_print_str, LEFT_MODE);
								}
							}
						}

						/* Write external memory by QSPI Communication. */
						if (User_QSPI_Write((uint8_t*) pSD->INTERNAL_BUFFER, (uint32_t) pQSPI_Addr->LCD_IMAGE_BUFFER[file_num], sizeof(pSD->INTERNAL_BUFFER)) != MEMORY_OK)
						{
							/* TODO : ERROR CHECK */
							fail++;
							sprintf((char*) qspi_print_str, "QSPI WRITE[%02d]  : FAILED  ", (file_num + 1));
							BSP_LCD_DisplayStringAt(20, 175, (uint8_t*) qspi_print_str, LEFT_MODE);
							break;
						}
						else
						{
							sprintf((char*) qspi_print_str, "QSPI WRITE[%02d]  : OK.     ", (file_num + 1));
							BSP_LCD_DisplayStringAt(20, 175, (uint8_t*) qspi_print_str, LEFT_MODE);
						}

						/* Read external memory by QSPI Communication. */
						if (User_QSPI_Read((uint8_t*) pSD->LCD_IMAGE_BUFFER[0], (uint32_t) pQSPI_Addr->LCD_IMAGE_BUFFER[file_num], sizeof(pSD->INTERNAL_BUFFER)) != MEMORY_OK)
						{
							fail++;
							break;
						}

						/* Compare buffers read by QSPI */
						if (User_QSPI_Buffercmp((uint8_t*) pSD->INTERNAL_BUFFER, (uint8_t*) pSD->LCD_IMAGE_BUFFER[0], sizeof(pSD->INTERNAL_BUFFER)) > 0)
						{
							/* TODO : ERROR CHECK */
							fail++;
							sprintf((char*) qspi_print_str, "QSPI READ[%02d]  : FAILED  ", (file_num + 1));
							BSP_LCD_DisplayStringAt(20, 200, (uint8_t*) qspi_print_str, LEFT_MODE);
							break;
						}
						else
						{
							sprintf((char*) qspi_print_str, "QSPI READ[%02d]  : OK.     ", (file_num + 1));
							BSP_LCD_DisplayStringAt(20, 200, (uint8_t*) qspi_print_str, LEFT_MODE);
						}
					}
					else
					{
						sprintf((char*) qspi_print_str, "QSPI COMPARE[%02d]  : OK.      ", (file_num + 1));
						BSP_LCD_DisplayStringAt(20, 125, (uint8_t*) qspi_print_str, LEFT_MODE);
					}
				}

				if (fail != 0)
				{
					BSP_LCD_DisplayStringAt(20, 225, (uint8_t*) "QSPI Writing Aborted.", LEFT_MODE);
					while (1)
					{
					}
				}
				else
				{
					BSP_LCD_DisplayStringAt(20, 225, (uint8_t*) "QSPI Writing : OK.     ", LEFT_MODE);
					HAL_Delay(3000);
				}
			}
		}
	}
	BSP_LCD_Clear(LCD_COLOR_WHITE);
}

/**
 * @brief  Display QSPI Demo Hint
 * @param  None
 * @retval None
 */
static void QSPI_SetHint(void)
{
	/* Clear the LCD */
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	/* Set LCD Demo description */
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(0, 0, (uint8_t*) "QSPI", CENTER_MODE);
#if 0
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(0, 30, (uint8_t*) "This example shows how to write", CENTER_MODE);
	BSP_LCD_DisplayStringAt(0, 45, (uint8_t*) "and read data on QSPI memory", CENTER_MODE);
	BSP_LCD_DisplayStringAt(0, 60, (uint8_t*) "(Hardware modifications needed)", CENTER_MODE);
#endif

	/* Set the LCD Text Color */
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 100);
	BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 102);

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
}

/**
 * @brief  Compares two buffers.
 * @param  pBuffer1, pBuffer2: buffers to be compared.
 * @param  BufferLength: buffer's length
 * @retval 1: pBuffer identical to pBuffer1
 *         0: pBuffer differs from pBuffer1
 */
static uint8_t User_QSPI_Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint32_t BufferLength)
{
	while (BufferLength--)
	{
		if (*pBuffer1 != *pBuffer2)
		{
			return 1;
		}

		pBuffer1++;
		pBuffer2++;
	}

	return 0;
}
/* USER CODE END 0 */

