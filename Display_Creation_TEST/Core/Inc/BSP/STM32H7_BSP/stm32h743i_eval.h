/**
 ******************************************************************************
 * @file    stm32h743i_eval.h
 * @author  MCD Application Team
 * @version V1.2.0
 * @date    29-December-2017
 * @brief   This file contains definitions for STM32H743I_EVAL LEDs,
 *          push-buttons and COM ports hardware resources.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32H743I_EVAL_H
#define __STM32H743I_EVAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/**
 * @brief  Define for STM32H743I_EVAL board
 */
#if !defined (USE_STM32H743I_EVAL)
#define USE_STM32H743I_EVAL
#endif

/* Exported constant IO ------------------------------------------------------*/

/*  The MFX_I2C_ADDR input pin selects the MFX I2C device address
 MFX_I2C_ADDR input pin     MFX I2C device address
 0                           b: 1000 010x    (0x84)
 1                           b: 1000 011x    (0x86)
 This input is sampled during the MFX firmware startup.  */
#define IO_I2C_ADDRESS                   ((uint16_t)0x84)  /*mfx MFX_I2C_ADDR 0*/
#define IO_I2C_ADDRESS_2                 ((uint16_t)0x86)  /*mfx MFX_I2C_ADDR 1*/
#define TS_I2C_ADDRESS                   ((uint16_t)0x82)  /*stmpe811 used on MB1046 board */
#define TS3510_I2C_ADDRESS               ((uint16_t)0x80)
#define EXC7200_I2C_ADDRESS              ((uint16_t)0x08)
#define AUDIO_I2C_ADDRESS                ((uint16_t)0x34)
#define EEPROM_I2C_ADDRESS_A01           ((uint16_t)0xA0)
#define EEPROM_I2C_ADDRESS_A02           ((uint16_t)0xA6)
/* I2C clock speed configuration (in Hz)
 WARNING:
 Make sure that this define is not already declared in other files (ie.
 stm32h743i_eval.h file). It can be used in parallel by other modules. */
#ifndef I2C_SPEED
#define I2C_SPEED                        ((uint32_t)100000)
#endif /* I2C_SPEED */

/* User can use this section to tailor I2Cx/I2Cx instance used and associated
 resources */
/* Definition for I2Cx clock resources */
#define EVAL_I2Cx                             I2C2
#define EVAL_I2Cx_CLK_ENABLE()                __HAL_RCC_I2C2_CLK_ENABLE()
//#define EVAL_DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define EVAL_I2Cx_SCL_SDA_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define EVAL_I2Cx_FORCE_RESET()               __HAL_RCC_I2C2_FORCE_RESET()
#define EVAL_I2Cx_RELEASE_RESET()             __HAL_RCC_I2C2_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define EVAL_I2Cx_SCL_PIN                     GPIO_PIN_10
#define EVAL_I2Cx_SCL_SDA_GPIO_PORT           GPIOB
#define EVAL_I2Cx_SCL_SDA_AF                  GPIO_AF4_I2C2
#define EVAL_I2Cx_SDA_PIN                     GPIO_PIN_11

/* I2C interrupt requests */
#define EVAL_I2Cx_EV_IRQn                     I2C2_EV_IRQn
#define EVAL_I2Cx_ER_IRQn                     I2C2_ER_IRQn

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated from APB1 source clock = 50 MHz */
/* 0x40912732 takes in account the big rising and aims a clock of 100khz */
/* this value might be adapted when next Rev Birdie board is available */
#ifndef EVAL_I2Cx_TIMING
#define EVAL_I2Cx_TIMING                      ((uint32_t)0x40912732)
#endif /* EVAL_I2Cx_TIMING */

/**
 * @}
 */

/** @addtogroup STM32H743I_EVAL_LCD_TIM
 * @{
 */
/**
 * @brief Definition for LCD Timer used to control the Brightnes
 */
#define LCD_TIMx                           TIM3
#define LCD_TIMx_CLK_ENABLE()              __HAL_RCC_TIM3_CLK_ENABLE()
#define LCD_TIMx_CLK_DISABLE()             __HAL_RCC_TIM3_CLK_DISABLE()
#define LCD_TIMx_CHANNEL                   TIM_CHANNEL_1
#define LCD_TIMx_CHANNEL_AF                GPIO_AF2_TIM3
#define LCD_TIMX_PERIOD_VALUE              ((uint32_t)50000) /* Period Value    */
#define LCD_TIMX_PRESCALER_VALUE           ((uint32_t)4)     /* Prescaler Value */

uint32_t BSP_GetVersion(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32H743I_EVAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
