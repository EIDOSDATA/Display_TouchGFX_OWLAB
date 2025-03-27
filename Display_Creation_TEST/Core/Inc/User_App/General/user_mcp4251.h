/*
 * user_mcp4251.h
 *
 *  Created on: Mar 27, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_GENERAL_USER_MCP4251_H_
#define INC_USER_APP_GENERAL_USER_MCP4251_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum
{
	MCP4251_OK = 0x00, /* OK */
	MCP4251_SPI_ERROR, /* ERROR */
	MCP4251_INVALID_CHANNEL /* INVALID */
} MCP4251_Status;

typedef struct
{
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
	float max_resistance;
} MCP4251_Device;
extern MCP4251_Device pot; /* Declaration of Device Instance */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
extern MCP4251_Status User_MCP4251_Test_Fucntion(void);
extern MCP4251_Status User_MCP4251_Init(MCP4251_Device *dev, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, float max_res);
extern MCP4251_Status User_MCP4251_SetResistance(MCP4251_Device *dev, uint8_t channel, float resistance);
extern MCP4251_Status User_MCP4251_SetRaw(MCP4251_Device *dev, uint8_t channel, uint8_t value);
extern MCP4251_Status User_MCP4251_ReadRaw(MCP4251_Device *dev, uint8_t channel, uint8_t *value);
extern float User_MCP4251_ToResistance(MCP4251_Device *dev, uint8_t raw_value);

#if 0
/* User Test Function --------------------------------------------------*/
extern void User_MCP4251_Init(void);
extern void User_MCP4251_Write(uint8_t channel, uint8_t value);
extern uint8_t User_MCP4251_Read(uint8_t channel);
extern float User_MCP4251_Calc_Resistance(uint8_t value);
extern uint8_t User_MCP4251_Test(void);
#endif

#if 0
/* Fucking Arduino Function --------------------------------------------------*/
extern uint8_t MCP4251_SPI_transfer(uint8_t reg);
extern void MCP4251_DigitalPotWiperIncrement(uint8_t potNum);
extern void MCP4251_DigitalPotWiperDecrement(uint8_t potNum);
extern void MCP4251_DigitalPotSetWiperPosition(uint8_t potNum, uint32_t value);
extern void MCP4251_DigitalPotSetWiperMin(uint8_t potNum);
extern void MCP4251_DigitalPotSetWiperMax(uint8_t potNum);
extern void MCP4251_DigitalPotSetWiperMid(uint8_t potNum);

extern uint16_t MCP4251_DigitalPotReadWiperPosition(uint8_t potNum);
extern uint16_t MCP4251_DigitalPotReadStatusRegister(void);
extern uint16_t MCP4251_DigitalPotReadTconRegister(void);

extern void MCP4251_DigitalPotWriteTconRegister(uint16_t value);
extern void MCP4251_DigitalPotStartup(uint8_t potNum);
extern void MCP4251_DigitalPotShutdown(uint8_t potNum);
extern void MCP4251_DigitalPotTerminalBConnect(uint8_t potNum);
extern void MCP4251_DigitalPotTerminalBDisconnect(uint8_t potNum);
extern void MCP4251_DigitalPotTerminalAConnect(uint8_t potNum);
extern void MCP4251_DigitalPotTerminalADisconnect(uint8_t potNum);
extern void MCP4251_DigitalPotWiperConnect(uint8_t potNum);
extern void MCP4251_DigitalPotWiperDisconnect(uint8_t potNum);
extern void MCP4251_DigitalPotInitTcon();
#endif

//uint8_t MCP4251_Write(uint8_t ch, uint8_t Addr, uint8_t reg);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* 8BIT COMMAND
 * AD3 | AC2 | AD1 | AD0 | C1 | C0 | D9 | D8
 *
 * 16BIT COMMAND
 * AD3 | AC2 | AD1 | AD0 | C1 | C0 | D9 | D8 | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0
 *
 * Pin Control
 * C1	|	C0	|	Description
 * -------------------------
 * 0	|	0	|	WRITE DATA
 * 0	|	1	|	INCR
 * 1	|	0	|	DECR
 * 1	|	1	|	READ DATA
 * */
/* GPIO Macro */
#define MCP4251_CS_GPIO_Port    CON_JF1_MCP4251_CS_GPIO_Port
#define MCP4251_CS_Pin          CON_JF1_MCP4251_CS_Pin

/* Command Macro */
#define USER_MCP4251_WRITE_WIPER0       0x00
#define USER_MCP4251_WRITE_WIPER1       0x10
#define USER_MCP4251_READ_WIPER0        0x0C
#define USER_MCP4251_READ_WIPER1        0x1C

#define TCON0_OFF_TCON1_ON 			0xF0
#define TCON0_ON_TCON1_OFF 			0x0F
#define TCON0_OFF_TCON1_OFF 		0x00
#define TCON0_ON_TCON1_ON 			0xFF
#define MCP4251_FLAG_TIMEOUT 		0x1000

#if 0
/* Fucking Arduino Macro --------------------------------------------------*/
#define ADDRESS_WIPER_0 0x00
#define ADDRESS_WIPER_1 0x10
#define ADDRESS_TCON 	0x40
#define ADDRESS_STATUS 	0x50

#define COMMAND_WRITE 0x00
#define COMMAND_INCREMENT 0x04
#define COMMAND_DECREMENT 0x08
#define COMMAND_READ 0x0C

#define DUMMY_DATA 0xFF

#define BITMASK_READ_DATA_REGISTER 0x01FF //B0000000111111111
#define BITMASK_POT0_STARTUP 					0x08
#define BITMASK_POT1_STARTUP 					0x80
#define BITMASK_POT0_B_TERMINAL_CONNECT 		0x01
#define BITMASK_POT1_B_TERMINAL_CONNECT 		0x10
#define BITMASK_POT0_WIPER_TERMINAL_CONNECT 	0x02
#define BITMASK_POT1_WIPER_TERMINAL_CONNECT 	0x20
#define BITMASK_POT0_A_TERMINAL_CONNECT 		0x04
#define BITMASK_POT1_A_TERMINAL_CONNECT 		0x40
#endif

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_GENERAL_USER_MCP4251_H_ */

