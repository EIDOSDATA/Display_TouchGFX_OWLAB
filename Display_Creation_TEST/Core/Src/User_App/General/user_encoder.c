/* USER CODE BEGIN Header */
/*
 * user_encoder.h
 *
 *  Created on: Mar 25, 2025
 *      Author: user
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "user_encoder.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user_disp_lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ENCODER_DISPLAY_HANDLE_PERIOD		100
#define ENCODER_PARAM_READ_PERIOD			100
#define ENCODER_STATE_HANDLE_PERIOD			10
#define ENCODER_MAX_PARAM					5000

#define DEF_TIM1_PSC				5000
#define DEF_TIM1_PERIOD				6994
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;
encoder_sys_state_t cur_motor_dir;
encoder_sys_state_t prev_motor_dir;

static uint16_t encoder_cnt1, encoder_cnt2;
static uint16_t encoder_diff;

static uint32_t gSchdule_tick_enc_disp = 0;
static uint32_t gSchdule_tick_enc_state = 0;
static uint32_t gSchdule_tick_enc_param = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void User_Encoder_Start(void);
void User_Handle_Encoder_Operation(void);
void User_Update_Encoder_State(void);
void User_Encoder_Parameter_Read(void);
void User_Encoder_Parameter_Display(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Encoder Start */
void User_Encoder_Start(void)
{
	/* Encoder Start */
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_2);

	/* Encoder Parameter Initialization */
	encoder_cnt1 = TIM4->CNT;
	prev_motor_dir = SYS_STATE_INIT;
	cur_motor_dir = SYS_STATE_INIT;
}

/* Encoder Operation */
void User_Handle_Encoder_Operation(void)
{
	/* Encoder State Machine */
	User_Update_Encoder_State();

	/* Encoder Read */
	User_Encoder_Parameter_Read();

	/* Encoder Parameter Display */
	User_Encoder_Parameter_Display();
}

/* Update Encoder Status */
void User_Update_Encoder_State(void)
{
	if (HAL_GetTick() - gSchdule_tick_enc_state >= ENCODER_STATE_HANDLE_PERIOD)
	{
		/* CCW :: Encoder Value -- */
		if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4))
		{
			cur_motor_dir = SYS_STATE_CCW;
			/* Exception Handling */
			if ((encoder_cnt2 > encoder_cnt1) && (encoder_cnt2 - encoder_cnt1 < 100))
			{
				cur_motor_dir = SYS_STATE_ERROR;
			}
		}

		/* CW :: Encoder Value ++ */
		else
		{
			cur_motor_dir = SYS_STATE_CW;
			/* Exception Handling */
			if ((encoder_cnt1 > encoder_cnt2) && (encoder_cnt1 - encoder_cnt2 < 100))
			{
				cur_motor_dir = SYS_STATE_ERROR;
			}
		}

		/* State Update */
		if (cur_motor_dir != prev_motor_dir)
		{
			prev_motor_dir = cur_motor_dir;
		}
		gSchdule_tick_enc_state = HAL_GetTick();
	}
}

/* Encoder Parameter Read Function */
void User_Encoder_Parameter_Read(void)
{
	if (HAL_GetTick() - gSchdule_tick_enc_param >= ENCODER_PARAM_READ_PERIOD)
	{
		/* Encoder Value :: Under Flow */
		if (prev_motor_dir == SYS_STATE_CCW && TIM4->CNT > 60000)
		{
			TIM4->CNT = 0;
			encoder_cnt1 = 0;
			encoder_cnt2 = 0;
		}
		else if (prev_motor_dir == SYS_STATE_CW && TIM4->CNT > 60000)
		{
			TIM4->CNT = 0;
			encoder_cnt1 = 0;
			encoder_cnt2 = 0;
		}

		/* Encoder Value :: Over Flow */
		if (prev_motor_dir == SYS_STATE_CW && TIM4->CNT > ENCODER_MAX_PARAM)
		{
			TIM4->CNT = ENCODER_MAX_PARAM - 1;
			encoder_cnt1 = ENCODER_MAX_PARAM - 1;
			encoder_cnt2 = ENCODER_MAX_PARAM - 1;
		}
		else if (prev_motor_dir == SYS_STATE_CCW && TIM4->CNT > ENCODER_MAX_PARAM)
		{
			TIM4->CNT = ENCODER_MAX_PARAM - 1;
			encoder_cnt1 = ENCODER_MAX_PARAM - 1;
			encoder_cnt2 = ENCODER_MAX_PARAM - 1;
		}
		else
		{
			encoder_cnt2 = TIM4->CNT;
		}

		/* MOTOR DIRECTION */
		switch (cur_motor_dir)
		{
		case SYS_STATE_CW:
			/* Up Counting :: CW */
			if (encoder_cnt2 >= encoder_cnt1)
			{
				encoder_diff = encoder_cnt2 - encoder_cnt1;
			}
			else
			{
				encoder_diff = (TIM4->ARR + encoder_cnt2) - encoder_cnt1;
			}
			break;
		case SYS_STATE_CCW:
			/* Down Counting :: CCW */
			if (encoder_cnt1 >= encoder_cnt2)
			{
				encoder_diff = encoder_cnt1 - encoder_cnt2;
			}
			else
			{
				encoder_diff = (TIM4->ARR + encoder_cnt1) - encoder_cnt2;
			}
			break;
		default:
			break;
		}
		if (encoder_diff != 0)
		{
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
			HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);

			__HAL_TIM_SET_PRESCALER(&htim1, DEF_TIM1_PSC - 1);
			__HAL_TIM_SET_AUTORELOAD(&htim1, (DEF_TIM1_PERIOD + (encoder_cnt2 * 10)) - 1);
			//TIM1->CNT = 0;
			TIM1->CCR3 = ((DEF_TIM1_PERIOD + (encoder_cnt2 * 10)) / 2) - 1;

			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
		}

		encoder_cnt1 = TIM4->CNT;
		gSchdule_tick_enc_param = HAL_GetTick();
	}
}

/* Encoder Parameter Display Function */
void User_Encoder_Parameter_Display(void)
{
	char print_str[100];
	if (HAL_GetTick() - gSchdule_tick_enc_disp >= ENCODER_DISPLAY_HANDLE_PERIOD)
	{
		sprintf(print_str, "Encoder Direction : %5d", cur_motor_dir);
		BSP_LCD_DisplayStringAt(20, 90, (uint8_t*) print_str, CENTER_MODE);
		sprintf(print_str, "Encoder CNT1 = %5d", encoder_cnt1);
		BSP_LCD_DisplayStringAt(20, 110, (uint8_t*) print_str, CENTER_MODE);
		sprintf(print_str, "Encoder CNT2 = %5d", encoder_cnt2);
		BSP_LCD_DisplayStringAt(20, 130, (uint8_t*) print_str, CENTER_MODE);
		sprintf(print_str, "Encoder Chennel Diff = %5d", encoder_diff);
		BSP_LCD_DisplayStringAt(20, 150, (uint8_t*) print_str, CENTER_MODE);
		gSchdule_tick_enc_disp = HAL_GetTick();
	}
}
/* USER CODE END 0 */

