/*
 * user_mem_sdcard.h
 *
 *  Created on: Mar 25, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_MEMORY_USER_MEM_SDCARD_H_
#define INC_USER_APP_MEMORY_USER_MEM_SDCARD_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
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
extern uint32_t User_SDCARD_Storage_Init(void);
extern uint32_t User_SDCARD_OpenRead_BMP_File(uint8_t *Address, const char *BmpName);
extern uint32_t User_SDCARD_GetDirectory_BMP_Files(const char *DirName, char *Files[]);
extern uint32_t User_SDCARD_Check_BMP_File(const char *BmpName, uint32_t *FileLen);
extern uint8_t User_Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define MAX_BMP_FILES     25
#define MAX_BMP_FILE_NAME 11
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_MEMORY_USER_MEM_SDCARD_H_ */

