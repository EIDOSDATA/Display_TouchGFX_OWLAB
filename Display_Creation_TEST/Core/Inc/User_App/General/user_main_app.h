/*
 * main_app.h
 *
 *  Created on: Mar 6, 2025
 *      Author: user
 */

#ifndef __MAIN_APP_H
#define __MAIN_APP_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "type.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* Common Value :: System Value ------------------------------ */
/*
 * Message Print Stinrg
 * */

/*
 * Decimal Value
 * @brief : Hexa to Decimal / dec_1 ~ dec_100000000
 * */
typedef struct
{
uint32_t	dec_100M;  // 100,000,000 자리
	uint32_t dec_10M;// 10,000,000 자리
	uint32_t dec_1M;// 1,000,000 자리
	uint32_t dec_100K;// 100,000 자리
	uint32_t dec_10K;// 10,000 자리
	uint32_t dec_1K;// 1,000 자리
	uint32_t dec_100;// 100 자리
	uint32_t dec_10;// 10 자리
	uint32_t dec_1;// 1 자리
}DecimalDigits;
extern DecimalDigits digits;

/*
 * Touch Screen Control Key Value
 * */

/* EEPROM Control Value ------------------------------ */

/* Test Value ------------------------------ */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
extern void Test_EEPROM_Read(void);
extern void Test_Touch_Key_Event(void);
extern void Test_Image_Print(void);
extern void User_Update_Encoder_State(void);
extern void User_Encoder_Parameter_Read(void);
extern void User_Encoder_Parameter_Display(void);

extern void hex2dec(uint32_t hex);
extern void User_Display_Loading_Sequence(void);

extern void User_System_Init(void);
extern void User_Main_App(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* DecimalDigits digits Define */
#define DEC_100M	digits.dec_100M
#define DEC_10M		digits.dec_10M
#define DEC_1M		digits.dec_1M
#define DEC_100K	digits.dec_100K
#define DEC_10K		digits.dec_10K
#define DEC_1K		digits.dec_1K
#define DEC_100		digits.dec_100
#define DEC_10		digits.dec_10
#define DEC_1		digits.dec_1

#define PROGRAM_VERSION "V2025.03.24"
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_MAIN_APP_H_ */
