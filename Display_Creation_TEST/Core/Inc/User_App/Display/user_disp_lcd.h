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
extern void User_TFTLCD_Init(void);

/* Radial Display Function --------------------*/
/* Radial */
extern void radial_key_display(void);
extern void radial_sensor_key_display(void);
extern void radial_image_delete(void);
extern void radial_select_draw(uint8_t no, uint32_t color);

/* Focus Display Function --------------------*/
/* Focus */
extern void focus_key_display(void);
extern void focus_image_delete(void);
extern void focus_select_draw(uint8_t no, uint32_t color);
extern void focus_seting_display(void);
extern void focus_seting_single_display(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define LCD_FRAME_BUFFER         0xD0000000
#define JPEG_OUTPUT_DATA_BUFFER  0xD0500000

//#define LCD_FRAME_BUFFER_LAYER0                  0xD0130000 //640x480x4
#define LCD_FRAME_BUFFER_LAYER0                  (LCD_FRAME_BUFFER + 0x200000)
#define LCD_FRAME_BUFFER_LAYER1                  (LCD_FRAME_BUFFER)

#define LCD_ON() HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1); /* Display ON */
#define LCD_OFF() HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1); /* Display OFF */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_APP_DISPLAY_USER_DISPT_H_ */

