/* USER CODE BEGIN Header */
/*
 * user_disp_touch.c
 *
 *  Created on: Mar 5, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_disp_touch.h"
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
TS_StateTypeDef Touch_State;
_BTN_LCD_PRINT_t Lcds;

extern I2C_HandleTypeDef hi2c1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void User_Touch_Init(void);
uint8_t User_TS_DetectTouch(void);
uint8_t User_TS_GetState(TS_StateTypeDef *TS_State);

static void User_TS_Error(void);
static void User_Touch_Btn_Position(uint8_t id, uint16_t xx, uint16_t yy, uint16_t x_size, uint16_t y_size);
static void User_Touch_Btn_Init(uint8_t menu);
uint8_t User_Touch_Btn_Check(uint8_t menu);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
 * Touch Initialization
 * */
void User_Touch_Init(void)
{
#if 0
	HAL_StatusTypeDef status;
#endif
	uint8_t tmp_buffer[2];
	tmp_buffer[0] = 0xA4;
	tmp_buffer[1] = 0x00;
	/*
	 * NOTE :
	 * 	If GPIO toggled.. Touch Reset
	 * */
	HAL_GPIO_WritePin(TOUCH_RESET_GPIO_Port, TOUCH_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(TOUCH_RESET_GPIO_Port, TOUCH_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(TOUCH_RESET_GPIO_Port, TOUCH_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(100);

	if (HAL_I2C_Master_Transmit(&hi2c1, 0x70, tmp_buffer, 2, 1000) != HAL_OK)
	{
		User_TS_Error();
	}
#if 0
	tmp_buffer[0] = 0;
	status = HAL_I2C_Master_Transmit(&hi2c1, 0x70, tmp_buffer, 1, 1000);
#endif
}

/*
 * Touch Detection
 * */
uint8_t User_TS_DetectTouch(void)
{
	uint8_t tmp_buffer[2];

	tmp_buffer[0] = 2;
	if (HAL_I2C_Master_Transmit(&hi2c1, 0x70, tmp_buffer, 1, 10) != HAL_OK)
	{
		User_TS_Error();
		return 0;
	}

	memset(tmp_buffer, 0, sizeof(tmp_buffer));
	if (HAL_I2C_Master_Receive(&hi2c1, 0x71, tmp_buffer, 1, 10) != HAL_OK)
	{
		User_TS_Error();
		return 0;
	}

	/* check for first byte */
	if (tmp_buffer[0] == 0xFF || tmp_buffer[0] == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*
 * Touch Screen :: Get Status
 * */
uint8_t User_TS_GetState(TS_StateTypeDef *TS_State)
{
	uint8_t tmp_buffer[2];
	uint8_t a_buffer[8];

	if (User_TS_DetectTouch() == 1)
	{
		tmp_buffer[0] = 0;
		HAL_I2C_Master_Transmit(&hi2c1, 0x70, tmp_buffer, 1, 1000);

		memset(a_buffer, 0, sizeof(a_buffer));
		HAL_I2C_Master_Receive(&hi2c1, 0x71, a_buffer, sizeof(a_buffer), 1000);

		/* Calculate positions (handling reversed LCD logic) */
		uint16_t x_raw = ((a_buffer[3] & 0xF) << 8) | a_buffer[4];
		uint16_t y_raw = ((a_buffer[5] & 0xF) << 8) | a_buffer[6];

		if (LCD_POSITION_REVERSED)
		{
			/* The LCD position has been inverted. */
			TS_State->x_pos = LCD_MAXIMUM_X_SIZE - (x_raw % LCD_MAXIMUM_X_SIZE);
			TS_State->y_pos = LCD_MAXIMUM_Y_SIZE - (y_raw % LCD_MAXIMUM_Y_SIZE);
		}
		else
		{
			/* LCD position is not inverted. */
			TS_State->x_pos = LCD_MAXIMUM_X_SIZE - (x_raw % LCD_MAXIMUM_X_SIZE);
			TS_State->y_pos = y_raw % LCD_MAXIMUM_Y_SIZE;
		}

		/* Set touch detected */
		TS_State->TouchDetected = 1;
		return 1;
	}
	else
	{
		/* No touch detected */
		TS_State->TouchDetected = 0;
		return 0;
	}
}

/*
 * Touch Screen Error Handler??
 * */
static void User_TS_Error(void)
{
	HAL_I2C_DeInit(&hi2c1);

	MX_I2C1_Init();

	// TOUCH_Init();
}

/*
 * Touch Screen Button Position Clac
 * */
static void User_Touch_Btn_Position(uint8_t id, uint16_t xx, uint16_t yy, uint16_t x_size, uint16_t y_size)
{
	Lcds.Btn_Pos[id].bnt_x1_pos = xx;
	Lcds.Btn_Pos[id].btn_y1_pos = yy;
	Lcds.Btn_Pos[id].btn_x2_pos = xx + x_size;
	Lcds.Btn_Pos[id].btn_y2_pos = yy + y_size;
}

/*
 * Touch Button Initialization
 * */
static void User_Touch_Btn_Init(uint8_t menu)
{
	// 이전 좌표 정보 초기화.
	for (uint8_t i = 0; i < BTN_MAX; i++)
	{
		User_Touch_Btn_Position(i, 0, 0, 0, 0);
	}

	switch (menu)
	{
	case MENU_SETTING:
		Lcds.BtnNum = 5;

		// Setting 버튼 위치 설정
		User_Touch_Btn_Position(0, 223, 290, 114, 42); // 키 부저 소리 변경 버튼.
		User_Touch_Btn_Position(1, 454, 290, 114, 42); // 종료 부저 소리 변경 버튼.
		User_Touch_Btn_Position(2, 742, 132, 49, 42);  // 백라이트 밝기 증가 버튼.
		User_Touch_Btn_Position(3, 742, 289, 49, 42);  // 백라이트 밝기 감소 버튼.

		User_Touch_Btn_Position(4, 382, 369, 258, 62); // OK버튼.
		break;

	case COUNTER_SETTING:
		Lcds.BtnNum = 13;

		// 숫자패드.
		User_Touch_Btn_Position(0, 625 + (0 * 100), 130 + (0 * 100), 80, 80);  // 1
		User_Touch_Btn_Position(1, 625 + (1 * 100), 130 + (0 * 100), 80, 80);  // 2
		User_Touch_Btn_Position(2, 625 + (2 * 100), 130 + (0 * 100), 80, 80);  // 3
		User_Touch_Btn_Position(3, 625 + (0 * 100), 130 + (1 * 100), 80, 80);  // 4
		User_Touch_Btn_Position(4, 625 + (1 * 100), 130 + (1 * 100), 80, 80);  // 5
		User_Touch_Btn_Position(5, 625 + (2 * 100), 130 + (1 * 100), 80, 80);  // 6
		User_Touch_Btn_Position(6, 625 + (0 * 100), 130 + (2 * 100), 80, 80);  // 7
		User_Touch_Btn_Position(7, 625 + (1 * 100), 130 + (2 * 100), 80, 80);  // 8
		User_Touch_Btn_Position(8, 625 + (2 * 100), 130 + (2 * 100), 80, 80);  // 9
		User_Touch_Btn_Position(9, 625 + (0 * 100), 130 + (3 * 100), 80, 80);  // backspace
		User_Touch_Btn_Position(10, 625 + (1 * 100), 130 + (3 * 100), 80, 80); // 0
		User_Touch_Btn_Position(11, 625 + (2 * 100), 130 + (3 * 100), 80, 80); // enter

		User_Touch_Btn_Position(12, 863, 559, 161, 41); // young-in logo // factory 진입 용도로 사용.

		break;

	case FACTORY_SETTING:
		Lcds.BtnNum = 17;

		// 숫자패드.
		User_Touch_Btn_Position(0, 625 + (0 * 100), 130 + (0 * 100), 80, 80);  // 1
		User_Touch_Btn_Position(1, 625 + (1 * 100), 130 + (0 * 100), 80, 80);  // 2
		User_Touch_Btn_Position(2, 625 + (2 * 100), 130 + (0 * 100), 80, 80);  // 3
		User_Touch_Btn_Position(3, 625 + (0 * 100), 130 + (1 * 100), 80, 80);  // 4
		User_Touch_Btn_Position(4, 625 + (1 * 100), 130 + (1 * 100), 80, 80);  // 5
		User_Touch_Btn_Position(5, 625 + (2 * 100), 130 + (1 * 100), 80, 80);  // 6
		User_Touch_Btn_Position(6, 625 + (0 * 100), 130 + (2 * 100), 80, 80);  // 7
		User_Touch_Btn_Position(7, 625 + (1 * 100), 130 + (2 * 100), 80, 80);  // 8
		User_Touch_Btn_Position(8, 625 + (2 * 100), 130 + (2 * 100), 80, 80);  // 9
		User_Touch_Btn_Position(9, 625 + (0 * 100), 130 + (3 * 100), 80, 80);  // backspace
		User_Touch_Btn_Position(10, 625 + (1 * 100), 130 + (3 * 100), 80, 80); // 0
		User_Touch_Btn_Position(11, 625 + (2 * 100), 130 + (3 * 100), 80, 80); // enter

		// 카운터값 위치
		User_Touch_Btn_Position(12, 10, 300 + (0 * 48), 550, 40); // focus total cnt
		User_Touch_Btn_Position(13, 10, 300 + (1 * 48), 550, 40); // focus hand cnt
		User_Touch_Btn_Position(14, 10, 300 + (2 * 48), 550, 40); // radial total cnt
		User_Touch_Btn_Position(15, 10, 300 + (3 * 48), 550, 40); // radial hand cnt
		User_Touch_Btn_Position(16, 10, 300 + (4 * 48), 550, 40); // Clear all counters.
		break;

	case DEBUGGING:
		Lcds.BtnNum = 2;

		User_Touch_Btn_Position(0, 950, 50, 49, 42);  // 증가 버튼.
		User_Touch_Btn_Position(1, 950, 100, 49, 42); // 감소 버튼.
		break;
	}
}

/*
 * Touch Button Position Check
 * */
uint8_t User_Touch_Btn_Check(uint8_t menu)
{
	static uint8_t old_menu = 0xFF;
	uint8_t btn = 0, btn_no = 0xFF;

	// 메뉴가 변경되었으면 터치버튼 좌표값 재설정.
	if (old_menu != menu)
	{
		User_Touch_Btn_Init(menu);
		old_menu = menu;
	}

	for (btn = 0; btn < Lcds.BtnNum; btn++)
	{
		if ((Touch_State.x_pos > Lcds.Btn_Pos[btn].bnt_x1_pos) && (Touch_State.y_pos > Lcds.Btn_Pos[btn].btn_y1_pos) && (Touch_State.x_pos < Lcds.Btn_Pos[btn].btn_x2_pos) && (Touch_State.y_pos < Lcds.Btn_Pos[btn].btn_y2_pos))
		{
			btn_no = 100 + btn;
			break;
		}
	}

	return btn_no;
}

/* USER CODE END 0 */

