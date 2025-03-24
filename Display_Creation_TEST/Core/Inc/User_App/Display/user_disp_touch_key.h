/*
 * user_disp_touch_key.h
 *
 *  Created on: Mar 9, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_DISPLAY_USER_DISP_TOUCH_KEY_H_
#define INC_USER_APP_DISPLAY_USER_DISP_TOUCH_KEY_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "user_disp_touch.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32h743i_eval_lcd.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum
{
	TOUCH_BTN_STATE_INIT = 0x00, /* Initialization */
	TOUCH_BTN_STATE_PRESSED, /* Pressed */
	TOUCH_BTN_STATE_RELEASED, /* UnPressed */
	TOUCH_BTN_STATE_ERROR /* ERROR */
} touch_btn_state_t;

/* Button Area Struct Definition */
typedef struct
{
	uint16_t x_min;       // 버튼 영역의 최소 X 좌표
	uint16_t x_max;       // 버튼 영역의 최대 X 좌표
	uint16_t y_min;       // 버튼 영역의 최소 Y 좌표
	uint16_t y_max;       // 버튼 영역의 최대 Y 좌표
	uint8_t button_id;    // 버튼 ID 또는 값
	touch_btn_state_t cur_state; // 현재 버튼 상태
	touch_btn_state_t prev_state; // 현재 버튼 상태
} Touch_BTN_Region_t;
extern Touch_BTN_Region_t touch_btns[];

extern uint8_t g_key_start_toggle_f;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
/* Touch Main Key Function --------------------*/
extern void User_Main_Touch_Key(void);
extern void User_Update_Button_States(TS_StateTypeDef *touch_state);
extern touch_btn_state_t User_GetButton_State(uint8_t button_id);

extern void User_ProcessButton_Actions(void);

extern void User_TouchScreen_Position_Print(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define USER_TOUCH_BTN_MAX	4
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_DISPLAY_USER_DISP_TOUCH_KEY_H_ */

