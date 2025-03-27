#ifndef __USER_MCP4251_H
#define __USER_MCP4251_H

#include "main.h"

/* Private define ------------------------------------------------------------*/
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

/* TODO : Check PLZ */
#define TCON0_OFF_TCON1_ON 			0xF0
#define TCON0_ON_TCON1_OFF 			0x0F
#define TCON0_OFF_TCON1_OFF 		0x00
#define TCON0_ON_TCON1_ON 			0xFF
#define MCP4251_FLAG_TIMEOUT 		0x1000
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Function define ------------------------------------------------------------*/
void MCP4251_configuration(void);
uint8_t MCP4251_Write(uint8_t ch, uint8_t Addr, uint8_t reg);
uint8_t MCP4251_Test(void);

#endif //__USER_MCP4251_H
