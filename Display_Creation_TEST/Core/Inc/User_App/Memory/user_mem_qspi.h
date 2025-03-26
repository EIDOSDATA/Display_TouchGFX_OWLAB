/*
 * user_mem_qspi.h
 *
 *  Created on: Mar 26, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_MEMORY_USER_MEM_QSPI_H_
#define INC_USER_APP_MEMORY_USER_MEM_QSPI_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "memory.h"
#include "n25q512a.h"
#include "stm32h7xx_hal.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct
{
	uint32_t FlashSize; /*!< Size of the flash */
	uint32_t EraseSectorSize; /*!< Size of sectors for the erase operation */
	uint32_t EraseSectorsNumber; /*!< Number of sectors for the erase operation */
	uint32_t ProgPageSize; /*!< Size of pages for the program operation */
	uint32_t ProgPagesNumber; /*!< Number of pages for the program operation */
}User_QSPI_Info;

typedef struct
{
	__IO uint8_t LCD_IMAGE_BUFFER[32][0x200000];
}_QSPI_ADDRESS_t;

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
extern uint32_t User_QSPI_Startup_Parameter_Init(void);
extern uint32_t User_QSPI_Startup(uint32_t Mode);
extern uint32_t User_QSPI_DeInit(void);
extern uint32_t User_QSPI_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
extern uint32_t User_QSPI_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
extern uint32_t User_QSPI_Erase_Block(uint32_t BlockAddress);
extern uint32_t User_QSPI_Erase_Chip(void);
extern uint32_t User_QSPI_GetStatus(void);
extern uint32_t User_QSPI_GetInfo(User_QSPI_Info *pInfo);
extern void User_QSPI_MspInsit(QSPI_HandleTypeDef *hqspi, void *Params);
extern void User_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi, void *Params);

extern uint32_t User_QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi);

/* QSPI App Function */
extern void User_QSPI_Init(void);
extern void User_QSPI_SaveData_To_FlashMemory(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_MEMORY_USER_MEM_QSPI_H_ */

