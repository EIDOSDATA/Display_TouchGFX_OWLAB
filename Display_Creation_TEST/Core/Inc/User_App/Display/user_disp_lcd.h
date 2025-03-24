/*
 * user_disp.h
 *
 *  Created on: Mar 6, 2025
 *      Author: user
 */

#ifndef INC_USER_APP_DISPLAY_USER_DISPT_H_
#define INC_USER_APP_DISPLAY_USER_DISPT_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32h743i_eval_lcd.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
extern void User_LCD_Init(void);
extern void User_LCD_BackLight_On(void);
extern void User_LCD_BackLight_Off(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define LCD_MAXIMUM_X_SIZE			1024
#define LCD_MAXIMUM_Y_SIZE			600

/* SDRAM MEMORY ADDRESS */
#define LCD_FRAME_BUFFER			0xD0000000
#define JPEG_OUTPUT_DATA_BUFFER		0xD0500000

/* SDRAM MEMORY ADDRESS */
//#define LCD_FRAME_BUFFER_LAYER0					0xD0258000 // LCD_MAXIMUM_X_SIZE * LCD_MAXIMUM_Y_SIZE * 2
#define LCD_FRAME_BUFFER_LAYER0					(LCD_FRAME_BUFFER + 0x200000)
#define LCD_FRAME_BUFFER_LAYER1					(LCD_FRAME_BUFFER)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_DISPLAY_USER_DISPT_H_ */

