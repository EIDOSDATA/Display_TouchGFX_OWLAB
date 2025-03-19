/*
 * user_buzzer.h
 *
 *  Created on: Mar 6, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_GENERAL_USER_BUZZER_H_
#define INC_USER_APP_GENERAL_USER_BUZZER_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct
{
	uint16_t Timer; // 각 음의 재생시간
	uint16_t Melody;
	uint16_t Tone; // 음계
	uint16_t Interval;
	uint16_t *Addr;
	uint16_t Envelope;
	__IO uint8_t Volume;
	__IO uint8_t KeyVolume;
	__IO uint8_t CompleteVolume;
	uint16_t Duty;
	uint16_t Size;
	uint8_t State;
	uint8_t Octave;
} _BUZZER_VALUE_t;

extern _BUZZER_VALUE_t Buz;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
extern void BUZZER_Init(void);
extern void BUZZER_TimInterrupt(void);

extern void Buzzer(uint8_t Octave, uint8_t Command);
extern void SetBuzzerVolume(uint8_t volume);
extern void Beep(uint8_t volume);
extern void CompleteBeep(uint8_t volume);

extern void BUZZER_ON(void);
extern void BUZZER_OFF(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/*
 * BUZZER TONE VALUE
 * */
#define tDO (30578) // 32.7032Hz (1 Octave : C(도))
#define tDOX (28862)
#define tLE (27242)
#define tLEX (25713)
#define tMI (24270)
#define tPA (22908)
#define tPAX (21622)
#define tSOL (20408)
#define tSOLX (19263)
#define tLA (18182)
#define tLAX (17161)
#define tSI (16198)  // 61.7354Hz (1 Octave : B(시))
#define tDOO (15289) // 65.4064Hz (2 Octave : C(도)

/*
 * BUZZER KEY FLAG
 * */
#define BZ_SWITCH 0
#define BZ_START 1
#define BZ_COMPLETE 2
#define BZ_TEST 3
#define BZ_UP 4
#define BZ_DOWN 5

/*
 * BUZZER TONE CONTROL
 * */
#define BUZZER_TONE TIM3->ARR
#define BUZZER_DUTY TIM3->CCR1

// #define Beep()          Buzzer(SWITCH)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_GENERAL_USER_BUZZER_H_ */
