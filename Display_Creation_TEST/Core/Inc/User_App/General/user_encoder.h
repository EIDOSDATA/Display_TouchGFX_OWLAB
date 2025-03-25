/*
 * user_encoder.h
 *
 *  Created on: Mar 25, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_GENERAL_USER_ENCODER_H_
#define INC_USER_APP_GENERAL_USER_ENCODER_H_

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
typedef enum
{
	SYS_STATE_INIT = 0x00, /* Initialization */
	SYS_STATE_CCW, /* CCW */
	SYS_STATE_CW, /* CW */
	SYS_STATE_ERROR /* ERROR */
} encoder_sys_state_t;
extern encoder_sys_state_t cur_motor_dir;
extern encoder_sys_state_t prev_motor_dir;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
extern void User_Encoder_Start(void);
extern void User_Handle_Encoder_Operation(void);
extern void User_Update_Encoder_State(void);
extern void User_Encoder_Parameter_Read(void);
extern void User_Encoder_Parameter_Display(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_GENERAL_USER_ENCODER_H_ */

