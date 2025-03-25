/* USER CODE BEGIN Header */
/*
 * user_mem_sdram.c
 *
 *  Created on: Mar 5, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_mem_sdram.h"

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
FMC_SDRAM_TimingTypeDef SDRAM_Timing;
FMC_SDRAM_CommandTypeDef command;
_SDRAM_ADDRESS_t *pSD = (_SDRAM_ADDRESS_t*) SDRAM_DEVICE_ADDR;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void User_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command);

uint8_t User_SDRAM_ReadData(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t User_SDRAM_ReadData_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t User_SDRAM_WriteData(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t User_SDRAM_WriteData_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t User_SDRAM_Sendcmd(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *SdramCmd);

void User_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram, void *Params);
void User_SDRAM_MspDeInit(SDRAM_HandleTypeDef *hsdram, void *Params);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void User_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
	/* Step 1:  Configure a clock configuration enable command */
	Command->CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;

	Command->AutoRefreshNumber = 1;
	Command->ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

	/* Step 2: Insert 100 us minimum delay */
	/* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
	HAL_Delay(1);

	/* Step 3: Configure a PALL (precharge all) command */
	Command->CommandMode = FMC_SDRAM_CMD_PALL;
	Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
	Command->AutoRefreshNumber = 1;
	Command->ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

	/* Step 4 : Configure a Auto-Refresh command */
	Command->CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
	Command->AutoRefreshNumber = 8;
	Command->ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

	/* Step 5: Program the external memory mode register */

	Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
	Command->AutoRefreshNumber = 1;

#if (!TOUCHGFX_ENABLED_MODE)
	__IO uint32_t tmpmrd = 0;
	tmpmrd = (uint32_t) SDRAM_MODEREG_BURST_LENGTH_1 |
	SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
	SDRAM_MODEREG_CAS_LATENCY_3 |
	SDRAM_MODEREG_OPERATING_MODE_STANDARD |
	SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
	Command->ModeRegisterDefinition = tmpmrd;
#else
	uint32_t mode_reg = 0;
	mode_reg |= (0 << 0); // Burst Length = 1
	mode_reg |= (0 << 3); // Burst Type
						  //   0 : Sequential
						  //   1 : Interleaved
	mode_reg |= (2 << 4); // CAS Latency Mode
						  //   2 :
						  //   3 :
	mode_reg |= (0 << 7); // Operation Mode
	mode_reg |= (1 << 9); // Write Burst Mode
						  //   0 : Programmed Burst Length
						  //   1 : Single Location Access
	Command->ModeRegisterDefinition = mode_reg;
#endif
	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

	/* Step 6: Set the refresh rate counter */
	/* Set the device refresh rate */
	HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT);

}

/**
 * @brief  Reads an amount of data from the SDRAM memory in polling mode.
 * @param  uwStartAddress: Read start address
 * @param  pData: Pointer to data to be read
 * @param  uwDataSize: Size of read data from the memory
 * @retval SDRAM status
 */
uint8_t User_SDRAM_ReadData(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
	if (HAL_SDRAM_Read_32b(hsdram, (uint32_t*) uwStartAddress, pData, uwDataSize) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		return HAL_OK;
	}
}

/**
 * @brief  Reads an amount of data from the SDRAM memory in DMA mode.
 * @param  uwStartAddress: Read start address
 * @param  pData: Pointer to data to be read
 * @param  uwDataSize: Size of read data from the memory
 * @retval SDRAM status
 */
uint8_t User_SDRAM_ReadData_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
	if (HAL_SDRAM_Read_DMA(hsdram, (uint32_t*) uwStartAddress, pData, uwDataSize) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		return HAL_OK;
	}
}

/**
 * @brief  Writes an amount of data to the SDRAM memory in polling mode.
 * @param  uwStartAddress: Write start address
 * @param  pData: Pointer to data to be written
 * @param  uwDataSize: Size of written data from the memory
 * @retval SDRAM status
 */
uint8_t User_SDRAM_WriteData(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
	if (HAL_SDRAM_Write_32b(hsdram, (uint32_t*) uwStartAddress, pData, uwDataSize) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		return HAL_OK;
	}
}

/**
 * @brief  Writes an amount of data to the SDRAM memory in DMA mode.
 * @param  uwStartAddress: Write start address
 * @param  pData: Pointer to data to be written
 * @param  uwDataSize: Size of written data from the memory
 * @retval SDRAM status
 */
uint8_t User_SDRAM_WriteData_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
	if (HAL_SDRAM_Write_DMA(hsdram, (uint32_t*) uwStartAddress, pData, uwDataSize) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		return HAL_OK;
	}
}

/**
 * @brief  Sends command to the SDRAM bank.
 * @param  SdramCmd: Pointer to SDRAM command structure
 * @retval SDRAM status
 */
uint8_t User_SDRAM_Sendcmd(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *SdramCmd)
{
	if (HAL_SDRAM_SendCommand(hsdram, SdramCmd, SDRAM_TIMEOUT) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		return HAL_OK;
	}
}

/**
 * @brief  This function handles SDRAM MDMA interrupt request.
 * @param  Instance SDRAM instance
 * @retval None
 */
void User_SDRAM_IRQHandler(SDRAM_HandleTypeDef *hsdram, uint32_t Instance)
{
	HAL_MDMA_IRQHandler(hsdram[Instance].hmdma);
}

/**
 * @brief  Initializes SDRAM MSP.
 * @param  hsdram: SDRAM handle
 * @param  Params: Pointer to void
 * @retval None
 */
static uint32_t FMC_Initialized = 0;
void User_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram, void *Params)
{
#if 0
	static MDMA_HandleTypeDef MDMA_Handle;
#endif
	GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };
	if (FMC_Initialized)
	{
		return;
	}
	FMC_Initialized = 1;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct =
	{ 0 };

	/** Initializes the peripherals clock
	 */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
	PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Enable FMC clock */
	__HAL_RCC_FMC_CLK_ENABLE();

	/* Enable chosen MDMAx clock */
	__MDMAx_CLK_ENABLE()
	;

	/* Enable GPIOs clock */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();

	/** FMC GPIO Configuration
	 PI9   ------> FMC_D30
	 PI10   ------> FMC_D31
	 PF0   ------> FMC_A0
	 PF1   ------> FMC_A1
	 PF2   ------> FMC_A2
	 PF3   ------> FMC_A3
	 PF4   ------> FMC_A4
	 PF5   ------> FMC_A5
	 PH5   ------> FMC_SDNWE
	 PF11   ------> FMC_SDNRAS
	 PF12   ------> FMC_A6
	 PF13   ------> FMC_A7
	 PF14   ------> FMC_A8
	 PF15   ------> FMC_A9
	 PG0   ------> FMC_A10
	 PG1   ------> FMC_A11
	 PE7   ------> FMC_D4
	 PE8   ------> FMC_D5
	 PE9   ------> FMC_D6
	 PE10   ------> FMC_D7
	 PE11   ------> FMC_D8
	 PE12   ------> FMC_D9
	 PE13   ------> FMC_D10
	 PE14   ------> FMC_D11
	 PE15   ------> FMC_D12
	 PH6   ------> FMC_SDNE1
	 PH7   ------> FMC_SDCKE1
	 PH8   ------> FMC_D16
	 PH9   ------> FMC_D17
	 PH10   ------> FMC_D18
	 PH11   ------> FMC_D19
	 PH12   ------> FMC_D20
	 PD8   ------> FMC_D13
	 PD9   ------> FMC_D14
	 PD10   ------> FMC_D15
	 PD14   ------> FMC_D0
	 PD15   ------> FMC_D1
	 PG4   ------> FMC_BA0
	 PG5   ------> FMC_BA1
	 PG8   ------> FMC_SDCLK
	 PH13   ------> FMC_D21
	 PH14   ------> FMC_D22
	 PH15   ------> FMC_D23
	 PI0   ------> FMC_D24
	 PI1   ------> FMC_D25
	 PI2   ------> FMC_D26
	 PI3   ------> FMC_D27
	 PD0   ------> FMC_D2
	 PD1   ------> FMC_D3
	 PG15   ------> FMC_SDNCAS
	 PE0   ------> FMC_NBL0
	 PE1   ------> FMC_NBL1
	 PI4   ------> FMC_NBL2
	 PI5   ------> FMC_NBL3
	 PI6   ------> FMC_D28
	 PI7   ------> FMC_D29
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* Configure common MDMA parameters */
#if 0
	MDMA_Handle.Init.Request = MDMA_REQUEST_SW;
	MDMA_Handle.Init.TransferTriggerMode = MDMA_BLOCK_TRANSFER;
	MDMA_Handle.Init.Priority = MDMA_PRIORITY_HIGH;
	MDMA_Handle.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
	MDMA_Handle.Init.SourceInc = MDMA_SRC_INC_WORD;
	MDMA_Handle.Init.DestinationInc = MDMA_DEST_INC_WORD;
	MDMA_Handle.Init.SourceDataSize = MDMA_SRC_DATASIZE_WORD;
	MDMA_Handle.Init.DestDataSize = MDMA_DEST_DATASIZE_WORD;
	MDMA_Handle.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
	MDMA_Handle.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
	MDMA_Handle.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
	MDMA_Handle.Init.BufferTransferLength = 128;
	MDMA_Handle.Init.SourceBlockAddressOffset = 0;
	MDMA_Handle.Init.DestBlockAddressOffset = 0;

	MDMA_Handle.Instance = SDRAM_MDMAx_CHANNEL;

	/* Associate the DMA handle */
	__HAL_LINKDMA(hsdram, hmdma, MDMA_Handle);

	/* Deinitialize the stream for new transfer */
	HAL_MDMA_DeInit(&MDMA_Handle);

	/* Configure the DMA stream */
	HAL_MDMA_Init(&MDMA_Handle);

	/* NVIC configuration for DMA transfer complete interrupt */
	HAL_NVIC_SetPriority(SDRAM_MDMAx_IRQn, 0x0F, 0);
	HAL_NVIC_EnableIRQ(SDRAM_MDMAx_IRQn);
#endif
}

/**
 * @brief  DeInitializes SDRAM MSP.
 * @param  hsdram: SDRAM handle
 * @param  Params: Pointer to void
 * @retval None
 */
static uint32_t FMC_DeInitialized = 0;
void User_SDRAM_MspDeInit(SDRAM_HandleTypeDef *hsdram, void *Params)
{

	if (FMC_DeInitialized)
	{
		return;
	}
	FMC_DeInitialized = 1;

#if 0
	static MDMA_HandleTypeDef MDMA_Handle;

	/* Disable NVIC configuration for DMA interrupt */
	HAL_NVIC_DisableIRQ(SDRAM_MDMAx_IRQn);

	/* Deinitialize the stream for new transfer */
	MDMA_Handle.Instance = SDRAM_MDMAx_CHANNEL;
	HAL_MDMA_DeInit(&MDMA_Handle);
#endif

	/* Peripheral clock enable */
	__HAL_RCC_FMC_CLK_DISABLE();

	/** FMC GPIO Configuration
	 PI9   ------> FMC_D30
	 PI10   ------> FMC_D31
	 PF0   ------> FMC_A0
	 PF1   ------> FMC_A1
	 PF2   ------> FMC_A2
	 PF3   ------> FMC_A3
	 PF4   ------> FMC_A4
	 PF5   ------> FMC_A5
	 PH5   ------> FMC_SDNWE
	 PF11   ------> FMC_SDNRAS
	 PF12   ------> FMC_A6
	 PF13   ------> FMC_A7
	 PF14   ------> FMC_A8
	 PF15   ------> FMC_A9
	 PG0   ------> FMC_A10
	 PG1   ------> FMC_A11
	 PE7   ------> FMC_D4
	 PE8   ------> FMC_D5
	 PE9   ------> FMC_D6
	 PE10   ------> FMC_D7
	 PE11   ------> FMC_D8
	 PE12   ------> FMC_D9
	 PE13   ------> FMC_D10
	 PE14   ------> FMC_D11
	 PE15   ------> FMC_D12
	 PH6   ------> FMC_SDNE1
	 PH7   ------> FMC_SDCKE1
	 PH8   ------> FMC_D16
	 PH9   ------> FMC_D17
	 PH10   ------> FMC_D18
	 PH11   ------> FMC_D19
	 PH12   ------> FMC_D20
	 PD8   ------> FMC_D13
	 PD9   ------> FMC_D14
	 PD10   ------> FMC_D15
	 PD14   ------> FMC_D0
	 PD15   ------> FMC_D1
	 PG4   ------> FMC_BA0
	 PG5   ------> FMC_BA1
	 PG8   ------> FMC_SDCLK
	 PH13   ------> FMC_D21
	 PH14   ------> FMC_D22
	 PH15   ------> FMC_D23
	 PI0   ------> FMC_D24
	 PI1   ------> FMC_D25
	 PI2   ------> FMC_D26
	 PI3   ------> FMC_D27
	 PD0   ------> FMC_D2
	 PD1   ------> FMC_D3
	 PG15   ------> FMC_SDNCAS
	 PE0   ------> FMC_NBL0
	 PE1   ------> FMC_NBL1
	 PI4   ------> FMC_NBL2
	 PI5   ------> FMC_NBL3
	 PI6   ------> FMC_D28
	 PI7   ------> FMC_D29
	 */
	HAL_GPIO_DeInit(GPIOI, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

	HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

	HAL_GPIO_DeInit(GPIOH, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

	HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15);

	HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1);

	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1);
}
/* USER CODE END 0 */
