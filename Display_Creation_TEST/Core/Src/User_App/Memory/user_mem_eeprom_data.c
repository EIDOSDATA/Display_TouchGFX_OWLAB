/* USER CODE BEGIN Header */
/*
 * user_mem_eeprom_data.c
 *
 *  Created on: Mar 5, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_mem_eeprom_data.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user_disp_lcd.h"

#include "user_buzzer.h"
#include "stm32h743i_eval_lcd.h"
#include "stm32h743i_eval_eeprom.h"

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
_EERPOM_t EepData;
_EERPOM_t *EepAddr;

/*
 * EEPROM DATA
 * */
uint32_t total_eepdata_32[4];
uint16_t radial_eepdata_16[24];
uint16_t focus_eepdata_16[24];
uint16_t focus_setting_16[20]; /* Unused Value */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static void EEPROM_Error(void);
void EEPROM_DataInit(void);
uint8_t EEPROM_CountSave(uint8_t idx, uint32_t eep_data);
void BSP_EEPROM_TIMEOUT_UserCallback(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * EEPROM Data Initialization
 * */
void EEPROM_DataInit(void)
{
	uint8_t srt_flag[5] =
	{ "INIT" };
	char str_init[4] =
	{ 0, };
	uint32_t size;

	/* Parameter Memory Area Check */
	size = sizeof(_EERPOM_t);
	if (size >= 4000)
	{
		/* LCD ON */
		User_LCD_BackLight_On();
		BSP_LCD_DisplayStringAt(20, 150, (uint8_t*) "EEPROM Size Over", CENTER_MODE);
		while (1)
		{
		}
	}

	// 기존 데이터 저장 영역
	//     0 ~   15 : u32 total_eepdata_32[4]   // 카운터 데이터 저장 (사용하지 않음)
	//    16 ~   63 : u16 radial_eepdata_16[24] // ez_radial 셋팅값 저장
	//    64 ~  111 : u16 focus_eepdata_16[24]  // ez_focus 셋팅값 저장
	//   112 ~  113 : 사용안함.
	//   114 ~  144 : u8  radial_sensor_8[31]   // 라디알 bar별 센서값 저장.
	//   145 ~  289 : 사용안함.
	//   290 ~ 1217 : u16 radial_out_16[31]     // 라디알 bar별 PWM값 저장.
	//  1218 ~ 1219 : 사용안함.
	//  1220 ~ 1240 : u16 focus_out_soo_8[21]   // 포커스 intensity별 펄스 횟수값 저장.
	//  2192 ~ 2233 : u16 focus_out_16[21]      // 포커스 intensity별 펄스폭값 저장.
	//  ..
	//  ..
	//  8000 ~ 8003 : ez radial, focus 셋팅값 초기값 확인용.
	//  8004 ~ 8007 : 라디알 공장 셋팅값 초기값 확인용.
	//  8008 ~ 8011 : 카운터 데이터 초기값 확인용.
	//  8012 ~ 8015 : 포커스 intensity별 펄스폭값 초기값 확인용.
	//  8016 ~ 8019 : 포커스 intensity별 펄스 횟수값 초기값 확인용.

	// 옵션 저장 영역
	EepAddr = (_EERPOM_t*) (4000); // 7999까지 사용가능.

	BSP_EEPROM_ReadBuffer((uint8_t*) EepData.Flag, (uint32_t) EepAddr->Flag, sizeof(EepAddr->Flag));

	if (strcmp((char*) EepData.Flag, (char const*) srt_flag) != 0)
	{
		EepData.KeyVolume = 0;
		EepData.CompleteVolume = 0;
		EepData.BacklightBright = 10;

		BSP_EEPROM_ReadBuffer((uint8_t*) &EepData.FactoryModeEnterTime, (uint32_t) &EepAddr->FactoryModeEnterTime, sizeof(EepAddr->FactoryModeEnterTime));
		if (EepData.FactoryModeEnterTime != 10)
		{
			EepData.FactoryModeEnterTime = 0;
		}

		for (uint8_t i = 0; i < 10; i++)
		{
			EepData.FocusTotalCnt[i] = 0;
			EepData.FocusHandCnt[i] = 0;
			EepData.RadialTotalCnt[i] = 0;
			EepData.RadialHandCnt[i] = 0;
		}

		EepData.FocusCntIdx = 0;
		EepData.RadialCntIdx = 0;

		// 기존 flag "INIT" 저장되어 있는 영역.
		// 호환성위해 남김. (V2020.10.27이전버전)
		memcpy(EepData.Dummy, srt_flag, 5);

		BSP_EEPROM_ReadBuffer((uint8_t*) str_init, 8008, 4); // Read EEPROM

		// 8008~8011번지 값이 "CCCC"값과 맞지 않으면 0으로 초기화.
		if (strncmp((const char*) str_init, "CCCC", 4) == 0)
		{
			BSP_EEPROM_ReadBuffer((uint8_t*) &EepData.FocusTotalCnt[0], (4 * 0), 4);
			BSP_EEPROM_ReadBuffer((uint8_t*) &EepData.FocusHandCnt[0], (4 * 1), 4);
			BSP_EEPROM_ReadBuffer((uint8_t*) &EepData.RadialTotalCnt[0], (4 * 2), 4);
			BSP_EEPROM_ReadBuffer((uint8_t*) &EepData.RadialHandCnt[0], (4 * 3), 4);

			uint32_t temp = 0;
			BSP_EEPROM_WriteBuffer((uint8_t*) &temp, 8008, 4); // Write EEPROM
		}

		// EEPROM설정 확인용.
		memcpy(EepData.Flag, srt_flag, 5);

		// EEPROM초기화.
		BSP_EEPROM_WriteBuffer((uint8_t*) &EepData, (uint32_t) EepAddr, sizeof(_EERPOM_t));
		BSP_EEPROM_ReadBuffer((uint8_t*) &EepData, (uint32_t) EepAddr, sizeof(_EERPOM_t));
	}
	else
	{
		BSP_EEPROM_ReadBuffer((uint8_t*) &EepData, (uint32_t) EepAddr, sizeof(_EERPOM_t));
	}

	// 공장모드 변수. //21.06.25
	BSP_EEPROM_ReadBuffer((uint8_t*) EepData.Factory.Flag, (uint32_t) &EepAddr->Factory.Flag, sizeof(EepAddr->Factory.Flag));

	if (strcmp((char*) EepData.Factory.Flag, (char const*) "FACT") != 0)
	{
		EepData.Factory.RadialKeyMode = 0;

		// EEPROM설정 확인용.
		memcpy(EepData.Factory.Flag, "FACT", 5);

		BSP_EEPROM_WriteBuffer((uint8_t*) &EepData.Factory, (uint32_t) &EepAddr->Factory, sizeof(EepAddr->Factory));
		BSP_EEPROM_ReadBuffer((uint8_t*) &EepData.Factory, (uint32_t) &EepAddr->Factory, sizeof(EepAddr->Factory));
	}
	else
	{
		BSP_EEPROM_ReadBuffer((uint8_t*) &EepData.Factory, (uint32_t) &EepAddr->Factory, sizeof(EepAddr->Factory));
	}

	if (EepData.Factory.RadialKeyMode > 1)
	{
		EepData.Factory.RadialKeyMode = 0;
	}

#if 0
	focused_TotalCount = total_eepdata_32[0] = EepData.FocusTotalCnt[EepData.FocusCntIdx];
	focused_HandCount = total_eepdata_32[1] = EepData.FocusHandCnt[EepData.FocusCntIdx];
	radial_TotalCount = total_eepdata_32[2] = EepData.RadialTotalCnt[EepData.RadialCntIdx];
	radial_HandCount = total_eepdata_32[3] = EepData.RadialHandCnt[EepData.RadialCntIdx];
#endif
}

/*
 * EEPROM Data Initialization
 * */
uint8_t EEPROM_CountSave(uint8_t idx, uint32_t eep_data)
{
	uint32_t eep_addr[2];
	uint32_t *pRead_data[2];

	uint8_t err_cnt;

	switch (idx)
	{
	case 0:
		eep_addr[0] = (uint32_t) &EepAddr->FocusTotalCnt[EepData.FocusCntIdx];
		// eep_addr[1] = (u32)&EepAddr->FocusTotalCnt[1];
		pRead_data[0] = &EepData.FocusTotalCnt[EepData.FocusCntIdx];
		// pRead_data[1] = &EepData.FocusTotalCnt[1];
		break;
	case 1:
		eep_addr[0] = (uint32_t) &EepAddr->FocusHandCnt[EepData.FocusCntIdx];
		// eep_addr[1] = (u32)&EepAddr->FocusHandCnt[1];
		pRead_data[0] = &EepData.FocusHandCnt[EepData.FocusCntIdx];
		// pRead_data[1] = &EepData.FocusHandCnt[1];
		break;
	case 2:
		eep_addr[0] = (uint32_t) &EepAddr->RadialTotalCnt[EepData.RadialCntIdx];
		// eep_addr[1] = (u32)&EepAddr->RadialTotalCnt[1];
		pRead_data[0] = &EepData.RadialTotalCnt[EepData.RadialCntIdx];
		// pRead_data[1] = &EepData.RadialTotalCnt[1];
		break;
	case 3:
		eep_addr[0] = (uint32_t) &EepAddr->RadialHandCnt[EepData.RadialCntIdx];
		// eep_addr[1] = (u32)&EepAddr->RadialHandCnt[1];
		pRead_data[0] = &EepData.RadialHandCnt[EepData.RadialCntIdx];
		// pRead_data[1] = &EepData.RadialHandCnt[1];
		break;
	default:
		return 0;
	}

	for (err_cnt = 0; err_cnt < 10; err_cnt++)
	{
		BSP_EEPROM_WriteBuffer((uint8_t*) &eep_data, eep_addr[0], 4);
		BSP_EEPROM_ReadBuffer((uint8_t*) pRead_data[0], eep_addr[0], 4);
		if (eep_data == *pRead_data[0])
		{
			break;
		}
		else
		{
			EEPROM_Error();
			HAL_Delay(10);
		}
	}
	if (err_cnt == 10)
	{
		BUZZER_ON();
		HAL_Delay(1000);
		BUZZER_OFF();

		HAL_NVIC_SystemReset();
		return 0;
	}

	return 1;
}

/*
 * EEPROM ERROR
 * */
static void EEPROM_Error(void)
{
	BSP_EEPROM_DeInit();
	BSP_EEPROM_Init();
}

/*
 * EEPROM Timeout Callback
 * */
void BSP_EEPROM_TIMEOUT_UserCallback(void)
{
	EEPROM_Error();
}
/* USER CODE END 0 */

