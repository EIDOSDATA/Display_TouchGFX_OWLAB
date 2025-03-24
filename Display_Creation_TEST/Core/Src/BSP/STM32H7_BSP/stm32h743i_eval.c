/**
 ******************************************************************************
 * @file    stm32h743i_eval.c
 * @author  MCD Application Team
 * @version V1.2.0
 * @date    29-December-2017
 * @brief   This file provides a set of firmware functions to manage LEDs,
 *          push-buttons and COM ports available on STM32H743I-EVAL
 *          evaluation board(MB1219) from STMicroelectronics.
 *
 @verbatim
 This driver requires the stm32h743i_eval_io.c/.h files to manage the
 IO module resources mapped on the MFX IO expander.
 These resources are mainly LEDs, Joystick push buttons, SD detect pin,
 USB OTG power switch/over current drive pins, Audio
 INT pin
 @endverbatim
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "stm32h743i_eval.h"
#if defined(USE_IOEXPANDER)
#include "stm32h743i_eval_io.h"
#endif /* USE_IOEXPANDER */

/** @defgroup STM32H743I_EVAL_LOW_LEVEL_Private_Defines STM32H743I-EVAL LOW LEVEL Private Defines
 * @{
 */
/**
 * @brief STM32H743I EVAL BSP Driver version number V1.2.0
 */
#define __STM32H743I_EVAL_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32H743I_EVAL_BSP_VERSION_SUB1   (0x02) /*!< [23:16] sub1 version */
#define __STM32H743I_EVAL_BSP_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __STM32H743I_EVAL_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32H743I_EVAL_BSP_VERSION         ((__STM32H743I_EVAL_BSP_VERSION_MAIN << 24)\
                                             |(__STM32H743I_EVAL_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32H743I_EVAL_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32H743I_EVAL_BSP_VERSION_RC))
/**
 * @}
 */

I2C_HandleTypeDef heval_I2c;

#if UNUSED_BSP
static ADC_HandleTypeDef heval_ADC;
#endif

/** @defgroup STM32H743I_EVAL_LOW_LEVEL_Private_FunctionPrototypes  STM32H743I_EVAL LOW LEVEL Private Function Prototypes
 * @{
 */
static void I2Cx_MspInit(void);
static void I2Cx_Init(void);
static void I2Cx_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
static uint8_t I2Cx_Read(uint8_t Addr, uint8_t Reg);
static HAL_StatusTypeDef I2Cx_ReadMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length);
static HAL_StatusTypeDef I2Cx_WriteMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length);
static HAL_StatusTypeDef I2Cx_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);
static void I2Cx_Error(uint8_t Addr);

/* I2C EEPROM IO function */
void EEPROM_IO_Init(void);
HAL_StatusTypeDef EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

/* TouchScreen (TS) IO functions */
void TS_IO_Init(void);
void TS_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t TS_IO_Read(uint8_t Addr, uint8_t Reg);
uint16_t TS_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
void TS_IO_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
void TS_IO_Delay(uint32_t Delay);

void OTM8009A_IO_Delay(uint32_t Delay);
/**
 * @}
 */

/**
 * @brief  This method returns the STM32H743I EVAL BSP Driver revision
 * @retval version: 0xXYZR (8bits for each decimal, R for RC)
 */
uint32_t BSP_GetVersion(void)
{
	return __STM32H743I_EVAL_BSP_VERSION;
}

/*******************************************************************************
 BUS OPERATIONS
 *******************************************************************************/

/******************************* I2C Routines *********************************/
/**
 * @brief  Initializes I2C MSP.
 * @retval None
 */
static void I2Cx_MspInit(void)
{
	GPIO_InitTypeDef gpio_init_structure;
	RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

	/* Configure the I2C clock source */
	RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C123;
	RCC_PeriphClkInit.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
	HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

#if 0
	/* set STOPWUCK in RCC_CFGR */
	__HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);
#endif

	/*** Configure the GPIOs ***/
	/* Enable GPIO clock */
	EVAL_I2Cx_SCL_SDA_GPIO_CLK_ENABLE();

	/* Configure I2C Tx as alternate function */
	gpio_init_structure.Pin = EVAL_I2Cx_SCL_PIN;
	gpio_init_structure.Mode = GPIO_MODE_AF_OD;
	gpio_init_structure.Pull = GPIO_NOPULL;
	gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_init_structure.Alternate = EVAL_I2Cx_SCL_SDA_AF;
	HAL_GPIO_Init(EVAL_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);

	/* Configure I2C Rx as alternate function */
	gpio_init_structure.Pin = EVAL_I2Cx_SDA_PIN;
	HAL_GPIO_Init(EVAL_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);

	/*** Configure the I2C peripheral ***/
	/* Enable I2C clock */
	EVAL_I2Cx_CLK_ENABLE();

	/* Force the I2C peripheral clock reset */
	EVAL_I2Cx_FORCE_RESET();

	/* Release the I2C peripheral clock reset */
	EVAL_I2Cx_RELEASE_RESET();

	/* Enable and set I2Cx Interrupt to a lower priority */
	HAL_NVIC_SetPriority(EVAL_I2Cx_EV_IRQn, 0x0F, 0);
	HAL_NVIC_EnableIRQ(EVAL_I2Cx_EV_IRQn);

	/* Enable and set I2Cx Interrupt to a lower priority */
	HAL_NVIC_SetPriority(EVAL_I2Cx_ER_IRQn, 0x0F, 0);
	HAL_NVIC_EnableIRQ(EVAL_I2Cx_ER_IRQn);
}

/**
 * @brief  Initializes I2C HAL.
 * @retval None
 */
static void I2Cx_Init(void)
{
	if (HAL_I2C_GetState(&heval_I2c) == HAL_I2C_STATE_RESET)
	{
		heval_I2c.Instance = EVAL_I2Cx;
		heval_I2c.Init.Timing = EVAL_I2Cx_TIMING;
		heval_I2c.Init.OwnAddress1 = 0;
		heval_I2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		heval_I2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		heval_I2c.Init.OwnAddress2 = 0;
		heval_I2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		heval_I2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

		/* Init the I2C */
		I2Cx_MspInit();
		HAL_I2C_Init(&heval_I2c);
	}
}

/**
 * @brief  Writes a single data.
 * @param  Addr: I2C address
 * @param  Reg: Register address
 * @param  Value: Data to be written
 * @retval None
 */
static void I2Cx_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(&heval_I2c, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 100);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* Execute user timeout callback */
		I2Cx_Error(Addr);
	}
}

/**
 * @brief  Reads a single data.
 * @param  Addr: I2C address
 * @param  Reg: Register address
 * @retval Read data
 */
static uint8_t I2Cx_Read(uint8_t Addr, uint8_t Reg)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t Value = 0;

	status = HAL_I2C_Mem_Read(&heval_I2c, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 1000);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* Execute user timeout callback */
		I2Cx_Error(Addr);
	}
	return Value;
}

/**
 * @brief  Reads multiple data.
 * @param  Addr: I2C address
 * @param  Reg: Reg address
 * @param  MemAddress: memory address to be read
 * @param  Buffer: Pointer to data buffer
 * @param  Length: Length of the data
 * @retval Number of read data
 */
static HAL_StatusTypeDef I2Cx_ReadMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	if (Addr == EXC7200_I2C_ADDRESS)
	{
		status = HAL_I2C_Master_Receive(&heval_I2c, Addr, Buffer, Length, 1000);
	}
	else
	{
		status = HAL_I2C_Mem_Read(&heval_I2c, Addr, (uint16_t) Reg, MemAddress, Buffer, Length, 1000);
	}

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* I2C error occurred */
		I2Cx_Error(Addr);
	}
	return status;
}

/**
 * @brief  Writes a value in a register of the device through BUS in using DMA mode.
 * @param  Addr: Device address on BUS Bus.
 * @param  Reg: The target register address to write
 * @param  MemAddress: memory address to be written
 * @param  Buffer: The target register value to be written
 * @param  Length: buffer size to be written
 * @retval HAL status
 */
static HAL_StatusTypeDef I2Cx_WriteMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(&heval_I2c, Addr, (uint16_t) Reg, MemAddress, Buffer, Length, 1000);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* Re-Initiaize the I2C Bus */
		I2Cx_Error(Addr);
	}
	return status;
}

/**
 * @brief  Checks if target device is ready for communication.
 * @note   This function is used with Memory devices
 * @param  DevAddress: Target device address
 * @param  Trials: Number of trials
 * @retval HAL status
 */
static HAL_StatusTypeDef I2Cx_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{
	return (HAL_I2C_IsDeviceReady(&heval_I2c, DevAddress, Trials, 1000));
}

/**
 * @brief  Manages error callback by re-initializing I2C.
 * @param  Addr: I2C Address
 * @retval None
 */
static void I2Cx_Error(uint8_t Addr)
{
	/* De-initialize the I2C comunication bus */
	HAL_I2C_DeInit(&heval_I2c);

	/* Re-Initialize the I2C communication bus */
	I2Cx_Init();
}

/******************************** LINK I2C EEPROM *****************************/

/**
 * @brief  Initializes peripherals used by the I2C EEPROM driver.
 * @retval None
 */
void EEPROM_IO_Init(void)
{
	I2Cx_Init();
}

/**
 * @brief  Write data to I2C EEPROM driver in using DMA channel.
 * @param  DevAddress: Target device address
 * @param  MemAddress: Internal memory address
 * @param  pBuffer: Pointer to data buffer
 * @param  BufferSize: Amount of data to be sent
 * @retval HAL status
 */
HAL_StatusTypeDef EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize)
{
	return (I2Cx_WriteMultiple(DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pBuffer, BufferSize)); //////////////////////////////////////////8bit  //16bit
}

/**
 * @brief  Read data from I2C EEPROM driver in using DMA channel.
 * @param  DevAddress: Target device address
 * @param  MemAddress: Internal memory address
 * @param  pBuffer: Pointer to data buffer
 * @param  BufferSize: Amount of data to be read
 * @retval HAL status
 */
HAL_StatusTypeDef EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize)
{
	return (I2Cx_ReadMultiple(DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pBuffer, BufferSize)); //////////////////////////////////////////8bit   //16bit
}

/**
 * @brief  Checks if target device is ready for communication.
 * @note   This function is used with Memory devices
 * @param  DevAddress: Target device address
 * @param  Trials: Number of trials
 * @retval HAL status
 */
HAL_StatusTypeDef EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{
	return (I2Cx_IsDeviceReady(DevAddress, Trials));
}

/******************************** LINK TS (TouchScreen) *****************************/

/**
 * @brief  Initialize I2C communication
 *         channel from MCU to TouchScreen (TS).
 */
void TS_IO_Init(void)
{
	I2Cx_Init();
}

/**
 * @brief  Writes single data with I2C communication
 *         channel from MCU to TouchScreen.
 * @param  Addr: I2C address
 * @param  Reg: Register address
 * @param  Value: Data to be written
 */
void TS_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
	I2Cx_Write(Addr, Reg, Value);
}

/**
 * @brief  Reads single data with I2C communication
 *         channel from TouchScreen.
 * @param  Addr: I2C address
 * @param  Reg: Register address
 * @retval Read data
 */
uint8_t TS_IO_Read(uint8_t Addr, uint8_t Reg)
{
	return I2Cx_Read(Addr, Reg);
}

/**
 * @brief  Reads multiple data with I2C communication
 *         channel from TouchScreen.
 * @param  Addr: I2C address
 * @param  Reg: Register address
 * @param  Buffer: Pointer to data buffer
 * @param  Length: Length of the data
 * @retval Number of read data
 */
uint16_t TS_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
{
	return I2Cx_ReadMultiple(Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, Buffer, Length);
}

/**
 * @brief  Writes multiple data with I2C communication
 *         channel from MCU to TouchScreen.
 * @param  Addr: I2C address
 * @param  Reg: Register address
 * @param  Buffer: Pointer to data buffer
 * @param  Length: Length of the data
 * @retval None
 */
void TS_IO_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
{
	I2Cx_WriteMultiple(Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, Buffer, Length);
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
