/* USER CODE BEGIN Header */
/*
 * user_mcp4251.c
 *
 *  Created on: Mar 27, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_mcp4251.h"

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
extern SPI_HandleTypeDef hspi6;
MCP4251_Device pot; /* Declaration of Device Instance */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
MCP4251_Status User_MCP4251_Test_Fucntion(void);
MCP4251_Status User_MCP4251_Init(MCP4251_Device *dev, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, float max_res);
MCP4251_Status User_MCP4251_SetResistance(MCP4251_Device *dev, uint8_t channel, float resistance);
MCP4251_Status User_MCP4251_SetRaw(MCP4251_Device *dev, uint8_t channel, uint8_t value);
MCP4251_Status User_MCP4251_ReadRaw(MCP4251_Device *dev, uint8_t channel, uint8_t *value);
float User_MCP4251_ToResistance(MCP4251_Device *dev, uint8_t raw_value);
static MCP4251_Status User_Send_Command_Raw(MCP4251_Device *dev, uint8_t cmd, uint8_t data);

#if 0
/* User Test Function --------------------------------------------------*/
void User_MCP4251_Init(void);
void User_MCP4251_Write(uint8_t channel, uint8_t value);
uint8_t User_MCP4251_Read(uint8_t channel);
float User_Calc_Resistance(uint8_t value);
uint8_t User_MCP4251_Test(void);
#endif

#if 0
/* Fucking Arduino Function --------------------------------------------------*/
uint8_t MCP4251_SPI_transfer(uint8_t reg);
void MCP4251_DigitalPotWiperIncrement(uint8_t potNum);
void MCP4251_DigitalPotWiperDecrement(uint8_t potNum);
void MCP4251_DigitalPotSetWiperPosition(uint8_t potNum, uint32_t value);
void MCP4251_DigitalPotSetWiperMin(uint8_t potNum);
void MCP4251_DigitalPotSetWiperMax(uint8_t potNum);
void MCP4251_DigitalPotSetWiperMid(uint8_t potNum);

uint16_t MCP4251_DigitalPotReadWiperPosition(uint8_t potNum);
uint16_t MCP4251_DigitalPotReadStatusRegister(void);
uint16_t MCP4251_DigitalPotReadTconRegister(void);

void MCP4251_DigitalPotWriteTconRegister(uint16_t value);
void MCP4251_DigitalPotStartup(uint8_t potNum);
void MCP4251_DigitalPotShutdown(uint8_t potNum);
void MCP4251_DigitalPotTerminalBConnect(uint8_t potNum);
void MCP4251_DigitalPotTerminalBDisconnect(uint8_t potNum);
void MCP4251_DigitalPotTerminalAConnect(uint8_t potNum);
void MCP4251_DigitalPotTerminalADisconnect(uint8_t potNum);
void MCP4251_DigitalPotWiperConnect(uint8_t potNum);
void MCP4251_DigitalPotWiperDisconnect(uint8_t potNum);
void MCP4251_DigitalPotInitTcon();
#endif
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

MCP4251_Status User_MCP4251_Test_Fucntion(void)
{
	/* Example 2: Breathing Light Effect (Channel 1) */
	for (uint8_t val = 0; val < 255; val++)
	{
		User_MCP4251_SetRaw(&pot, 1, val);
		HAL_Delay(10);
	}

	for (uint8_t val = 255; val > 0; val--)
	{
		User_MCP4251_SetRaw(&pot, 1, val);
		HAL_Delay(10);
	}

#if 0
	/* Read current value verification */
	uint8_t raw;
	if (MCP4251_ReadRaw(&pot, 1, &raw) == MCP4251_OK)
	{
		float res = MCP4251_ToResistance(&pot, raw);
		printf("Current: %.1f Ω\n", res); /* Need to implement serial port OR LCD Print */
	}
#endif

	return MCP4251_OK;
}

/* Initialize the device */
MCP4251_Status User_MCP4251_Init(MCP4251_Device *dev, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, float max_res)
{
	if (!dev || !hspi || max_res <= 0)
	{
		return MCP4251_INVALID_CHANNEL;
	}

	dev->hspi = hspi;
	dev->cs_port = cs_port;
	dev->cs_pin = cs_pin;
	dev->max_resistance = max_res;

	/* Initial CS pin is at a high level */
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
	return MCP4251_OK;
}

/* Set the actual resistance value (unit: ohms) */
MCP4251_Status User_MCP4251_SetResistance(MCP4251_Device *dev, uint8_t channel, float resistance)
{
	if (resistance < 75.0f || resistance > dev->max_resistance)
	{
		return MCP4251_INVALID_CHANNEL;
	}

	/* Calculate the original value: R = 75 + (max_res * value) / 255 */
	float raw = ((resistance - 75.0f) * 255.0f) / dev->max_resistance;
	uint8_t value = (uint8_t) fminf(fmaxf(raw, 0.0f), 255.0f);

	return User_MCP4251_SetRaw(dev, channel, value);
}

/* Directly write to the 8-bit original value */
MCP4251_Status User_MCP4251_SetRaw(MCP4251_Device *dev, uint8_t channel, uint8_t value)
{
	uint8_t cmd = (channel == 0) ? USER_MCP4251_WRITE_WIPER0 : USER_MCP4251_WRITE_WIPER1;
	return User_Send_Command_Raw(dev, cmd, value);
}

/* Read the current original value */
MCP4251_Status User_MCP4251_ReadRaw(MCP4251_Device *dev, uint8_t channel, uint8_t *value)
{
	if (!value)
	{
		return MCP4251_INVALID_CHANNEL;
	}

	uint8_t cmd = (channel == 0) ? USER_MCP4251_READ_WIPER0 : USER_MCP4251_READ_WIPER1;
	uint8_t rx_data =
	{ 0 };
	HAL_StatusTypeDef hal_status;

	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
	hal_status = HAL_SPI_TransmitReceive(dev->hspi, &cmd, rx_data, 2, 100); // &rx_data
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);

	if (hal_status != HAL_OK)
	{
		return MCP4251_SPI_ERROR;
	}

	*value = rx_data; /* The second byte is the data. */
	return MCP4251_OK;
}

/* Convert the original value to actual resistance */
float User_MCP4251_ToResistance(MCP4251_Device *dev, uint8_t raw_value)
{
	return 75.0f + (dev->max_resistance * (float) raw_value) / 255.0f;
}

/* Private function: send command + data */
static MCP4251_Status User_Send_Command_Raw(MCP4251_Device *dev, uint8_t cmd, uint8_t data)
{
	uint8_t tx_buf =
	{ cmd, data };
	HAL_StatusTypeDef hal_status;

	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
	hal_status = HAL_SPI_Transmit(dev->hspi, tx_buf, sizeof(tx_buf), 100);
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
	return (hal_status == HAL_OK) ? MCP4251_OK : MCP4251_SPI_ERROR;
}

#if 0
/* User Test Function --------------------------------------------------*/
void User_MCP4251_Init(void)
{
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	MX_SPI6_Init();
}

/*
 * uint8_t cmd = 0x01 | ((channel & 0x01) << 4);
 * 	- 0x01 | 			: Basic command writing
 * 	- | (channel << 4) 	: Channel selection bit (bit4-5)
 *
 * Resistance value range
 * 	- MCP4251 has an 8-bit resolution, with a valid range of 0 to 255.
 * 	- Corresponding resistance formula: (R_serial is a fixed series resistor, approximately 75 ohms)
 * 	- R_wiper = (R_total / 255) * value + R_serial
 * */
/* Write to MCP4251 designated channel */
void User_MCP4251_Write(uint8_t channel, uint8_t value)
{
#if 0
	uint8_t tx_data;

	tx_data = (channel == 0) ? USER_WRITE_WIPER0 : USER_WRITE_WIPER1;
	tx_data = value;

	HAL_GPIO_WritePin(MCP4251_CS_PORT, MCP4251_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, tx_data, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(MCP4251_CS_PORT, MCP4251_CS_PIN, GPIO_PIN_SET);
#endif
	/* Command byte format: 0b00CC0001 (CC=channel: 00=channel 0, 01=channel 1) */
	uint8_t cmd = 0x01 | ((channel & 0x01) << 4);

	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_RESET); /* CS PIN LOW */
	if (HAL_SPI_Transmit(&hspi6, &cmd, 1, 10) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_SPI_Transmit(&hspi6, &value, 1, 10) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_SET); /* CS PIN HIGH */
}

/* Read the current potentiometer value (requires MISO connection) */
uint8_t User_MCP4251_Read(uint8_t channel)
{
#if 0
	uint8_t tx_data =
	{ 0 };
	uint8_t rx_data =
	{ 0 };

	tx_data = (channel == 0) ? USER_READ_WIPER0 : USER_READ_WIPER1;

	HAL_GPIO_WritePin(MCP4251_CS_PORT, MCP4251_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(MCP4251_CS_PORT, MCP4251_CS_PIN, GPIO_PIN_SET);
	return rx_data; /* Return the second byte of data */
#endif

	uint8_t cmd = 0x0C | ((channel & 0x01) << 4); /* Read the command */
	uint8_t rx_data =
	{ 0 };
	// uint8_t vrx_data[2];

	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi6, &cmd, 1, 10);
	HAL_SPI_Receive(&hspi6, rx_data, 2, 10);
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_SET);

	return rx_data;
	//return rx_data[1]; /* Return the second byte of data */
}

float User_Calc_Resistance(uint8_t value)
{
	return 75.0 + (10000.0 * value) / 255.0; /* 75Ω fixed resistor + adjustable portion */
}

uint8_t User_MCP4251_Test(void)
{
	for (uint8_t i = 0; i < 255; i++)
	{
		User_MCP4251_Write(0, i);
		HAL_Delay(10);
	}
	for (uint8_t i = 255; i > 0; i--)
	{
		User_MCP4251_Write(0, i);
		HAL_Delay(10);
	}

#if 0
	while (1)
	{
		for (uint8_t i = 0; i <= 255; i++)
		{
			MCP4251_DigitalPotSetWiperPosition(1, i);
			HAL_Delay(1);
		}
	}
#endif
	return 0;
}
#endif

#if 0
/* Fucking Arduino Function --------------------------------------------------*/
/*
 * 00h - Volatile Wiper 0
 * 				Write Data 0000 00xx xxxx xxxx
 * 				Read Data 0000 11xx xxxx xxxx
 * 				Increment Wiper 0000 0100
 * 				Decrement Wiper 0000 1000
 * 01h - Volatile Wiper 1
 * 				Write Data 0001 00xx xxxx xxxx
 * 				Read Data 0001 11xx xxxx xxxx
 * 				Increment Wiper 0001 0100
 * 				Decrement Wiper 0001 1000
 * 02h - Reserved
 * 03h - Reserved
 * 04h - Volatile TCON
 * 				Write Data 0100 00xx xxxx xxxx
 * 				Read Data 0100 11xx xxxx xxxx
 * 05h - Status Register
 * 				Read Data 0101 11xx xxxx xxxx
 * 06h ~ 0Fh - Reserved
 * */

/*
 #define MCP4131_WRITE		(0x00 << 2)
 #define MCP4131_READ		(0x03 << 2)
 #define MCP4131_WIPER_SHIFT	4
 int address = chan->channel << MCP4131_WIPER_SHIFT;

 --Write--
 data->buf[0] = address << MCP4131_WIPER_SHIFT;
 data->buf[0] |= MCP4131_WRITE | (val >> 8);
 data->buf[1] = val & 0xFF;

 --Read--
 data->buf[0] = (address << MCP4131_WIPER_SHIFT) | MCP4131_READ;
 data->buf[1] = 0;
 */

uint8_t MCP4251_SPI_transfer(uint8_t reg)
{
	uint32_t SPITimeout = MCP4251_FLAG_TIMEOUT;

	/* Loop while DR register in not emplty */
	while (HAL_SPI_GetState(&hspi6) == HAL_SPI_STATE_RESET)
	{
		if ((SPITimeout--) == 0)
		{
			return HAL_SPI_STATE_ERROR;
		}
	}

	/* Send u8 through the SPI1 peripheral */
	if (HAL_SPI_Transmit(&hspi6, &reg, 1, 10) != HAL_OK)
	{
		Error_Handler();
	}

	return 0;
}

void MCP4251_DigitalPotWiperIncrement(uint8_t potNum)
{
	uint8_t cmdu8 = 0x00;

	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_RESET); /* CS PIN LOW */
	if (potNum)
	{
		cmdu8 = ADDRESS_WIPER_1 | COMMAND_INCREMENT;
		MCP4251_SPI_transfer(cmdu8);
	}
	else
	{
		cmdu8 = ADDRESS_WIPER_0 | COMMAND_INCREMENT;
		MCP4251_SPI_transfer(cmdu8);
	}
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_SET); /* CS PIN HIGH */
}

void MCP4251_DigitalPotWiperDecrement(uint8_t potNum)
{
	uint8_t cmdu8 = 0x00;
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_RESET); /* CS PIN LOW */
	if (potNum)
	{
		cmdu8 = ADDRESS_WIPER_1 | COMMAND_DECREMENT;
		MCP4251_SPI_transfer(cmdu8);
	}
	else
	{
		cmdu8 = ADDRESS_WIPER_0 | COMMAND_DECREMENT;
		MCP4251_SPI_transfer(cmdu8);
	}
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_SET); /* CS PIN HIGH */
}

void MCP4251_DigitalPotSetWiperPosition(uint8_t potNum, uint32_t value)
{
	uint8_t cmdu8 = 0x00;
	uint8_t datau8 = 0x00;
	if (value > 255)
	{
		cmdu8 |= 0x01;
	}
	else
	{
		datau8 = (uint8_t) (value & 0X00FF);
	}
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_RESET); /* CS PIN LOW */
	HAL_Delay(10);
	if (potNum)
	{
		cmdu8 = cmdu8 | ADDRESS_WIPER_1 | COMMAND_WRITE;
		MCP4251_SPI_transfer(cmdu8);
		MCP4251_SPI_transfer(datau8);
	}
	else
	{
		cmdu8 = cmdu8 | ADDRESS_WIPER_0 | COMMAND_WRITE;
		MCP4251_SPI_transfer(cmdu8);
		MCP4251_SPI_transfer(datau8);
	}
	HAL_Delay(10);
	//DEBUGPRINT("cmdu8 %d, datau8 %d\r\n", cmdu8, datau8);

	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_SET); /* CS PIN HIGH */
}

void MCP4251_DigitalPotSetWiperMin(uint8_t potNum)
{
	if (potNum)
	{
		MCP4251_DigitalPotSetWiperPosition(1, 0);
	}
	else
	{
		MCP4251_DigitalPotSetWiperPosition(0, 0);
	}
}

void MCP4251_DigitalPotSetWiperMax(uint8_t potNum)
{
	if (potNum)
	{
		MCP4251_DigitalPotSetWiperPosition(1, 256);
	}
	else
	{
		MCP4251_DigitalPotSetWiperPosition(0, 256);
	}
}

void MCP4251_DigitalPotSetWiperMid(uint8_t potNum)
{
	if (potNum)
	{
		MCP4251_DigitalPotSetWiperPosition(1, 128);
	}
	else
	{
		MCP4251_DigitalPotSetWiperPosition(0, 128);
	}
}

uint16_t MCP4251_DigitalPotReadWiperPosition(uint8_t potNum)
{
	uint8_t cmdu8 = 0x00;
	uint8_t hu8 = 0x00;
	uint8_t lu8 = 0x00;
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_RESET); /* CS PIN LOW */
	if (potNum)
	{
		cmdu8 = ADDRESS_WIPER_1 | COMMAND_READ;
		hu8 = MCP4251_SPI_transfer(cmdu8);
		lu8 = MCP4251_SPI_transfer(DUMMY_DATA);
	}
	else
	{
		cmdu8 = ADDRESS_WIPER_0 | COMMAND_READ;
		hu8 = MCP4251_SPI_transfer(cmdu8);
		lu8 = MCP4251_SPI_transfer(DUMMY_DATA);
	}
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_SET); /* CS PIN HIGH */
	return ((uint16_t) hu8 << 8 | (uint16_t) lu8) & BITMASK_READ_DATA_REGISTER;
}

uint16_t MCP4251_DigitalPotReadStatusRegister(void)
{
	uint8_t cmdu8 = 0x00;
	uint8_t hu8 = 0x00;
	uint8_t lu8 = 0x00;
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_RESET); /* CS PIN LOW */
	cmdu8 = ADDRESS_STATUS | COMMAND_READ;
	hu8 = MCP4251_SPI_transfer(cmdu8);
	lu8 = MCP4251_SPI_transfer(DUMMY_DATA);
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_SET); /* CS PIN HIGH */
	return ((uint16_t) hu8 << 8 | (uint16_t) lu8) & BITMASK_READ_DATA_REGISTER;
}

uint16_t MCP4251_DigitalPotReadTconRegister(void)
{
	uint8_t cmdu8 = 0x00;
	uint8_t hu8 = 0x00;
	uint8_t lu8 = 0x00;
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_RESET); /* CS PIN LOW */
	cmdu8 = ADDRESS_TCON | COMMAND_READ;
	hu8 = MCP4251_SPI_transfer(cmdu8);
	lu8 = MCP4251_SPI_transfer(DUMMY_DATA);
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_SET); /* CS PIN HIGH */
	return ((uint16_t) hu8 << 8 | (uint16_t) lu8) & BITMASK_READ_DATA_REGISTER;
}

void MCP4251_DigitalPotWriteTconRegister(uint16_t value)
{
	uint8_t cmdu8 = 0x00;
	uint8_t datau8 = 0x00;
	if (value > 255)
	{
		cmdu8 |= 0x01;
	}
	else
	{
		datau8 = (uint8_t) (value & 0X00FF);
	}
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_RESET); /* CS PIN LOW */
	cmdu8 = cmdu8 | ADDRESS_TCON | COMMAND_WRITE;
	MCP4251_SPI_transfer(cmdu8);
	MCP4251_SPI_transfer(datau8);
	HAL_GPIO_WritePin(MCP4251_CS_GPIO_Port, MCP4251_CS_Pin, GPIO_PIN_SET); /* CS PIN HIGH */
}

void MCP4251_DigitalPotStartup(uint8_t potNum)
{
	uint16_t tconData = MCP4251_DigitalPotReadTconRegister();
	uint8_t hu8 = (uint8_t) tconData >> 8;
	uint8_t lu8 = (uint8_t) tconData & 0xff;

	if (potNum)
	{
		lu8 = lu8 | BITMASK_POT1_STARTUP;
	}
	else
	{
		lu8 = lu8 | BITMASK_POT0_STARTUP;
	}

	tconData = (uint16_t) hu8 << 8 | (uint16_t) lu8;
	MCP4251_DigitalPotWriteTconRegister(tconData);
}

void MCP4251_DigitalPotShutdown(uint8_t potNum)
{
	uint16_t tconData = MCP4251_DigitalPotReadTconRegister();
	uint8_t hu8 = (uint8_t) tconData >> 8;
	uint8_t lu8 = (uint8_t) tconData & 0xff;

	if (potNum)
	{
		lu8 = lu8 & ~BITMASK_POT1_STARTUP;
	}
	else
	{
		lu8 = lu8 & ~BITMASK_POT0_STARTUP;

		tconData = (uint16_t) hu8 << 8 | (uint16_t) lu8;
		MCP4251_DigitalPotWriteTconRegister(tconData);
	}
}

void MCP4251_DigitalPotTerminalBConnect(uint8_t potNum)
{
	uint16_t tconData = MCP4251_DigitalPotReadTconRegister();
	uint8_t hu8 = (uint8_t) tconData >> 8;
	uint8_t lu8 = (uint8_t) tconData & 0xff;

	if (potNum)
	{
		lu8 = lu8 | BITMASK_POT1_B_TERMINAL_CONNECT;
	}
	else
	{
		lu8 = lu8 | BITMASK_POT0_B_TERMINAL_CONNECT;
	}

	tconData = (uint16_t) hu8 << 8 | (uint16_t) lu8;
	MCP4251_DigitalPotWriteTconRegister(tconData);
}

void MCP4251_DigitalPotTerminalBDisconnect(uint8_t potNum)
{
	uint16_t tconData = MCP4251_DigitalPotReadTconRegister();
	uint8_t hu8 = (uint8_t) tconData >> 8;
	uint8_t lu8 = (uint8_t) tconData & 0xff;

	if (potNum)
	{
		lu8 = lu8 & ~BITMASK_POT1_B_TERMINAL_CONNECT;
	}
	else
	{
		lu8 = lu8 & ~BITMASK_POT0_B_TERMINAL_CONNECT;
	}

	tconData = (uint16_t) hu8 << 8 | (uint16_t) lu8;
	MCP4251_DigitalPotWriteTconRegister(tconData);
}

void MCP4251_DigitalPotTerminalAConnect(uint8_t potNum)
{
	uint16_t tconData = MCP4251_DigitalPotReadTconRegister();
	uint8_t hu8 = (uint8_t) tconData >> 8;
	uint8_t lu8 = (uint8_t) tconData & 0xff;

	if (potNum)
	{
		lu8 = lu8 | BITMASK_POT1_A_TERMINAL_CONNECT;
	}
	else
	{
		lu8 = lu8 | BITMASK_POT0_A_TERMINAL_CONNECT;
	}

	tconData = (uint16_t) hu8 << 8 | (uint16_t) lu8;
	MCP4251_DigitalPotWriteTconRegister(tconData);
}

void MCP4251_DigitalPotTerminalADisconnect(uint8_t potNum)
{
	uint16_t tconData = MCP4251_DigitalPotReadTconRegister();
	uint8_t hu8 = (uint8_t) tconData >> 8;
	uint8_t lu8 = (uint8_t) tconData & 0xff;

	if (potNum)
	{
		lu8 = lu8 & ~BITMASK_POT1_A_TERMINAL_CONNECT;
	}
	else
	{
		lu8 = lu8 & ~BITMASK_POT0_A_TERMINAL_CONNECT;
	}

	tconData = (uint16_t) hu8 << 8 | (uint16_t) lu8;
	MCP4251_DigitalPotWriteTconRegister(tconData);
}

void MCP4251_DigitalPotWiperConnect(uint8_t potNum)
{
	uint16_t tconData = MCP4251_DigitalPotReadTconRegister();
	uint8_t hu8 = (uint8_t) tconData >> 8;
	uint8_t lu8 = (uint8_t) tconData & 0xff;

	if (potNum)
	{
		lu8 = lu8 | BITMASK_POT1_WIPER_TERMINAL_CONNECT;
	}
	else
	{
		lu8 = lu8 | BITMASK_POT0_WIPER_TERMINAL_CONNECT;
	}

	tconData = (uint16_t) hu8 << 8 | (uint16_t) lu8;
	MCP4251_DigitalPotWriteTconRegister(tconData);
}

void MCP4251_DigitalPotWiperDisconnect(uint8_t potNum)
{
	uint16_t tconData = MCP4251_DigitalPotReadTconRegister();
	uint8_t hu8 = (uint8_t) tconData >> 8;
	uint8_t lu8 = (uint8_t) tconData & 0xff;

	if (potNum)
	{
		lu8 = lu8 & ~BITMASK_POT1_WIPER_TERMINAL_CONNECT;
	}
	else
	{
		lu8 = lu8 & ~BITMASK_POT0_WIPER_TERMINAL_CONNECT;
	}

	tconData = (uint16_t) hu8 << 8 | (uint16_t) lu8;
	MCP4251_DigitalPotWriteTconRegister(tconData);
}

void MCP4251_DigitalPotInitTcon()
{
	uint16_t tconData = MCP4251_DigitalPotReadTconRegister();
	uint8_t hu8 = (uint8_t) tconData >> 8;
	uint8_t lu8 = (uint8_t) tconData & 0xff;

	lu8 = lu8 | DUMMY_DATA;

	tconData = (uint16_t) hu8 << 8 | (uint16_t) lu8;
	MCP4251_DigitalPotWriteTconRegister(tconData);
}
#endif
/* USER CODE END 0 */

