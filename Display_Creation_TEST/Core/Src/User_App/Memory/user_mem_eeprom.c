/* USER CODE BEGIN Header */
/*
 * user_mem_eeprom.c
 *
 *  Created on: Mar 25, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_mem_eeprom.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "user_disp_lcd.h"

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
void User_EEPROM_Init(void);
void User_EEPROM_DataInit(void);

void BSP_EEPROM_TIMEOUT_UserCallback(void);
void User_EEPROM_Error(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * EEPROM Initialization
 * */
void User_EEPROM_Init(void)
{
	/* I2C2 EEPROM Initialization */
	BSP_EEPROM_Init(); /* I2C2 */
	User_EEPROM_DataInit();
}

/*
 * EEPROM Data Initialization
 * */
void User_EEPROM_DataInit(void)
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

	/*
	 * 기존 데이터 저장 영역
	 * -----------------------------
	 * 0 ~   15 : u32 total_eepdata_32[4]   // 카운터 데이터 저장 (사용하지 않음)
	 * 16 ~   63 : u16 radial_eepdata_16[24] // ez_radial 셋팅값 저장
	 * 64 ~  111 : u16 focus_eepdata_16[24]  // ez_focus 셋팅값 저장
	 * 112 ~  113 : 사용안함.
	 * 114 ~  144 : u8  radial_sensor_8[31]   // 라디알 bar별 센서값 저장.
	 * 145 ~  289 : 사용안함.
	 * 290 ~ 1217 : u16 radial_out_16[31]     // 라디알 bar별 PWM값 저장.
	 * 1218 ~ 1219 : 사용안함.
	 * 1220 ~ 1240 : u16 focus_out_soo_8[21]   // 포커스 intensity별 펄스 횟수값 저장.
	 * 2192 ~ 2233 : u16 focus_out_16[21]      // 포커스 intensity별 펄스폭값 저장.
	 * ..
	 * ..
	 * 8000 ~ 8003 : ez radial, focus 셋팅값 초기값 확인용.
	 * 8004 ~ 8007 : 라디알 공장 셋팅값 초기값 확인용.
	 * 8008 ~ 8011 : 카운터 데이터 초기값 확인용.
	 * 8012 ~ 8015 : 포커스 intensity별 펄스폭값 초기값 확인용.
	 * 8016 ~ 8019 : 포커스 intensity별 펄스 횟수값 초기값 확인용.
	 */

	/* 옵션 저장 영역
	 * 7999까지 사용가능.
	 * */
	EepAddr = (_EERPOM_t*) (4000);

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

		/* 기존 flag "INIT" 저장되어 있는 영역. */
		/* 호환성위해 남김. (V2020.10.27이전버전) */
		memcpy(EepData.Dummy, srt_flag, 5);

		BSP_EEPROM_ReadBuffer((uint8_t*) str_init, 8008, 4); // Read EEPROM

		/* 8008~8011번지 값이 "CCCC"값과 맞지 않으면 0으로 초기화. */
		if (strncmp((const char*) str_init, "CCCC", 4) == 0)
		{
			BSP_EEPROM_ReadBuffer((uint8_t*) &EepData.FocusTotalCnt[0], (4 * 0), 4);
			BSP_EEPROM_ReadBuffer((uint8_t*) &EepData.FocusHandCnt[0], (4 * 1), 4);
			BSP_EEPROM_ReadBuffer((uint8_t*) &EepData.RadialTotalCnt[0], (4 * 2), 4);
			BSP_EEPROM_ReadBuffer((uint8_t*) &EepData.RadialHandCnt[0], (4 * 3), 4);

			uint32_t temp = 0;
			BSP_EEPROM_WriteBuffer((uint8_t*) &temp, 8008, 4); // Write EEPROM
		}

		/* EEPROM설정 확인용. */
		memcpy(EepData.Flag, srt_flag, 5);

		/* EEPROM초기화 */
		BSP_EEPROM_WriteBuffer((uint8_t*) &EepData, (uint32_t) EepAddr, sizeof(_EERPOM_t));
		BSP_EEPROM_ReadBuffer((uint8_t*) &EepData, (uint32_t) EepAddr, sizeof(_EERPOM_t));
	}
	else
	{
		BSP_EEPROM_ReadBuffer((uint8_t*) &EepData, (uint32_t) EepAddr, sizeof(_EERPOM_t));
	}

	/* 공장모드 변수. Date : 21.06.25 */
	BSP_EEPROM_ReadBuffer((uint8_t*) EepData.Factory.Flag, (uint32_t) &EepAddr->Factory.Flag, sizeof(EepAddr->Factory.Flag));

	if (strcmp((char*) EepData.Factory.Flag, (char const*) "FACT") != 0)
	{
		EepData.Factory.RadialKeyMode = 0;

		/* EEPROM설정 확인용. */
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
 * EEPROM Timeout Callback
 * */
void BSP_EEPROM_TIMEOUT_UserCallback(void)
{
	User_EEPROM_Error();
}

/*
 * EEPROM ERROR
 * */
void User_EEPROM_Error(void)
{
	BSP_EEPROM_DeInit();
	BSP_EEPROM_Init();
}

/* USER CODE END 0 */

