/*
 * user_mem_qspi.h
 *
 *  Created on: Mar 5, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_MEMORY_USER_MEM_QSPI_H_
#define INC_USER_APP_MEMORY_USER_MEM_QSPI_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f769i_eval_qspi.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct
{
	__IO uint8_t LCD_IMAGE_BUFFER[32][0x200000];
} _QSPI_ADDRESS_t;

extern _QSPI_ADDRESS_t *pQSPI;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
extern void User_QSPI_Init(void);
extern void User_QSPI_SaveData_To_FlashMemory(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
//#define QSPI_DEVICE_ADDR ((uint32_t)0x90000000)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_MEMORY_USER_MEM_QSPI_H_ */
