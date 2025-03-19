/*
 * user_disp_touch.h
 *
 *  Created on: Mar 5, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_DISPLAY_USER_DISP_TOUCH_H_
#define INC_USER_APP_DISPLAY_USER_DISP_TOUCH_H_

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
/*
 * Touch Screen State Definition
 * */
typedef struct
{
	uint16_t TouchDetected;
	uint16_t x_pos;
	uint16_t y_pos;
	uint16_t z_pos;
} TS_StateTypeDef;

/*
 * Button Position
 * */
typedef struct
{
	uint16_t bnt_x1_pos;
	uint16_t btn_y1_pos;
	uint16_t btn_x2_pos;
	uint16_t btn_y2_pos;
} _BTN_POS_t;

/*
 * LCD Parameter
 * */
#define BTN_MAX 30
typedef struct
{
	uint8_t Status;					// LCD 상태
#if 0
	uint16_t TouchX;		/* 터치 X좌표 */
	uint16_t TouchY;		/* 터치 Y좌표 */
#endif
	uint8_t TouchBtn;				// 터치 버튼
	_BTN_POS_t Btn_Pos[BTN_MAX];	// 버튼 좌표
	uint16_t BtnNum;				// 버튼 갯수
} _LCD_t;

extern TS_StateTypeDef Touch_State;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
extern void User_Touch_Init(void);
extern uint8_t User_TS_GetState(TS_StateTypeDef *TS_State);
extern uint8_t User_Touch_Btn_Check(uint8_t menu);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define MENU_SETTING 0
#define COUNTER_SETTING 1
#define FACTORY_SETTING 2
#define DEBUGGING 3
// #define BTN_MAX 30

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_DISPLAY_USER_DISP_TOUCH_H_ */
