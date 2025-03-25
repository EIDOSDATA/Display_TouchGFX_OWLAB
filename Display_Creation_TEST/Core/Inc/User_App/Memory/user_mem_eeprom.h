/*
 * user_mem_eeprom.h
 *
 *  Created on: Mar 7, 2025
 *      Author: user
 */
#ifndef INC_USER_APP_MEMORY_USER_NEN_EEPROM_DATA_H_
#define INC_USER_APP_MEMORY_USER_NEN_EEPROM_DATA_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32h743i_eval_lcd.h"
#include "stm32h743i_eval_eeprom.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct
{
	uint8_t RadialKeyMode; // 라디알 키 모드 선택 : 0 = 3초 눌려야 연속 동작 , 1 = 짧게 눌러도 연속 동작.
	uint8_t Flag[5];//"FACT"  //
}_EEPROM_FACTORY_t;

typedef struct
{
	uint8_t KeyVolume;
	uint8_t CompleteVolume;
	uint8_t BacklightBright;
	uint8_t FactoryModeEnterTime;
	uint8_t Dummy[5]; // 기존 flag "INIT" 저장되어 있는 영역.//호환성위해 남김.

	uint32_t FocusTotalCnt[10];
	uint32_t FocusHandCnt[10];
	uint32_t RadialTotalCnt[10];
	uint32_t RadialHandCnt[10];
	uint8_t FocusCntIdx;
	uint8_t RadialCntIdx;
	uint8_t Flag[5];

	_EEPROM_FACTORY_t Factory;

}_EERPOM_t;

extern _EERPOM_t EepData;
extern _EERPOM_t *EepAddr;

/*
 * EEPROM DATA
 * */
extern uint32_t total_eepdata_32[];
extern uint16_t radial_eepdata_16[];
extern uint16_t focus_eepdata_16[];
extern uint16_t focus_setting_16[]; /* Unused Value */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
extern void User_EEPROM_Init(void);
extern void User_EEPROM_DataInit(void);
extern void User_EEPROM_Error(void);

/*
 * Read selected memory address group :: EEPROM
 * */
extern uint8_t User_EEPROM_CountSave(uint8_t idx, uint32_t eep_data);
extern void User_Main_Read_Settings_From_EEPROM(void);
extern void User_Save_HandpieceUsageCountToEEPROM(void);
extern void User_Read_EEPROM_Settings(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_MEMORY_USER_NEN_EEPROM_DATA_H_ */
