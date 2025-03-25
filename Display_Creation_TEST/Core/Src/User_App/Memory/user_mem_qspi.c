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
extern QSPI_HandleTypeDef hqspi;
//QSPI_HandleTypeDef QSPIHandle;
QSPI_CommandTypeDef sCommand;
QSPI_AutoPollingTypeDef sConfig;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
uint32_t User_QSPI_Startup(uint32_t Mode);
uint32_t User_QSPI_DeInit(void);
uint32_t User_QSPI_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
uint32_t User_QSPI_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
uint32_t User_QSPI_Erase_Block(uint32_t BlockAddress);
uint32_t User_QSPI_Erase_Chip(void);
uint32_t User_QSPI_GetStatus(void);
uint32_t User_QSPI_GetInfo(User_QSPI_Info *pInfo);

static uint32_t User_QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi);
static uint32_t User_QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi);
static uint32_t User_QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi);
static uint32_t User_QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi);
static uint32_t User_QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi);
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
uint32_t User_QSPI_Startup(uint32_t Mode)
{
	hqspi.Instance = QUADSPI;

	/* Call the DeInit function to reset the driver */
	if (HAL_QSPI_DeInit(&hqspi) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* System level initialization */
	HAL_QSPI_MspInit(&hqspi);

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
		return MEMORY_ERROR;
	}

	/* Configuration of the dummy cycles on QSPI memory side */
	if (User_QSPI_DummyCyclesCfg(&hqspi) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}

	/* Enable MemoryMapped mode */
	if (User_QSPI_EnableMemoryMappedMode(&hqspi) != MEMORY_OK)
	{
		return MEMORY_ERROR;
	}
#elif (BINARY_AREA == USE_SPI_NOR)
  if (QSPI_Copy(APPLICATION_ADDRESS, BINARY_BASE_OFFSET, BINARY_SIZE) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  if (QSPI_Shutdown() != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }
#endif /* (CODE_AREA == USE_QSPI) */
	return MEMORY_OK;
}

/**
 * @brief  De-Initializes the QSPI interface.
 * @retval QSPI memory status
 */\
uint32_t User_QSPI_DeInit(void)
{
	hqspi.Instance = QUADSPI;

	/* Call the DeInit function to reset the driver */
	if (HAL_QSPI_DeInit(&hqspi) != HAL_OK)
	{
		return MEMORY_ERROR;
	}

	/* System level De-initialization */
	/* TODO : CHECK PLZ */
	BSP_QSPI_MspDeInit(&hqspi, NULL);

	return MEMORY_OK;
}

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
static uint32_t User_QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi)
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

