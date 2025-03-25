/* USER CODE BEGIN Header */
/*
 * user_disp_touch_key.c
 *
 *  Created on: Mar 10, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_disp_touch_key.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user_disp_touch.h"
#include "user_disp_lcd.h"
#include "user_mem_qspi.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* Button Area Definition */
Touch_BTN_Region_t touch_btns[] =
{/* x pos min, x pos max, y pos min, y pos max, key value, Status */
{ 90, 320, 200, 260, 1, TOUCH_BTN_STATE_RELEASED }, /* RET Button */
{ 240, 460, 240, 340, 2, TOUCH_BTN_STATE_RELEASED }, /* CET Button */
{ 90, 470, 110, 180, 3, TOUCH_BTN_STATE_RELEASED }, /* Time Button */
{ 230, 480, 490, 550, 4, TOUCH_BTN_STATE_RELEASED } /* START/STOP Button */
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TOUCH_DEBOUNCE_DELAY_MS			50
#define START_BTN_DEBOUNCE_TIME_MS		200

#define BTN_PRESS_LONG_TIME				1000
#define BTN_PRESS_SHORT_TIME			100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t g_key_start_toggle_f = 0;
static uint32_t gSchdule_tick_touch_debounce = 0;
static uint32_t gSchdule_tick_btn_press_time = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void User_Main_Touch_Key(void);
void User_Update_Button_States(TS_StateTypeDef *touch_state);
touch_btn_state_t User_GetButton_State(uint8_t button_id);

void User_ProcessButton_Actions(void);

void User_TouchScreen_Position_Print(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void User_Main_Touch_Key(void)
{
	/* Touch Check */
	if (User_TS_GetState(&Touch_State) == 0)
	{
		return;
	}

	/* DEBOUNCE TIME CHECK */
	if (HAL_GetTick() - gSchdule_tick_touch_debounce < TOUCH_DEBOUNCE_DELAY_MS)
	{
		return;
	}
	gSchdule_tick_touch_debounce = HAL_GetTick();

#if 0
	User_TouchScreen_Position_Print();
#endif

	/* Check button presses */
	Touch_State.timestamp = HAL_GetTick();
	User_Update_Button_States(&Touch_State);
}

/* Touch Button Status Update */
void User_Update_Button_States(TS_StateTypeDef *touch_state)
{
	for (int i = 0; i < USER_TOUCH_BTN_MAX; ++i)
	{
		if (touch_state->x_pos > touch_btns[i].x_min && touch_state->x_pos < touch_btns[i].x_max && touch_state->y_pos > touch_btns[i].y_min && touch_state->y_pos < touch_btns[i].y_max)
		{
			/*
			 * TODO : STATUS CHECK PLZ
			 * */
			touch_btns[i].prev_state = touch_btns[i].cur_state;
			touch_btns[i].cur_state = (touch_state->is_pressed) ? TOUCH_BTN_STATE_PRESSED : TOUCH_BTN_STATE_RELEASED;
		}
		else
		{
			touch_btns[i].cur_state = TOUCH_BTN_STATE_RELEASED;
		}
	}
}

/* Touch Button Status Check */
touch_btn_state_t User_GetButton_State(uint8_t button_id)
{
	for (int i = 0; i < sizeof(touch_btns) / sizeof(touch_btns[0]); ++i)
	{
		if (touch_btns[i].button_id == button_id)
		{
			return touch_btns[i].cur_state;
		}
	}
	return TOUCH_BTN_STATE_RELEASED; // 버튼 ID가 없을 경우 기본값 반환
}

/* Button Process */
void User_ProcessButton_Actions(void)
{
	if (User_TS_GetState(&Touch_State) == 0)
	{
		return;
	}

	char print_str[100];

	for (int i = 0; i < USER_TOUCH_BTN_MAX; i++)
	{
		touch_btn_state_t current_state = User_GetButton_State(touch_btns[i].button_id);

		switch (current_state)
		{
		case TOUCH_BTN_STATE_INIT:
			break;
		case TOUCH_BTN_STATE_PRESSED:
			switch (touch_btns[i].button_id)
			{
			case 1: /* Button ID 1 (RET) */
				BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[3]);
				break;

			case 2: /* Button ID 2 (CET) */
				BSP_LCD_DrawBitmap(0, 0, (uint8_t*) pQSPI->LCD_IMAGE_BUFFER[2]);
				break;

			case 3: /* Button ID 3 (TIME) */
				sprintf(print_str, "Button 3 (TIME) pressed.");
				BSP_LCD_DisplayStringAt(500, 0, (uint8_t*) print_str, LEFT_MODE);
				break;

			case 4: /* Button ID 4 (START/STOP) */
#if 1
				/* BAN TIME : 1000ms */
				if (HAL_GetTick() - gSchdule_tick_btn_press_time <= BTN_PRESS_LONG_TIME)
				{
					gSchdule_tick_btn_press_time = HAL_GetTick();
					return;
				}
				else
				{
					touch_btns[i].prev_state = TOUCH_BTN_STATE_PRESSED;
					if (g_key_start_toggle_f == 0)
					{
						sprintf(print_str, "START action on PRESSED.");
						BSP_LCD_DisplayStringAt(500, 120, (uint8_t*) print_str, LEFT_MODE);
					}
					else if (g_key_start_toggle_f == 1)
					{
						sprintf(print_str, "STOP action on PRESSED.");
						BSP_LCD_DisplayStringAt(500, 120, (uint8_t*) print_str, LEFT_MODE);
					}
					/* Toggle Button Status */
					g_key_start_toggle_f = !g_key_start_toggle_f;
					gSchdule_tick_btn_press_time = HAL_GetTick();
				}
#endif
				break;
			default:
				break;
			}
			break;

		case TOUCH_BTN_STATE_RELEASED:
			switch (touch_btns[i].button_id)
			{
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4: /* Button ID 4 (START/STOP) */
				break;
			default:
				sprintf(print_str, "No action defined for RELEASED.");
				BSP_LCD_DisplayStringAt(500, 200, (uint8_t*) print_str, LEFT_MODE);
				break;
			}
			break;

		default:
			printf("Button %d state unknown.\n", touch_btns[i].button_id);
			break;
		} /* END OF SWTICH CASE */

	} /* END OF FOR */
}

void User_TouchScreen_Position_Print(void)
{
	char print_str[30];
	sprintf(print_str, "X=%4d", Touch_State.x_pos);
	BSP_LCD_DisplayStringAt(500, 0, (uint8_t*) print_str, LEFT_MODE);
	sprintf(print_str, "Y=%4d", Touch_State.y_pos);
	BSP_LCD_DisplayStringAt(500, 20, (uint8_t*) print_str, LEFT_MODE);
	sprintf(print_str, "Key=%4d", 22);
	BSP_LCD_DisplayStringAt(500, 40, (uint8_t*) print_str, LEFT_MODE);
}

/* USER CODE END 0 */
