/*
 * user_mem_sdram.h
 *
 *  Created on: Mar 5, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_MEMORY_USER_MEM_SDRAM_H_
#define INC_USER_APP_MEMORY_USER_MEM_SDRAM_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_hal.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32h7xx_hal_sdram.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern FMC_SDRAM_TimingTypeDef SDRAM_Timing;
extern FMC_SDRAM_CommandTypeDef command;

typedef struct
{
	// Total 32MByte
	__IO uint8_t _LCD_FRAME_BUFFER_LAYER0[0x300000]; // 3MByte * 1 =  3MByte
	__IO uint8_t _LCD_FRAME_BUFFER_LAYER1[0x300000]; // 3MByte * 1 =  3MByte
	__IO uint8_t INTERNAL_BUFFER[0x200000];          // 2MByte * 1 =  2MByte
	__IO uint8_t LCD_IMAGE_BUFFER[12][0x200000];     // 2MByte * 12= 26MByte
} _SDRAM_ADDRESS_t;

extern _SDRAM_ADDRESS_t *pSD;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
extern void User_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command);
extern uint8_t User_SDRAM_ReadData(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
extern uint8_t User_SDRAM_ReadData_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
extern uint8_t User_SDRAM_WriteData(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
extern uint8_t User_SDRAM_WriteData_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
extern uint8_t User_SDRAM_Sendcmd(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *SdramCmd);

extern void User_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram, void *Params);
extern void User_SDRAM_MspDeInit(SDRAM_HandleTypeDef *hsdram, void *Params);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define SDRAM_BANK_ADDR                 ((uint32_t)0xD0000000)
#define SDRAM_DEVICE_ADDR  				((uint32_t)0xD0000000)
#define SDRAM_DEVICE_SIZE  				((uint32_t)0x2000000)  /* SDRAM device size in Bytes (32MB)*/

/* #define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_8  */
/* #define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_16 */
#define SDRAM_MEMORY_WIDTH               FMC_SDRAM_MEM_BUS_WIDTH_32

#define SDCLOCK_PERIOD                   FMC_SDRAM_CLOCK_PERIOD_2
/* #define SDCLOCK_PERIOD                FMC_SDRAM_CLOCK_PERIOD_3 */

#define SDRAM_TIMEOUT                    ((uint32_t)0xFFFF)
#define REFRESH_COUNT                    ((uint32_t)0x0603)   /* SDRAM refresh counter */

/* DMA definitions for SDRAM DMA transfer */
#define __MDMAx_CLK_ENABLE                 __HAL_RCC_MDMA_CLK_ENABLE
#define __MDMAx_CLK_DISABLE                __HAL_RCC_MDMA_CLK_DISABLE
#define SDRAM_MDMAx_CHANNEL               MDMA_Channel0
#define SDRAM_MDMAx_IRQn                  MDMA_IRQn

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_MEMORY_USER_MEM_SDRAM_H_ */
