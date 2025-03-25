/* USER CODE BEGIN Header */
/*
 * user_mem_sdcard.h
 *
 *  Created on: Mar 25, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_mem_sdcard.h"
#include "fatfs.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
uint8_t sector[512];
FATFS fs;
FILINFO fno;
DIR dir;
FIL F;
const uint8_t SlidesCheck[2] =
{ 0x42, 0x4D };
uint32_t BytesRead = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
uint32_t User_SDCARD_Storage_Init(void);
uint32_t User_SDCARD_OpenRead_BMP_File(uint8_t *Address, const char *BmpName);
uint32_t User_SDCARD_GetDirectory_BMP_Files(const char *DirName, char *Files[]);
uint32_t User_SDCARD_Check_BMP_File(const char *BmpName, uint32_t *FileLen);
uint8_t User_Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
/* Radial and Focus Timer Configuration */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief  SDCARD Initialization for FatFs
 * @param  None
 * @retval err : Error status (0=> success, 1=> fail)
 */
uint32_t User_SDCARD_Storage_Init(void)
{
	BSP_SD_Init();

	/****************** FatFs Volume Access ******************************/
	if (f_mount(&fs, (TCHAR const*) "", 0))
	{
		return 1;
	}
	return 0;
}

/**
 * @brief  Open a file and copy its content to a buffer
 * @param  DirName: the Directory name to open
 * @param  FileName: the file name to open
 * @param  BufferAddress: A pointer to a buffer to copy the file to
 * @param  FileLen: the File lenght
 * @retval err: Error status (0=> success, 1=> fail)
 */
uint32_t User_SDCARD_OpenRead_BMP_File(uint8_t *Address, const char *BmpName)
{
	uint32_t index = 0, size = 0, i1 = 0;
	uint32_t BmpAddress;
	FIL F1;

	if (BSP_SD_IsDetected() != SD_PRESENT)
		return 0;

	if (f_mount(&fs, (TCHAR const*) SDPath, 0) != FR_OK)
	{
		f_close(&F1);
		return 0;
	}

	if (f_open(&F1, (TCHAR const*) BmpName, FA_READ) != FR_OK)
	{
		f_close(&F1);
		return 0;
	}
	if (f_read(&F1, sector, 30, (UINT*) &BytesRead) != FR_OK)
	{
		f_close(&F1);
		return 0;
	}

	BmpAddress = (uint32_t) sector;

	/* Read bitmap size */
	size = *(uint16_t*) (BmpAddress + 2);
	size |= (*(uint16_t*) (BmpAddress + 4)) << 16;

	/* Get bitmap data address offset */
	index = *(uint16_t*) (BmpAddress + 10);
	index |= (*(uint16_t*) (BmpAddress + 12)) << 16;

	f_close(&F1);

	f_open(&F1, (TCHAR const*) BmpName, FA_READ);

	do
	{
		if (size < 256 * 2)
		{
			i1 = size;
		}
		else
		{
			i1 = 256 * 2;
		}
		size -= i1;
		f_read(&F1, sector, i1, (UINT*) &BytesRead);

		for (index = 0; index < i1; index++)
		{
			*(__IO uint8_t*) (Address) = *(__IO uint8_t*) BmpAddress;

			BmpAddress++;
			Address++;
		}

		BmpAddress = (uint32_t) sector;
	} while (size > 0);

	f_close(&F1);

	return 1;
}

/**
 * @brief  Open a file and copy its content to a buffer
 * @param  DirName: the Directory name to open
 * @param  FileName: the file name to open
 * @param  BufferAddress: A pointer to a buffer to copy the file to
 * @param  FileLen: the File lenght
 * @retval err: Error status (0=> success, 1=> fail)
 */
uint32_t User_SDCARD_Check_BMP_File(const char *BmpName, uint32_t *FileLen)
{
	if (f_mount(&fs, (TCHAR const*) "", 0))
	{
		return 1;
	}
	if (f_open(&F, (TCHAR const*) BmpName, FA_READ))
	{
		return 2;
	}

	f_read(&F, sector, 6, (UINT*) &BytesRead);

	if (User_Buffercmp((uint8_t*) SlidesCheck, (uint8_t*) sector, 2) != 0)
	{
		return 3;
	}
	return 0;
}

/**
 * @brief  List up to 25 file on the root directory with extension .BMP
 * @param  None
 * @retval The number of the found files
 */
uint32_t User_SDCARD_GetDirectory_BMP_Files(const char *DirName, char *Files[])
{
	//FATFS fs;
	FILINFO fno;
	DIR dir;
	uint32_t counter = 0, index = 0;
	FRESULT res;

	if (BSP_SD_IsDetected() != SD_PRESENT)
		return 0;

	/* Open filesystem */
	if (f_mount(&fs, (TCHAR const*) "", 0) != FR_OK)
	{
		return 0;
	}

	/* Open directory */
	res = f_opendir(&dir, (TCHAR const*) DirName);

	if (res == FR_OK)
	{
		for (;;)
		{
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0)
				break;
			if (fno.fname[0] == '.')
				continue;

			if (!(fno.fattrib & AM_DIR))
			{
				do
				{
					counter++;
				} while (fno.fname[counter] != 0x2E); /* . */

				if (index < MAX_BMP_FILES)
				{
					if ((fno.altname[counter + 1] == 'B') && (fno.altname[counter + 2] == 'M') && (fno.altname[counter + 3] == 'P'))
					{
						if (sizeof(fno.fname) >= (MAX_BMP_FILE_NAME + 2))
						{
							sprintf(Files[index], "%s", fno.fname);
							index++;
						}
					}
				}
				counter = 0;
			}
		}
	}
	f_mount(NULL, (TCHAR const*) "", 0);
	return index;
}

/**
 * @brief  Compares two buffers.
 * @param  pBuffer1, pBuffer2: buffers to be compared.
 * @param  BufferLength: buffer's length.
 * @retval  0: pBuffer1 identical to pBuffer2
 *          1: pBuffer1 differs from pBuffer2
 */
uint8_t User_Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
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

