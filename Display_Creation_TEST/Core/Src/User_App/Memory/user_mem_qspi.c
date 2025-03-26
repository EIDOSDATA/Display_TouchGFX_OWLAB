/* USER CODE BEGIN Header */
/*
 * user_mem_qspi.c
 *
 *  Created on: Mar 25, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_mem_qspi.h"

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
extern QSPI_HandleTypeDef hqspi; //QSPI_HandleTypeDef QSPIHandle;
QSPI_CommandTypeDef sCommand;
QSPI_AutoPollingTypeDef sConfig;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
uint32_t User_QSPI_Startup_Parameter_Init(void);
uint32_t User_QSPI_Startup(uint32_t Mode);
uint32_t User_QSPI_DeInit(void);
uint32_t User_QSPI_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
uint32_t User_QSPI_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
uint32_t User_QSPI_Erase_Block(uint32_t BlockAddress);
uint32_t User_QSPI_Erase_Chip(void);
uint32_t User_QSPI_GetStatus(void);
uint32_t User_QSPI_GetInfo(User_QSPI_Info *pInfo);
uint32_t User_QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi);

/* MSP INIT and DE-INIT */
void User_QSPI_MspInsit(QSPI_HandleTypeDef *hqspi, void *Params);
void User_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi, void *Params);

static uint32_t User_QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi);
static uint32_t User_QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi);
static uint32_t User_QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi);
static uint32_t User_QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi);
// static uint32_t User_QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi);
static uint32_t User_QSPI_Shutdown(void);
static uint32_t User_QSPI_Copy(uint32_t WriteAddr, uint32_t ReadAddr, uint32_t Size);
static uint32_t User_QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief  Initializes and configure the QSPI interface.
 * @retval QSPI memory status
 */
uint32_t User_QSPI_Startup_Parameter_Init(void)
{
	hqspi.Instance = QUADSPI;

	/* Call the DeInit function to reset the driver */
	if (HAL_QSPI_DeInit(&hqspi) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* System level initialization */
	HAL_QSPI_MspInit(&hqspi); /* User_QSPI_MspInit Check PLZ */

	/* sCommand initialize common parameter */
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = QSPI_DATA_NONE;
	sCommand.DummyCycles = 0;
	sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	sCommand.AddressSize = QSPI_ADDRESS_32_BITS;

	/* sConfig initialize common parameter */
	sConfig.MatchMode = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

	return MEMORY_OK;
}

/**
 * @brief  Initializes and configure the QSPI interface.
 * @retval QSPI memory status
 */
uint32_t User_QSPI_Startup(uint32_t Mode)
{
#if 0
	int testval = 0;
	testval = POSITION_VAL(N25Q512A_FLASH_SIZE) - 1; /* SIZE 25 */
	User_QSPI_Startup_Parameter_Init();
#endif
	/* QSPI initialization */
	/* ClockPrescaler set to 1, so QSPI clock = 200MHz / (1+1) = 100MHz */
	/* STM32 CUBE MX Area... */
#if 1
	hqspi.Init.ClockPrescaler = 1;
	hqspi.Init.FifoThreshold = 4;
	hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
	hqspi.Init.FlashSize = POSITION_VAL(N25Q512A_FLASH_SIZE) - 1; /* n25q512a */
	hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_3_CYCLE;
	hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
	hqspi.Init.FlashID = QSPI_FLASH_ID_1;
	hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
	if (HAL_QSPI_Init(&hqspi) != HAL_OK)
	{
		return MEMORY_ERROR;
	}
#endif

	/* QSPI memory reset */
	if (User_QSPI_ResetMemory(&hqspi) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}

#if (CODE_AREA == USE_QSPI)
	/* Set the QSPI memory in 4-bytes address mode */
	if (User_QSPI_EnterFourBytesAddress(&hqspi) != MEMORY_OK)
	{
		return QSPI_MEMORY_NOT_SUPPORTED;
	}

	/* Configuration of the dummy cycles on QSPI memory side */
	if (User_QSPI_DummyCyclesCfg(&hqspi) != MEMORY_OK)
	{
		return QSPI_MEMORY_NOT_SUPPORTED;
	}

#if 0
	/* Enable MemoryMapped mode :: After Init */
	if (User_QSPI_EnableMemoryMappedMode(&hqspi) != MEMORY_OK)
	{
		return QSPI_MEMORY_NOT_SUPPORTED;
	}
#endif

#elif (BINARY_AREA == USE_SPI_NOR)
  if (User_QSPI_Copy(APPLICATION_ADDRESS, BINARY_BASE_OFFSET, BINARY_SIZE) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  if (User_QSPI_Shutdown() != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }
#endif /* (CODE_AREA == USE_QSPI) */
	return MEMORY_OK;
}

/**
 * @brief  De-Initializes the QSPI interface.
 * @retval QSPI memory status
 */
uint32_t User_QSPI_DeInit(void)
{
	hqspi.Instance = QUADSPI;

	/* Call the DeInit function to reset the driver */
	if (HAL_QSPI_DeInit(&hqspi) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* System level De-initialization */
	User_QSPI_MspDeInit(&hqspi, NULL);

	return MEMORY_OK;
}

/**
 * @brief  Reads an amount of data from the QSPI memory.
 * @param  pData: Pointer to data to be read
 * @param  ReadAddr: Read start address
 * @param  Size: Size of data to read
 * @retval QSPI memory status
 */

uint32_t User_QSPI_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
	QSPI_CommandTypeDef s_command;

	/* Initialize the read command */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = QUAD_OUT_FAST_READ_CMD; /* n25q512a */
	s_command.AddressMode = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize = QSPI_ADDRESS_32_BITS;
	s_command.Address = ReadAddr;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_4_LINES;
	s_command.DummyCycles = N25Q512A_DUMMY_CYCLES_READ_QUAD; /* n25q512a */
	s_command.NbData = Size;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	/* Configure the command */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Set S# timing for Read command */
	MODIFY_REG(hqspi.Instance->DCR, QUADSPI_DCR_CSHT, QSPI_CS_HIGH_TIME_3_CYCLE);

	/* Reception of the data */
	if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Restore S# timing for nonRead commands */
	MODIFY_REG(hqspi.Instance->DCR, QUADSPI_DCR_CSHT, QSPI_CS_HIGH_TIME_6_CYCLE);

	return MEMORY_OK;
}

/**
 * @brief  Writes an amount of data to the QSPI memory.
 * @param  pData: Pointer to data to be written
 * @param  WriteAddr: Write start address
 * @param  Size: Size of data to write
 * @retval QSPI memory status
 */
uint32_t User_QSPI_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
	QSPI_CommandTypeDef s_command;
	uint32_t end_addr, current_size, current_addr;

	/* Calculation of the size between the write address and the end of the page */
	current_size = N25Q512A_PAGE_SIZE - (WriteAddr % N25Q512A_PAGE_SIZE);

	/* Check if the size of the data is less than the remaining place in the page */
	if (current_size > Size)
	{
		current_size = Size;
	}

	/* Initialize the address variables */
	current_addr = WriteAddr;
	end_addr = WriteAddr + Size;

	/* Initialize the program command */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = QUAD_IN_FAST_PROG_CMD; /* n25q512a */
	s_command.AddressMode = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize = QSPI_ADDRESS_32_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_4_LINES;
	s_command.DummyCycles = 0;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	/* Perform the write page by page */
	do
	{
		s_command.Address = current_addr;
		s_command.NbData = current_size;

		/* Enable write operations */
		if (User_QSPI_WriteEnable(&hqspi) != MEMORY_OK)
		{
			return MEMORY_ERROR;
		}

		/* Configure the command */
		if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			return MEMORY_ERROR;
		}

		/* Transmission of the data */
		if (HAL_QSPI_Transmit(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			return MEMORY_ERROR;
		}

		/* Configure automatic polling mode to wait for end of program */
		if (User_QSPI_AutoPollingMemReady(&hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != MEMORY_OK)
		{
			return MEMORY_ERROR;
		}

		/* Update the address and size variables for next page programming */
		current_addr += current_size;
		pData += current_size;
		current_size = ((current_addr + N25Q512A_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : N25Q512A_PAGE_SIZE;
	} while (current_addr < end_addr);

	return MEMORY_OK;
}

/**
 * @brief  Erases the specified block of the QSPI memory.
 * @param  BlockAddress: Block address to erase
 * @retval QSPI memory status
 */
uint32_t User_QSPI_Erase_Block(uint32_t BlockAddress)
{
	QSPI_CommandTypeDef s_command;

	/* Initialize the erase command */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = SECTOR_ERASE_CMD; /* n25q512a :: SUBSECTOR_ERASE_CMD */
	s_command.AddressMode = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize = QSPI_ADDRESS_32_BITS;
	s_command.Address = BlockAddress;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_NONE;
	s_command.DummyCycles = 0;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	/* Enable write operations */
	if (User_QSPI_WriteEnable(&hqspi) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}

	/* Send the command */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Configure automatic polling mode to wait for end of erase */
	if (User_QSPI_AutoPollingMemReady(&hqspi, N25Q512A_SUBSECTOR_ERASE_MAX_TIME) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}

	return MEMORY_OK;
}

/**
 * @brief  Erases the entire QSPI memory.
 * @retval QSPI memory status
 */
uint32_t User_QSPI_Erase_Chip(void)
{
	QSPI_CommandTypeDef s_command;

	/* Initialize the erase command */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = BULK_ERASE_CMD; /* n25q512a */
	s_command.AddressMode = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_NONE;
	s_command.DummyCycles = 0;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	/* Enable write operations */
	if (User_QSPI_WriteEnable(&hqspi) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}

	/* Send the command */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Configure automatic polling mode to wait for end of erase */
	if (User_QSPI_AutoPollingMemReady(&hqspi, N25Q512A_BULK_ERASE_MAX_TIME) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}

	return MEMORY_OK;
}

/**
 * @brief  Reads current status of the QSPI memory.
 * @retval QSPI memory status
 */
uint32_t User_QSPI_GetStatus(void)
{
	QSPI_CommandTypeDef s_command;
	uint8_t reg;

	/* Initialize the read flag status register command */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = READ_FLAG_STATUS_REG_CMD; /* n25q512a */
	s_command.AddressMode = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_1_LINE;
	s_command.DummyCycles = 0;
	s_command.NbData = 1;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	/* Configure the command */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Reception of the data */
	if (HAL_QSPI_Receive(&hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Check the value of the register */
	if ((reg & (N25Q512A_FSR_PRERR | N25Q512A_FSR_VPPERR | N25Q512A_FSR_PGERR | N25Q512A_FSR_ERERR)) != 0)
	{
		return MEMORY_ERROR;
	}
	else if ((reg & (N25Q512A_FSR_PGSUS | N25Q512A_FSR_ERSUS)) != 0)
	{
		return QSPI_MEMORY_SUSPENDED;
	}
	else if ((reg & N25Q512A_FSR_READY) != 0)
	{
		return MEMORY_OK;
	}
	else
	{
		return QSPI_MEMORY_BUSY;
	}
}

/**
 * @brief  Return the configuration of the QSPI memory.
 * @param  pInfo: pointer on the configuration structure
 * @retval QSPI memory status
 */
uint32_t User_QSPI_GetInfo(User_QSPI_Info *pInfo)
{
	/* Configure the structure with the memory configuration */
	pInfo->FlashSize = N25Q512A_FLASH_SIZE;
	pInfo->EraseSectorSize = N25Q512A_SUBSECTOR_SIZE;
	pInfo->EraseSectorsNumber = (N25Q512A_FLASH_SIZE / N25Q512A_SUBSECTOR_SIZE);
	pInfo->ProgPageSize = N25Q512A_PAGE_SIZE;
	pInfo->ProgPagesNumber = (N25Q512A_FLASH_SIZE / N25Q512A_PAGE_SIZE);
	return MEMORY_OK;
}

/* MCU MSP Configuration Function Area ------------------------------ */

/**
 * @brief QSPI MSP Initialization
 * @param hqspi: QSPI handle
 * @param Params
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration
 *           - NVIC configuration for QSPI interrupt
 * @retval None
 */
void User_QSPI_MspInsit(QSPI_HandleTypeDef *hqspi, void *Params)
{
	/* HAL_QSPI_MspInit(&hqspi); Check PLZ */
	HAL_QSPI_MspInit(hqspi);
#if 0
	GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct =
	{ 0 };
	if (hqspi->Instance == QUADSPI)
	{
		/* USER CODE BEGIN QUADSPI_MspInit 0 */

		/* USER CODE END QUADSPI_MspInit 0 */

		/** Initializes the peripherals clock
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
		PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
			Error_Handler();
		}

		/* Peripheral clock enable */
		__HAL_RCC_QSPI_CLK_ENABLE();

		__HAL_RCC_GPIOF_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();
		/**QUADSPI GPIO Configuration
		 PF6     ------> QUADSPI_BK1_IO3
		 PF7     ------> QUADSPI_BK1_IO2
		 PF8     ------> QUADSPI_BK1_IO0
		 PF9     ------> QUADSPI_BK1_IO1
		 PB2     ------> QUADSPI_CLK
		 PG6     ------> QUADSPI_BK1_NCS
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		/* QUADSPI interrupt Init */
		HAL_NVIC_SetPriority(QUADSPI_IRQn, 15, 0);
		HAL_NVIC_EnableIRQ(QUADSPI_IRQn);
		/* USER CODE BEGIN QUADSPI_MspInit 1 */


		/* USER CODE END QUADSPI_MspInit 1 */
	}
#endif
}

/**
 * @brief QSPI MSP De-Initialization
 * @param hqspi: QSPI handle
 * @param Params
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO and NVIC configuration to their default state
 * @retval None
 */
void User_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi, void *Params)
{
	/* HAL_QSPI_MspDeInit(&hqspi); Check PLZ */
	HAL_QSPI_MspDeInit(hqspi);
#if 0
	if (hqspi->Instance == QUADSPI)
	{
		/* USER CODE BEGIN QUADSPI_MspDeInit 0 */
		__HAL_RCC_QSPI_FORCE_RESET();
		__HAL_RCC_QSPI_RELEASE_RESET();
		/* USER CODE END QUADSPI_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_QSPI_CLK_DISABLE();

		/**QUADSPI GPIO Configuration
		 PF6     ------> QUADSPI_BK1_IO3
		 PF7     ------> QUADSPI_BK1_IO2
		 PF8     ------> QUADSPI_BK1_IO0
		 PF9     ------> QUADSPI_BK1_IO1
		 PB2     ------> QUADSPI_CLK
		 PG6     ------> QUADSPI_BK1_NCS
		 */
		HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);

		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2);

		HAL_GPIO_DeInit(GPIOG, GPIO_PIN_6);

		/* QUADSPI interrupt DeInit */
		HAL_NVIC_DisableIRQ(QUADSPI_IRQn);
		/* USER CODE BEGIN QUADSPI_MspDeInit 1 */

		/* USER CODE END QUADSPI_MspDeInit 1 */
	}
#endif
}

/* Static Function Area ------------------------------ */
/**
 * @brief  This function reset the QSPI memory.
 * @param  hqspi: QSPI handle
 * @retval None
 */
static uint32_t User_QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi)
{
	QSPI_CommandTypeDef s_command;

	/* Initialize the reset enable command */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = RESET_ENABLE_CMD; /* n25q512a */
	s_command.AddressMode = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_NONE;
	s_command.DummyCycles = 0;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	/* Send the command */
	if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Send the reset memory command */
	s_command.Instruction = RESET_MEMORY_CMD; /* n25q512a */
	if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Configure automatic polling mode to wait the memory is ready */
	if (User_QSPI_AutoPollingMemReady(hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}

	return MEMORY_OK;
}

/**
 * @brief  This function set the QSPI memory in 4-byte address mode
 * @param  hqspi: QSPI handle
 * @retval None
 */
static uint32_t User_QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi)
{
	QSPI_CommandTypeDef s_command;

	/* Initialize the command */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = ENTER_4_BYTE_ADDR_MODE_CMD; /* n25q512a */
	s_command.AddressMode = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_NONE;
	s_command.DummyCycles = 0;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	/* Enable write operations */
	if (User_QSPI_WriteEnable(hqspi) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}

	/* Send the command */
	if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}

	/* Configure automatic polling mode to wait the memory is ready */
	if (User_QSPI_AutoPollingMemReady(hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}

	return MEMORY_OK;
}

/**
 * @brief  This function configure the dummy cycles on memory side.
 * @param  hqspi: QSPI handle
 * @retval None
 */
static uint32_t User_QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi)
{
	QSPI_CommandTypeDef s_command;
	uint16_t reg = 0;

	/* Initialize the read volatile configuration register command */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = READ_VOL_CFG_REG_CMD; /* n25q512a */
	s_command.AddressMode = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_1_LINE;
	s_command.DummyCycles = 0;
	s_command.NbData = 1;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	/* Configure the command */
	if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Reception of the data */
	if (HAL_QSPI_Receive(hqspi, (uint8_t*) (&reg), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Enable write operations */
	if (User_QSPI_WriteEnable(hqspi) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}

	/* Update volatile configuration register (with new dummy cycles) */
	s_command.Instruction = WRITE_VOL_CFG_REG_CMD; /* n25q512a */
	MODIFY_REG(reg, N25Q512A_VCR_NB_DUMMY, (N25Q512A_DUMMY_CYCLES_READ_QUAD << POSITION_VAL(N25Q512A_VCR_NB_DUMMY)));

	/* Configure the write volatile configuration register command */
	if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Transmission of the data */
	if (HAL_QSPI_Transmit(hqspi, (uint8_t*) (&reg), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	return MEMORY_OK;
}

/**
 * @brief  This function send a Write Enable and wait it is effective.
 * @param  hqspi: QSPI handle
 * @retval None
 */
static uint32_t User_QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi)
{
	QSPI_CommandTypeDef s_command;
	QSPI_AutoPollingTypeDef s_config;

	/* Enable write operations */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = WRITE_ENABLE_CMD; /* n25q512a */
	s_command.AddressMode = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_NONE;
	s_command.DummyCycles = 0;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* Configure automatic polling mode to wait for write enabling */
	s_config.Match = N25Q512A_SR_WREN; /* n25q512a */
	s_config.Mask = N25Q512A_SR_WREN; /* n25q512a */
	s_config.MatchMode = QSPI_MATCH_MODE_AND;
	s_config.StatusBytesSize = 1;
	s_config.Interval = 0x10;
	s_config.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

	s_command.Instruction = READ_STATUS_REG_CMD; /* n25q512a */
	s_command.DataMode = QSPI_DATA_1_LINE;

	if (HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	return MEMORY_OK;
}

/**
 * @brief  Configure the QSPI in memory-mapped mode
 * @retval QSPI memory status
 */
uint32_t User_QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi) // static -> non static
{
	QSPI_CommandTypeDef s_command;
	QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

	/* Configure the command for the read instruction */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = QUAD_OUT_FAST_READ_CMD; /* n25q512a */
	s_command.AddressMode = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize = QSPI_ADDRESS_32_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_4_LINES;
	s_command.DummyCycles = N25Q512A_DUMMY_CYCLES_READ_QUAD; /* n25q512a */
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	/* Configure the memory mapped mode */
	s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
	s_mem_mapped_cfg.TimeOutPeriod = 0;

	if (HAL_QSPI_MemoryMapped(hqspi, &s_command, &s_mem_mapped_cfg) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	return MEMORY_OK;
}

/**
 * @brief  De-Initializes and the QSPI interface.
 * @retval None
 */
static uint32_t User_QSPI_Shutdown(void)
{
	/* Call the DeInit function to reset the driver */
	if (HAL_QSPI_DeInit(&hqspi) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* System level De-initialization */
	// QSPI_MspDeInit();
	return MEMORY_OK;
}

/**
 * @brief  Copy an amount of data from the QSPI memory to destination memory.
 * @param  WriteAddr: Pointer to data to be read
 * @param  ReadAddr: Read start address
 * @param  Size: Size of data to read
 * @retval QSPI memory status
 */
static uint32_t User_QSPI_Copy(uint32_t WriteAddr, uint32_t ReadAddr, uint32_t Size)
{
	QSPI_CommandTypeDef s_command;

	/* Initialize the read command */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = FAST_READ_CMD; /* n25q512a */
	s_command.AddressMode = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_1_LINE;
	s_command.DummyCycles = N25Q512A_DUMMY_CYCLES_READ; /* n25q512a */
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	while (Size)
	{
		s_command.NbData = (Size < 256) ? Size : 256;
		s_command.Address = ReadAddr;

		/* Configure the command */
		if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			return MEMORY_ERROR;
		}

		/* Reception of the data */
		if (HAL_QSPI_Receive(&hqspi, (uint8_t*) WriteAddr, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			return MEMORY_ERROR;
		}

		if (Size > 256)
		{
			Size -= 256;
			WriteAddr += 256;
			ReadAddr += 256;
		}
		else
		{
			Size = 0;
		}
	}

	return MEMORY_OK;

}

/**
 * @brief  This function read the SR of the memory and wait the EOP.
 * @param  hqspi: QSPI handle
 * @param  Timeout
 * @retval None
 */
static uint32_t User_QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout)
{
	QSPI_CommandTypeDef s_command;
	QSPI_AutoPollingTypeDef s_config;

	/* Configure automatic polling mode to wait for memory ready */
	s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction = READ_STATUS_REG_CMD; /* n25q512a */
	s_command.AddressMode = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode = QSPI_DATA_1_LINE;
	s_command.DummyCycles = 0;
	s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	s_config.Match = 0;
	s_config.MatchMode = QSPI_MATCH_MODE_AND;
	s_config.Interval = 0x10;
	s_config.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;
	s_config.Mask = N25Q512A_SR_WIP; /* n25q512a */
	s_config.StatusBytesSize = 1;

	if (HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, Timeout) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	return MEMORY_OK;
}

/* USER CODE END 0 */

