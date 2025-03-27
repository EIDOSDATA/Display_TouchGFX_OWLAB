/* Includes ------------------------------------------------------------------*/
#include "user_mcp4251.h"

/* Private variables ---------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
 * Function Name :
 * Description   :
 * Parameters    : None
 * Return        : None
 * Description   :
 *******************************************************************************/
void MCP4251_configuration(void)
{
	//SPI2_CE0_HIGH();
	//SPI2_CE1_HIGH();
	//DelayMs(10);
	//SPIx_Init(SPI_2);
}

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

/*******************************************************************************
 * Function Name :
 * Description   :
 * Parameters    : None
 * Return        : None
 * Description   :
 *******************************************************************************/
uint8_t MCP4251_Read(uint8_t ch, uint8_t Addr, uint8_t reg)
{
	uint32_t SPITimeout = SPI_FLAG_TIMEOUT;
	I2C_FLAG_TIMEOUT
	SPI_FLAG_TXP

	if (ch == 1)
	{
		SPI2_CE0_LOW();

		SPI2_CE0_HIGH();
	}
	else
	{
		SPI2_CE1_LOW();

		SPI2_CE1_HIGH();
	}
	return 0;
}

/*******************************************************************************
 * Function Name :
 * Description   :
 * Parameters    : None
 * Return        : None
 * Description   :
 *******************************************************************************/
uint8_t SPI_transfer(uint8_t reg)
{
	uint32_t SPITimeout = SPI_FLAG_TIMEOUT;

	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		if ((SPITimeout--) == 0)
		{
			return 1;
		}
	}
	/* Send u8 through the SPI1 peripheral */
	SPI_FLAG_TXE
	HAL_SPI_GetState(hspi)
	//HAL_SPI_Transmit(hspi, pData, Size, Timeout);
	SPI_I2S_SendData(SPI2, (uint16_t) reg);

	return 0;
}

/*******************************************************************************
 * Function Name :
 * Description   :
 * Parameters    : None
 * Return        : None
 * Description   :
 *******************************************************************************/
void MCP4251_DigitalPotWiperIncrement(uint8_t potNum)
{
	uint8_t cmdu8 = 0x00;

	SPI2_CE0_LOW(); // CE 0 Low
	if (potNum)
	{
		cmdu8 = ADDRESS_WIPER_1 | COMMAND_INCREMENT;
		SPI_transfer(cmdu8);
	}
	else
	{
		cmdu8 = ADDRESS_WIPER_0 | COMMAND_INCREMENT;
		SPI_transfer(cmdu8);
	}
	SPI2_CE0_HIGH(); // CE 0 High
}

void MCP4251_DigitalPotWiperDecrement(uint8_t potNum)
{
	uint8_t cmdu8 = 0x00;
	SPI2_CE0_LOW(); // CE 0 Low
	if (potNum)
	{
		cmdu8 = ADDRESS_WIPER_1 | COMMAND_DECREMENT;
		SPI_transfer(cmdu8);
	}
	else
	{
		cmdu8 = ADDRESS_WIPER_0 | COMMAND_DECREMENT;
		SPI_transfer(cmdu8);
	}
	SPI2_CE0_HIGH(); // CE 0 High
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
		datau8 = (u8) (value & 0X00FF);
	}
	SPI2_CE0_LOW(); // CE 0 Low
	DelayUs(10);
	if (potNum)
	{
		cmdu8 = cmdu8 | ADDRESS_WIPER_1 | COMMAND_WRITE;
		SPI_transfer(cmdu8);
		SPI_transfer(datau8);
	}
	else
	{
		cmdu8 = cmdu8 | ADDRESS_WIPER_0 | COMMAND_WRITE;
		SPI_transfer(cmdu8);
		SPI_transfer(datau8);
	}
	DelayUs(10);
	DEBUGPRINT("cmdu8 %d, datau8 %d\r\n", cmdu8, datau8);
	SPI2_CE0_HIGH(); // CE 0 High
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
	SPI2_CE0_LOW(); // CE 0 Low
	if (potNum)
	{
		cmdu8 = ADDRESS_WIPER_1 | COMMAND_READ;
		hu8 = SPI_transfer(cmdu8);
		lu8 = SPI_transfer(DUMMY_DATA);
	}
	else
	{
		cmdu8 = ADDRESS_WIPER_0 | COMMAND_READ;
		hu8 = SPI_transfer(cmdu8);
		lu8 = SPI_transfer(DUMMY_DATA);
	}
	SPI2_CE0_HIGH(); // CE 0 High
	return ((uint16_t) hu8 << 8 | (uint16_t) lu8) & BITMASK_READ_DATA_REGISTER;
}

uint16_t MCP4251_DigitalPotReadStatusRegister()
{
	uint8_t cmdu8 = 0x00;
	uint8_t hu8 = 0x00;
	uint8_t lu8 = 0x00;
	SPI2_CE0_LOW(); // CE 0 Low
	cmdu8 = ADDRESS_STATUS | COMMAND_READ;
	hu8 = SPI_transfer(cmdu8);
	lu8 = SPI_transfer(DUMMY_DATA);
	SPI2_CE0_HIGH(); // CE 0 High
	return ((uint16_t) hu8 << 8 | (uint16_t) lu8) & BITMASK_READ_DATA_REGISTER;
}

uint16_t MCP4251_DigitalPotReadTconRegister()
{
	uint8_t cmdu8 = 0x00;
	uint8_t hu8 = 0x00;
	uint8_t lu8 = 0x00;
	SPI2_CE0_LOW(); // CE 0 Low
	cmdu8 = ADDRESS_TCON | COMMAND_READ;
	hu8 = SPI_transfer(cmdu8);
	lu8 = SPI_transfer(DUMMY_DATA);
	SPI2_CE0_HIGH(); // CE 0 High
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
		datau8 = (u8) (value & 0X00FF);
	}
	SPI2_CE0_LOW(); // CE 0 Low
	cmdu8 = cmdu8 | ADDRESS_TCON | COMMAND_WRITE;
	SPI_transfer(cmdu8);
	SPI_transfer(datau8);
	SPI2_CE0_HIGH(); // CE 0 High
}

void MCP4251_DigitalPotStartup(u8 potNum)
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

void MCP4251_DigitalPotShutdown(u8 potNum)
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
		]

		tconData = (uint16_t)hu8 << 8 | (uint16_t)lu8;
		MCP4251_DigitalPotWriteTconRegister(tconData);
	}
}

void MCP4251_DigitalPotTerminalBConnect(u8 potNum)
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

void MCP4251_DigitalPotTerminalBDisconnect(u8 potNum)
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

void MCP4251_DigitalPotTerminalAConnect(u8 potNum)
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

void MCP4251_DigitalPotTerminalADisconnect(u8 potNum)
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

void MCP4251_DigitalPotWiperConnect(u8 potNum)
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

void MCP4251_DigitalPotWiperDisconnect(u8 potNum)
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

/*******************************************************************************
 * Function Name :
 * Description   :
 * Parameters    : None
 * Return        : None
 * Description   :
 *******************************************************************************/
uint8_t MCP4251_Test(void)
{
	uint8_t i = 0;

	while (1)
	{
		for (i = 0; i <= 255; i++)
		{
			StatusLED1toggle();
			StatusLED2toggle();
			StatusLED3toggle();
			StatusLED4toggle();
			MCP4251_DigitalPotSetWiperPosition(1, i);
			DelayMs(1);
		}
	}
	return 0;
}
