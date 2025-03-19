
#ifndef __TYPE_H
#define __TYPE_H

/* Includes ------------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/

/* Type declarations ---------------------------------------------------------*/
// typedef unsigned char   BOOL;

typedef signed char s8;
typedef unsigned char u8;
typedef volatile signed char vs8;
typedef volatile unsigned char vu8;

typedef signed short s16;
typedef unsigned short u16;
typedef volatile signed short vs16;
typedef volatile unsigned short vu16;

typedef signed long s32;
typedef unsigned long u32;
typedef volatile signed long vs32;
typedef volatile unsigned long vu32;

typedef struct
{
	unsigned char Bit0 : 1;
	unsigned char Bit1 : 1;
	unsigned char Bit2 : 1;
	unsigned char Bit3 : 1;
	unsigned char Bit4 : 1;
	unsigned char Bit5 : 1;
	unsigned char Bit6 : 1;
	unsigned char Bit7 : 1;
} _BIT8_t;

typedef struct
{
	unsigned short Bit0 : 1;
	unsigned short Bit1 : 1;
	unsigned short Bit2 : 1;
	unsigned short Bit3 : 1;
	unsigned short Bit4 : 1;
	unsigned short Bit5 : 1;
	unsigned short Bit6 : 1;
	unsigned short Bit7 : 1;
	unsigned short Bit8 : 1;
	unsigned short Bit9 : 1;
	unsigned short Bit10 : 1;
	unsigned short Bit11 : 1;
	unsigned short Bit12 : 1;
	unsigned short Bit13 : 1;
	unsigned short Bit14 : 1;
	unsigned short Bit15 : 1;
} _BIT16_t;

typedef struct
{
	unsigned char Flag1 : 1;
	unsigned char Flag2 : 1;
	unsigned char Flag3 : 1;
	unsigned char Flag4 : 1;
	unsigned char Flag5 : 1;
	unsigned char Flag6 : 1;
	unsigned char Flag7 : 1;
	unsigned char Flag8 : 1;
} _FLAG8_t;

typedef struct
{
	unsigned short Flag1 : 1;
	unsigned short Flag2 : 1;
	unsigned short Flag3 : 1;
	unsigned short Flag4 : 1;
	unsigned short Flag5 : 1;
	unsigned short Flag6 : 1;
	unsigned short Flag7 : 1;
	unsigned short Flag8 : 1;
	unsigned short Flag9 : 1;
	unsigned short Flag10 : 1;
	unsigned short Flag11 : 1;
	unsigned short Flag12 : 1;
	unsigned short Flag13 : 1;
	unsigned short Flag14 : 1;
	unsigned short Flag15 : 1;
	unsigned short Flag16 : 1;
} _FLAG16_t;

/* External variable declarations --------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

#endif
