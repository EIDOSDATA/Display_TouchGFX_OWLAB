/**
 ******************************************************************************
 * @file    stm32h743i_eval_lcd.c
 * @author  MCD Application Team
 * @version V1.2.0
 * @date    29-December-2017
 * @brief   This file includes the driver for Liquid Crystal Display (LCD) module
 *          mounted on STM32H743I-EVAL evaluation board.
 @verbatim
 How To use this driver:
 -----------------------
 - This driver is used to drive directly an LCD TFT using the LTDC controller.
 - This driver selects dynamically the mounted LCD, AMPIRE 640x480 LCD mounted
 on MB1063 or AMPIRE 480x272 LCD mounted on MB1046 daughter board,
 and uses the adequate timing and setting for the specified LCD using
 device ID of the STMPE811 mounted on MB1046 daughter board.

 Driver description:
 ------------------
 + Initialization steps:
 o Initialize the LCD using the BSP_LCD_Init() function.
 o Apply the Layer configuration using the BSP_LCD_LayerDefaultInit() function.
 o Select the LCD layer to be used using the BSP_LCD_SelectLayer() function.
 o Enable the LCD display using the BSP_LCD_DisplayOn() function.

 + Options
 o Configure and enable the colour keying functionality using the
 BSP_LCD_SetColorKeying() function.
 o Modify in the fly the transparency and/or the frame buffer address
 using the following functions:
 - BSP_LCD_SetTransparency()
 - BSP_LCD_SetLayerAddress()

 + Display on LCD
 o Clear the whole LCD using BSP_LCD_Clear() function or only one specified string
 line using the BSP_LCD_ClearStringLine() function.
 o Display a character on the specified line and column using the BSP_LCD_DisplayChar()
 function or a complete string line using the BSP_LCD_DisplayStringAtLine() function.
 o Display a string line on the specified position (x,y in pixel) and align mode
 using the BSP_LCD_DisplayStringAtLine() function.
 o Draw and fill a basic shapes (dot, line, rectangle, circle, ellipse, .. bitmap)
 on LCD using the available set of functions.
 @endverbatim
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

/* Includes ------------------------------------------------------------------*/
#include "stm32h743i_eval_lcd.h"
#include "fonts.h"
#if 0
#include "font24.c"
#include "font20.c"
#include "font16.c"
#include "font12.c"
#include "font8.c"
#endif

/** @addtogroup BSP
 * @{
 */

/** @addtogroup STM32H743I_EVAL
 * @{
 */

/** @addtogroup STM32H743I_EVAL_LCD
 * @{
 */

/** @defgroup STM32H743I_EVAL_LCD_Private_TypesDefinitions LCD Private TypesDefinitions
 * @{
 */
/**
 * @}
 */

/** @defgroup STM32H743I_EVAL_LCD_Private_Defines LCD Private Defines
 * @{
 */
#define POLY_X(Z)              ((int32_t)((Points + Z)->X))
#define POLY_Y(Z)              ((int32_t)((Points + Z)->Y))
/**
 * @}
 */

/** @defgroup STM32H743I_EVAL_LCD_Private_Macros LCD Private Macros
 * @{
 */
#define ABS(X)  ((X) > 0 ? (X) : -(X))
/**
 * @}
 */

/** @defgroup STM32H743I_EVAL_LCD_Private_Variables LCD Private Variables
 * @{
 */
extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;
//static uint32_t            PCLK_profile = LCD_MAX_PCLK;

/* Timer handler declaration */
//static TIM_HandleTypeDef LCD_TimHandle;
/* Default LCD configuration with LCD Layer 1 */
static uint32_t ActiveLayer = 0;
static LCD_DrawPropTypeDef DrawProp[MAX_LAYER_NUMBER];
/**
 * @}
 */

/** @defgroup STM32H743I_EVAL_LCD_Private_FunctionPrototypes LCD Private FunctionPrototypes
 * @{
 */
static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3);
static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);
static void LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode);

/**
 * @}
 */

/** @addtogroup STM32H743I_EVAL_LCD_Exported_Functions
 * @{
 */

/**
 * @brief  Initializes the LCD.
 * @retval LCD state
 */
uint8_t BSP_LCD_Init(void)
{
	return (BSP_LCD_InitEx(LCD_MAX_PCLK));
}

/**
 * @brief  Initializes the LCD.
 * @param  PclkConfig : pixel clock profile
 * @retval LCD state
 */
uint8_t BSP_LCD_InitEx(uint32_t PclkConfig)
{
#if 0 //ahn
  PCLK_profile = PclkConfig;
  
  /* Select the used LCD */
  /* The AMPIRE 480x272 does not contain an ID register then we check the availability
  of AMPIRE 480x640 LCD using device ID of the STMPE811 mounted on MB1046 daughter board */
  if(stmpe811_ts_drv.ReadID(TS_I2C_ADDRESS) == STMPE811_ID)
  {
    /* The AMPIRE LCD 480x272 is selected */
    /* Timing Configuration */
    hltdc.Init.HorizontalSync = (AMPIRE480272_HSYNC - 1);
    hltdc.Init.VerticalSync = (AMPIRE480272_VSYNC - 1);
    hltdc.Init.AccumulatedHBP = (AMPIRE480272_HSYNC + AMPIRE480272_HBP - 1);
    hltdc.Init.AccumulatedVBP = (AMPIRE480272_VSYNC + AMPIRE480272_VBP - 1);
    hltdc.Init.AccumulatedActiveH = (AMPIRE480272_HEIGHT + AMPIRE480272_VSYNC + AMPIRE480272_VBP - 1);
    hltdc.Init.AccumulatedActiveW = (AMPIRE480272_WIDTH + AMPIRE480272_HSYNC + AMPIRE480272_HBP - 1);
    hltdc.Init.TotalHeigh = (AMPIRE480272_HEIGHT + AMPIRE480272_VSYNC + AMPIRE480272_VBP + AMPIRE480272_VFP - 1);
    hltdc.Init.TotalWidth = (AMPIRE480272_WIDTH + AMPIRE480272_HSYNC + AMPIRE480272_HBP + AMPIRE480272_HFP - 1);

    /* Initialize the LCD pixel width and pixel height */
    hltdc.LayerCfg->ImageWidth  = AMPIRE480272_WIDTH;
    hltdc.LayerCfg->ImageHeight = AMPIRE480272_HEIGHT;
  }
  else
  {
    /* The LCD AMPIRE 640x480 is selected */
    /* Timing configuration */
    hltdc.Init.HorizontalSync = (AMPIRE640480_HSYNC - 1);
    hltdc.Init.VerticalSync = (AMPIRE640480_VSYNC - 1);
    hltdc.Init.AccumulatedHBP = (AMPIRE640480_HSYNC + AMPIRE640480_HBP - 1);
    hltdc.Init.AccumulatedVBP = (AMPIRE640480_VSYNC + AMPIRE640480_VBP - 1);
    hltdc.Init.AccumulatedActiveH = (AMPIRE640480_HEIGHT + AMPIRE640480_VSYNC + AMPIRE640480_VBP - 1);
    hltdc.Init.AccumulatedActiveW = (AMPIRE640480_WIDTH + AMPIRE640480_HSYNC + AMPIRE640480_HBP - 1);
    hltdc.Init.TotalHeigh = (AMPIRE640480_HEIGHT + AMPIRE640480_VSYNC + AMPIRE640480_VBP + AMPIRE640480_VFP - 1);
    hltdc.Init.TotalWidth = (AMPIRE640480_WIDTH + AMPIRE640480_HSYNC + AMPIRE640480_HBP + AMPIRE640480_HFP - 1);

    /* Initialize the LCD pixel width and pixel height */
    hltdc.LayerCfg->ImageWidth  = AMPIRE640480_WIDTH;
    hltdc.LayerCfg->ImageHeight = AMPIRE640480_HEIGHT;
  }
    
  /* Background value */
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;

  /* Polarity */
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Instance = LTDC;

  /* LCD clock configuration */
  BSP_LCD_ClockConfig(&hltdc, &PCLK_profile);

  if(HAL_LTDC_GetState(&hltdc) == HAL_LTDC_STATE_RESET)
  {
    /* Initialize the LCD Msp: this __weak function can be rewritten by the application */
    BSP_LCD_MspInit(&hltdc, NULL);
  }
  HAL_LTDC_Init(&hltdc);


#if !defined(DATA_IN_ExtSDRAM)
	/* When DATA_IN_ExtSDRAM define is enabled, the SDRAM will be configured in SystemInit()
	 function (from system_stm32h7xx.c) before branch to main() routine. In such case, there
	 is no need to reconfigure the SDRAM within the LCD driver, since it's already initialized.
	 Otherwise the SDRAM must be configured. */
	BSP_SDRAM_Init();
#endif

#endif

	/* Initialize the font */
	BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

	/* Initialize TIM in PWM mode to control brightness */
	//TIMx_PWM_Init(&LCD_TimHandle);
	return LCD_OK;
}

/**
 * @brief  DeInitializes the LCD.
 * @retval LCD state
 */
uint8_t BSP_LCD_DeInit(void)
{
	/* Initialize the hltdc_eval Instance parameter */
	hltdc.Instance = LTDC;

	/* Disable LTDC block */
	__HAL_LTDC_DISABLE(&hltdc);

	/* DeInit the LTDC */
	HAL_LTDC_DeInit(&hltdc);

	/* DeInit the LTDC MSP : this __weak function can be rewritten by the application */
	BSP_LCD_MspDeInit(&hltdc, NULL);

	/* DeInit TIM PWM */
	//TIMx_PWM_DeInit(&LCD_TimHandle);
	return LCD_OK;
}

/**
 * @brief  Gets the LCD X size.
 * @retval Used LCD X size
 */
uint32_t BSP_LCD_GetXSize(void)
{
	return hltdc.LayerCfg[ActiveLayer].ImageWidth;
}

/**
 * @brief  Gets the LCD Y size.
 * @retval Used LCD Y size
 */
uint32_t BSP_LCD_GetYSize(void)
{
	return hltdc.LayerCfg[ActiveLayer].ImageHeight;
}

/**
 * @brief  Set the LCD X size.
 * @param  imageWidthPixels : image width in pixels unit
 * @retval None
 */
void BSP_LCD_SetXSize(uint32_t imageWidthPixels)
{
	hltdc.LayerCfg[ActiveLayer].ImageWidth = imageWidthPixels;
}

/**
 * @brief  Set the LCD Y size.
 * @param  imageHeightPixels : image height in lines unit
 * @retval None
 */
void BSP_LCD_SetYSize(uint32_t imageHeightPixels)
{
	hltdc.LayerCfg[ActiveLayer].ImageHeight = imageHeightPixels;
}

extern LTDC_HandleTypeDef hltdc;

/**
 * @brief  Initializes the LCD layers.
 * @param  LayerIndex: Layer foreground or background
 * @param  FB_Address: Layer frame buffer
 * @retval None
 */
void BSP_LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address)
{
	LCD_LayerCfgTypeDef layer_cfg;

	hltdc = hltdc;

	/* Layer Init */
	layer_cfg.WindowX0 = 0;
	layer_cfg.WindowX1 = 1024;
	layer_cfg.WindowY0 = 0;
	layer_cfg.WindowY1 = 600;
	layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
	layer_cfg.FBStartAdress = FB_Address;
	layer_cfg.Alpha = 255;
	layer_cfg.Alpha0 = 0;
	layer_cfg.Backcolor.Blue = 0;
	layer_cfg.Backcolor.Green = 0;
	layer_cfg.Backcolor.Red = 0;
	layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	layer_cfg.ImageWidth = 1024;
	layer_cfg.ImageHeight = 600;

	HAL_LTDC_ConfigLayer(&hltdc, &layer_cfg, LayerIndex);

	DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;
	DrawProp[LayerIndex].pFont = &Font24;
	DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK;
}

/**
 * @brief  Selects the LCD Layer.
 * @param  LayerIndex: Layer foreground or background
 * @retval None
 */
void BSP_LCD_SelectLayer(uint32_t LayerIndex)
{
	ActiveLayer = LayerIndex;
}

/**
 * @brief  Sets an LCD Layer visible
 * @param  LayerIndex: Visible Layer
 * @param  State: New state of the specified layer
 *          This parameter can be one of the following values:
 *            @arg  ENABLE
 *            @arg  DISABLE
 * @retval None
 */
void BSP_LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State)
{
	if (State == ENABLE)
	{
		__HAL_LTDC_LAYER_ENABLE(&hltdc, LayerIndex);
	}
	else
	{
		__HAL_LTDC_LAYER_DISABLE(&hltdc, LayerIndex);
	}
	__HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(&(hltdc));
}

/**
 * @brief  Sets an LCD Layer visible without reloading.
 * @param  LayerIndex: Visible Layer
 * @param  State: New state of the specified layer
 *          This parameter can be one of the following values:
 *            @arg  ENABLE
 *            @arg  DISABLE
 * @retval None
 */
void BSP_LCD_SetLayerVisible_NoReload(uint32_t LayerIndex, FunctionalState State)
{
	if (State == ENABLE)
	{
		__HAL_LTDC_LAYER_ENABLE(&hltdc, LayerIndex);
	}
	else
	{
		__HAL_LTDC_LAYER_DISABLE(&hltdc, LayerIndex);
	}
	/* Do not Sets the Reload  */
}

/**
 * @brief  Configures the transparency.
 * @param  LayerIndex: Layer foreground or background.
 * @param  Transparency: Transparency
 *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF
 * @retval None
 */
void BSP_LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency)
{
	HAL_LTDC_SetAlpha(&hltdc, Transparency, LayerIndex);
}

/**
 * @brief  Configures the transparency without reloading.
 * @param  LayerIndex: Layer foreground or background.
 * @param  Transparency: Transparency
 *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF
 * @retval None
 */
void BSP_LCD_SetTransparency_NoReload(uint32_t LayerIndex, uint8_t Transparency)
{
	HAL_LTDC_SetAlpha_NoReload(&hltdc, Transparency, LayerIndex);
}

/**
 * @brief  Sets an LCD layer frame buffer address.
 * @param  LayerIndex: Layer foreground or background
 * @param  Address: New LCD frame buffer value
 * @retval None
 */
void BSP_LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address)
{
	HAL_LTDC_SetAddress(&hltdc, Address, LayerIndex);
}

/**
 * @brief  Sets an LCD layer frame buffer address without reloading.
 * @param  LayerIndex: Layer foreground or background
 * @param  Address: New LCD frame buffer value
 * @retval None
 */
void BSP_LCD_SetLayerAddress_NoReload(uint32_t LayerIndex, uint32_t Address)
{
	HAL_LTDC_SetAddress_NoReload(&hltdc, Address, LayerIndex);
}

/**
 * @brief  Sets display window.
 * @param  LayerIndex: Layer index
 * @param  Xpos: LCD X position
 * @param  Ypos: LCD Y position
 * @param  Width: LCD window width
 * @param  Height: LCD window height
 * @retval None
 */
void BSP_LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
	/* Reconfigure the layer size */
	HAL_LTDC_SetWindowSize(&hltdc, Width, Height, LayerIndex);

	/* Reconfigure the layer position */
	HAL_LTDC_SetWindowPosition(&hltdc, Xpos, Ypos, LayerIndex);
}

/**
 * @brief  Sets display window without reloading.
 * @param  LayerIndex: Layer index
 * @param  Xpos: LCD X position
 * @param  Ypos: LCD Y position
 * @param  Width: LCD window width
 * @param  Height: LCD window height
 * @retval None
 */
void BSP_LCD_SetLayerWindow_NoReload(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
	/* Reconfigure the layer size */
	HAL_LTDC_SetWindowSize_NoReload(&hltdc, Width, Height, LayerIndex);

	/* Reconfigure the layer position */
	HAL_LTDC_SetWindowPosition_NoReload(&hltdc, Xpos, Ypos, LayerIndex);
}

/**
 * @brief  Configures and sets the color keying.
 * @param  LayerIndex: Layer foreground or background
 * @param  RGBValue: Color reference
 * @retval None
 */
void BSP_LCD_SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue)
{
	/* Configure and Enable the color Keying for LCD Layer */
	HAL_LTDC_ConfigColorKeying(&hltdc, RGBValue, LayerIndex);
	HAL_LTDC_EnableColorKeying(&hltdc, LayerIndex);
}

/**
 * @brief  Configures and sets the color keying without reloading.
 * @param  LayerIndex: Layer foreground or background
 * @param  RGBValue: Color reference
 * @retval None
 */
void BSP_LCD_SetColorKeying_NoReload(uint32_t LayerIndex, uint32_t RGBValue)
{
	/* Configure and Enable the color Keying for LCD Layer */
	HAL_LTDC_ConfigColorKeying_NoReload(&hltdc, RGBValue, LayerIndex);
	HAL_LTDC_EnableColorKeying_NoReload(&hltdc, LayerIndex);
}

/**
 * @brief  Disables the color keying.
 * @param  LayerIndex: Layer foreground or background
 * @retval None
 */
void BSP_LCD_ResetColorKeying(uint32_t LayerIndex)
{
	/* Disable the color Keying for LCD Layer */
	HAL_LTDC_DisableColorKeying(&hltdc, LayerIndex);
}

/**
 * @brief  Disables the color keying without reloading.
 * @param  LayerIndex: Layer foreground or background
 * @retval None
 */
void BSP_LCD_ResetColorKeying_NoReload(uint32_t LayerIndex)
{
	/* Disable the color Keying for LCD Layer */
	HAL_LTDC_DisableColorKeying_NoReload(&hltdc, LayerIndex);
}

/**
 * @brief  Disables the color keying without reloading.
 * @param  ReloadType: can be one of the following values
 *         - LCD_RELOAD_IMMEDIATE
 *         - LCD_RELOAD_VERTICAL_BLANKING
 * @retval None
 */
void BSP_LCD_Relaod(uint32_t ReloadType)
{
	HAL_LTDC_Reload(&hltdc, ReloadType);
}

/**
 * @brief  Sets the LCD text color.
 * @param  Color: Text color code ARGB(8-8-8-8)
 * @retval None
 */
void BSP_LCD_SetTextColor(uint32_t Color)
{
	DrawProp[ActiveLayer].TextColor = Color;
}

/**
 * @brief  Gets the LCD text color.
 * @retval Used text color.
 */
uint32_t BSP_LCD_GetTextColor(void)
{
	return DrawProp[ActiveLayer].TextColor;
}

/**
 * @brief  Sets the LCD background color.
 * @param  Color: Layer background color code ARGB(8-8-8-8)
 * @retval None
 */
void BSP_LCD_SetBackColor(uint32_t Color)
{
	DrawProp[ActiveLayer].BackColor = Color;
}

/**
 * @brief  Gets the LCD background color.
 * @retval Used background color
 */
uint32_t BSP_LCD_GetBackColor(void)
{
	return DrawProp[ActiveLayer].BackColor;
}

/**
 * @brief  Sets the LCD text font.
 * @param  fonts: Layer font to be used
 * @retval None
 */
void BSP_LCD_SetFont(sFONT *fonts)
{
	DrawProp[ActiveLayer].pFont = fonts;
}

/**
 * @brief  Gets the LCD text font.
 * @retval Used layer font
 */
sFONT* BSP_LCD_GetFont(void)
{
	return DrawProp[ActiveLayer].pFont;
}

/**
 * @brief  Reads an LCD pixel.
 * @param  Xpos: X position
 * @param  Ypos: Y position
 * @retval RGB pixel color
 */
uint32_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
	uint32_t ret = 0;

	if (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
	{
		/* Read data value from SDRAM memory */
		ret = *(__IO uint32_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (4 * (Ypos * BSP_LCD_GetXSize() + Xpos)));
	}
	else if (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
	{
		/* Read data value from SDRAM memory */
		ret = (*(__IO uint32_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (4 * (Ypos * BSP_LCD_GetXSize() + Xpos))) & 0x00FFFFFF);
	}
	else if ((hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))
	{
		/* Read data value from SDRAM memory */
		ret = *(__IO uint16_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (2 * (Ypos * BSP_LCD_GetXSize() + Xpos)));
	}
	else
	{
		/* Read data value from SDRAM memory */
		ret = *(__IO uint8_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (2 * (Ypos * BSP_LCD_GetXSize() + Xpos)));
	}

	return ret;
}

/**
 * @brief  Clears the hole LCD.
 * @param  Color: Color of the background
 * @retval None
 */
void BSP_LCD_Clear(uint32_t Color)
{
	/* Clear the LCD */
	LL_FillBuffer(ActiveLayer, (uint32_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress), BSP_LCD_GetXSize(), BSP_LCD_GetYSize(), 0, Color);
}

/**
 * @brief  Clears the selected line.
 * @param  Line: Line to be cleared
 * @retval None
 */
void BSP_LCD_ClearStringLine(uint32_t Line)
{
	uint32_t color_backup = DrawProp[ActiveLayer].TextColor;
	DrawProp[ActiveLayer].TextColor = DrawProp[ActiveLayer].BackColor;

	/* Draw rectangle with background color */
	BSP_LCD_FillRect(0, (Line * DrawProp[ActiveLayer].pFont->Height), BSP_LCD_GetXSize(), DrawProp[ActiveLayer].pFont->Height);

	DrawProp[ActiveLayer].TextColor = color_backup;
	BSP_LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);
}

/**
 * @brief  Displays one character.
 * @param  Xpos: Start column address
 * @param  Ypos: Line where to display the character shape.
 * @param  Ascii: Character ascii code
 *           This parameter must be a number between Min_Data = 0x20 and Max_Data = 0x7E
 * @retval None
 */
void BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
	DrawChar(Xpos, Ypos, &DrawProp[ActiveLayer].pFont->table[(Ascii - ' ') *\
 DrawProp[ActiveLayer].pFont->Height * ((DrawProp[ActiveLayer].pFont->Width + 7) / 8)]);
}

/**
 * @brief  Displays characters on the LCD.
 * @param  Xpos: X position (in pixel)
 * @param  Ypos: Y position (in pixel)
 * @param  Text: Pointer to string to display on LCD
 * @param  Mode: Display mode
 *          This parameter can be one of the following values:
 *            @arg  CENTER_MODE
 *            @arg  RIGHT_MODE
 *            @arg  LEFT_MODE
 * @retval None
 */
void BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
	uint16_t ref_column = 1, i = 0;
	uint32_t size = 0, xsize = 0;
	uint8_t *ptr = Text;

	/* Get the text size */
	while (*ptr++)
		size++;

	/* Characters number per line */
	xsize = (BSP_LCD_GetXSize() / DrawProp[ActiveLayer].pFont->Width);

	switch (Mode)
	{
	case CENTER_MODE:
	{
		ref_column = Xpos + ((xsize - size) * DrawProp[ActiveLayer].pFont->Width) / 2;
		break;
	}
	case LEFT_MODE:
	{
		ref_column = Xpos;
		break;
	}
	case RIGHT_MODE:
	{
		ref_column = -Xpos + ((xsize - size) * DrawProp[ActiveLayer].pFont->Width);
		break;
	}
	default:
	{
		ref_column = Xpos;
		break;
	}
	}

	/* Check that the Start column is located in the screen */
	if ((ref_column < 1) || (ref_column >= 0x8000))
	{
		ref_column = 1;
	}

	/* Send the string character by character on LCD */
	while ((*Text != 0) & (((BSP_LCD_GetXSize() - (i * DrawProp[ActiveLayer].pFont->Width)) & 0xFFFF) >= DrawProp[ActiveLayer].pFont->Width))
	{
		/* Display one character on LCD */
		BSP_LCD_DisplayChar(ref_column, Ypos, *Text);
		/* Decrement the column position by 16 */
		ref_column += DrawProp[ActiveLayer].pFont->Width;
		/* Point on the next character */
		Text++;
		i++;
	}
}

/**
 * @brief  Displays a maximum of 60 characters on the LCD.
 * @param  Line: Line where to display the character shape
 * @param  ptr: Pointer to string to display on LCD
 * @retval None
 */
void BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr)
{
	BSP_LCD_DisplayStringAt(0, LINE(Line), ptr, LEFT_MODE);
}

/**
 * @brief  Draws an horizontal line.
 * @param  Xpos: X position
 * @param  Ypos: Y position
 * @param  Length: Line length
 * @retval None
 */
void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
	uint32_t Xaddress = 0;

	/* Get the line address */
	Xaddress = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 4 * (BSP_LCD_GetXSize() * Ypos + Xpos);

	/* Write line */
	LL_FillBuffer(ActiveLayer, (uint32_t*) Xaddress, Length, 1, 0, DrawProp[ActiveLayer].TextColor);
}

/**
 * @brief  Draws a vertical line.
 * @param  Xpos: X position
 * @param  Ypos: Y position
 * @param  Length: Line length
 * @retval None
 */
void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
	uint32_t Xaddress = 0;

	/* Get the line address */
	Xaddress = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 4 * (BSP_LCD_GetXSize() * Ypos + Xpos);

	/* Write line */
	LL_FillBuffer(ActiveLayer, (uint32_t*) Xaddress, 1, Length, (BSP_LCD_GetXSize() - 1), DrawProp[ActiveLayer].TextColor);
}

/**
 * @brief  Draws an uni-line (between two points).
 * @param  x1: Point 1 X position
 * @param  y1: Point 1 Y position
 * @param  x2: Point 2 X position
 * @param  y2: Point 2 Y position
 * @retval None
 */
void BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, yinc1 = 0, yinc2 = 0, den = 0, num = 0, num_add = 0, num_pixels = 0, curpixel = 0;

	deltax = ABS(x2 - x1); /* The difference between the x's */
	deltay = ABS(y2 - y1); /* The difference between the y's */
	x = x1; /* Start x off at the first pixel */
	y = y1; /* Start y off at the first pixel */

	if (x2 >= x1) /* The x-values are increasing */
	{
		xinc1 = 1;
		xinc2 = 1;
	}
	else /* The x-values are decreasing */
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) /* The y-values are increasing */
	{
		yinc1 = 1;
		yinc2 = 1;
	}
	else /* The y-values are decreasing */
	{
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay) /* There is at least one x-value for every y-value */
	{
		xinc1 = 0; /* Don't change the x when numerator >= denominator */
		yinc2 = 0; /* Don't change the y for every iteration */
		den = deltax;
		num = deltax / 2;
		num_add = deltay;
		num_pixels = deltax; /* There are more x-values than y-values */
	}
	else /* There is at least one y-value for every x-value */
	{
		xinc2 = 0; /* Don't change the x for every iteration */
		yinc1 = 0; /* Don't change the y when numerator >= denominator */
		den = deltay;
		num = deltay / 2;
		num_add = deltax;
		num_pixels = deltay; /* There are more y-values than x-values */
	}

	for (curpixel = 0; curpixel <= num_pixels; curpixel++)
	{
		BSP_LCD_DrawPixel(x, y, DrawProp[ActiveLayer].TextColor); /* Draw the current pixel */
		num += num_add; /* Increase the numerator by the top of the fraction */
		if (num >= den) /* Check if numerator >= denominator */
		{
			num -= den; /* Calculate the new numerator value */
			x += xinc1; /* Change the x as appropriate */
			y += yinc1; /* Change the y as appropriate */
		}
		x += xinc2; /* Change the x as appropriate */
		y += yinc2; /* Change the y as appropriate */
	}
}

/**
 * @brief  Draws a rectangle.
 * @param  Xpos: X position
 * @param  Ypos: Y position
 * @param  Width: Rectangle width
 * @param  Height: Rectangle height
 * @retval None
 */
void BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
	/* Draw horizontal lines */
	BSP_LCD_DrawHLine(Xpos, Ypos, Width);
	BSP_LCD_DrawHLine(Xpos, (Ypos + Height - 1), Width);

	/* Draw vertical lines */
	BSP_LCD_DrawVLine(Xpos, Ypos, Height);
	BSP_LCD_DrawVLine((Xpos + Width - 1), Ypos, Height);
}

/**
 * @brief  Draws a circle.
 * @param  Xpos: X position
 * @param  Ypos: Y position
 * @param  Radius: Circle radius
 * @retval None
 */
void BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
	int32_t decision; /* Decision Variable */
	uint32_t current_x; /* Current X Value */
	uint32_t current_y; /* Current Y Value */

	decision = 3 - (Radius << 1);
	current_x = 0;
	current_y = Radius;

	while (current_x <= current_y)
	{
		BSP_LCD_DrawPixel((Xpos + current_x), (Ypos - current_y), DrawProp[ActiveLayer].TextColor);

		BSP_LCD_DrawPixel((Xpos - current_x), (Ypos - current_y), DrawProp[ActiveLayer].TextColor);

		BSP_LCD_DrawPixel((Xpos + current_y), (Ypos - current_x), DrawProp[ActiveLayer].TextColor);

		BSP_LCD_DrawPixel((Xpos - current_y), (Ypos - current_x), DrawProp[ActiveLayer].TextColor);

		BSP_LCD_DrawPixel((Xpos + current_x), (Ypos + current_y), DrawProp[ActiveLayer].TextColor);

		BSP_LCD_DrawPixel((Xpos - current_x), (Ypos + current_y), DrawProp[ActiveLayer].TextColor);

		BSP_LCD_DrawPixel((Xpos + current_y), (Ypos + current_x), DrawProp[ActiveLayer].TextColor);

		BSP_LCD_DrawPixel((Xpos - current_y), (Ypos + current_x), DrawProp[ActiveLayer].TextColor);

		if (decision < 0)
		{
			decision += (current_x << 2) + 6;
		}
		else
		{
			decision += ((current_x - current_y) << 2) + 10;
			current_y--;
		}
		current_x++;
	}
}

/**
 * @brief  Draws an poly-line (between many points).
 * @param  Points: Pointer to the points array
 * @param  PointCount: Number of points
 * @retval None
 */
void BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount) //다각형
{
	int16_t x = 0, y = 0;

	if (PointCount < 2)
	{
		return;
	}

	BSP_LCD_DrawLine(Points->X, Points->Y, (Points + PointCount - 1)->X, (Points + PointCount - 1)->Y);

	while (--PointCount)
	{
		x = Points->X;
		y = Points->Y;
		Points++;
		BSP_LCD_DrawLine(x, y, Points->X, Points->Y);
	}
}

/**
 * @brief  Draws an ellipse on LCD.
 * @param  Xpos: X position
 * @param  Ypos: Y position
 * @param  XRadius: Ellipse X radius
 * @param  YRadius: Ellipse Y radius
 * @retval None
 */
void BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius) //타원
{
	int x = 0, y = -YRadius, err = 2 - 2 * XRadius, e2;
	float k = 0, rad1 = 0, rad2 = 0;

	rad1 = XRadius;
	rad2 = YRadius;

	k = (float) (rad2 / rad1);

	do
	{
		BSP_LCD_DrawPixel((Xpos - (uint16_t) (x / k)), (Ypos + y), DrawProp[ActiveLayer].TextColor);
		BSP_LCD_DrawPixel((Xpos + (uint16_t) (x / k)), (Ypos + y), DrawProp[ActiveLayer].TextColor);
		BSP_LCD_DrawPixel((Xpos + (uint16_t) (x / k)), (Ypos - y), DrawProp[ActiveLayer].TextColor);
		BSP_LCD_DrawPixel((Xpos - (uint16_t) (x / k)), (Ypos - y), DrawProp[ActiveLayer].TextColor);

		e2 = err;
		if (e2 <= x)
		{
			err += ++x * 2 + 1;
			if (-y == x && e2 <= y)
				e2 = 0;
		}
		if (e2 > y)
			err += ++y * 2 + 1;
	} while (y <= 0);
}

/**
 * @brief  Draws a pixel on LCD.
 * @param  Xpos: X position
 * @param  Ypos: Y position
 * @param  RGB_Code: Pixel color in ARGB mode (8-8-8-8)
 * @retval None
 */
void BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code)
{
	/* Write data value to all SDRAM memory */
	*(__IO uint32_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (4 * (Ypos * BSP_LCD_GetXSize() + Xpos))) = RGB_Code;
}

/**
 * @brief  Draws a bitmap picture loaded in the internal Flash (32 bpp).
 * @param  Xpos: Bmp X position in the LCD
 * @param  Ypos: Bmp Y position in the LCD
 * @param  pbmp: Pointer to Bmp picture address in the internal Flash
 * @retval None
 */
void BSP_LCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp)
{
	uint32_t index = 0, width = 0, height = 0, bit_pixel = 0;
	uint32_t address;
	uint32_t input_color_mode = 0;

	if (pbmp[0] != 'B' || pbmp[1] != 'M')
	{
		return;
	}

	/* Get bitmap data address offset */
	index = pbmp[10] + (pbmp[11] << 8) + (pbmp[12] << 16) + (pbmp[13] << 24);

	/* Read bitmap width */
	width = pbmp[18] + (pbmp[19] << 8) + (pbmp[20] << 16) + (pbmp[21] << 24);

	/* Read bitmap height */
	height = pbmp[22] + (pbmp[23] << 8) + (pbmp[24] << 16) + (pbmp[25] << 24);

	/* Read bit/pixel */
	bit_pixel = pbmp[28] + (pbmp[29] << 8);

	/* Set the address */
	address = hltdc.LayerCfg[ActiveLayer].FBStartAdress + (((BSP_LCD_GetXSize() * Ypos) + Xpos) * (4));

	/* Get the layer pixel format */
	if ((bit_pixel / 8) == 4)
	{
		input_color_mode = DMA2D_INPUT_ARGB8888;
	}
	else if ((bit_pixel / 8) == 2)
	{
		input_color_mode = DMA2D_INPUT_RGB565;
	}
	else
	{
		input_color_mode = DMA2D_INPUT_RGB888;
	}

	/* Bypass the bitmap header */
	pbmp += (index + (width * (height - 1) * (bit_pixel / 8)));
	pbmp += ((width % 4) * (height - 1));

	/* Convert picture to ARGB8888 pixel format */
	for (index = 0; index < height; index++)
	{
		/* Pixel format conversion */
		LL_ConvertLineToARGB8888((uint32_t*) pbmp, (uint32_t*) address, width, input_color_mode);

		/* Increment the source and destination buffers */
		address += (BSP_LCD_GetXSize() * 4);
		pbmp -= (width * (bit_pixel / 8) + (width % 4));
	}
}

/**
 * @brief  Draws a full rectangle.
 * @param  Xpos: X position
 * @param  Ypos: Y position
 * @param  Width: Rectangle width
 * @param  Height: Rectangle height
 * @retval None
 */
void BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
	uint32_t x_address = 0;

	/* Set the text color */
	BSP_LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);

	/* Get the rectangle start address */
	x_address = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 4 * (BSP_LCD_GetXSize() * Ypos + Xpos);

	/* Fill the rectangle */
	LL_FillBuffer(ActiveLayer, (uint32_t*) x_address, Width, Height, (BSP_LCD_GetXSize() - Width), DrawProp[ActiveLayer].TextColor);
}

/**
 * @brief  Draws a full circle.
 * @param  Xpos: X position
 * @param  Ypos: Y position
 * @param  Radius: Circle radius
 * @retval None
 */
void BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
	int32_t decision; /* Decision Variable */
	uint32_t current_x; /* Current X Value */
	uint32_t current_y; /* Current Y Value */

	decision = 3 - (Radius << 1);

	current_x = 0;
	current_y = Radius;

	BSP_LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);

	while (current_x <= current_y)
	{
		if (current_y > 0)
		{
			BSP_LCD_DrawHLine(Xpos - current_y, Ypos + current_x, 2 * current_y);
			BSP_LCD_DrawHLine(Xpos - current_y, Ypos - current_x, 2 * current_y);
		}

		if (current_x > 0)
		{
			BSP_LCD_DrawHLine(Xpos - current_x, Ypos - current_y, 2 * current_x);
			BSP_LCD_DrawHLine(Xpos - current_x, Ypos + current_y, 2 * current_x);
		}
		if (decision < 0)
		{
			decision += (current_x << 2) + 6;
		}
		else
		{
			decision += ((current_x - current_y) << 2) + 10;
			current_y--;
		}
		current_x++;
	}

	BSP_LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);
	BSP_LCD_DrawCircle(Xpos, Ypos, Radius);
}

/**
 * @brief  Draws a full poly-line (between many points).
 * @param  Points: Pointer to the points array
 * @param  PointCount: Number of points
 * @retval None
 */
void BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount) //다각형
{
	int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
	uint16_t image_left = 0, image_right = 0, image_top = 0, image_bottom = 0;

	image_left = image_right = Points->X;
	image_top = image_bottom = Points->Y;

	for (counter = 1; counter < PointCount; counter++)
	{
		pixelX = POLY_X(counter);
		if (pixelX < image_left)
		{
			image_left = pixelX;
		}
		if (pixelX > image_right)
		{
			image_right = pixelX;
		}

		pixelY = POLY_Y(counter);
		if (pixelY < image_top)
		{
			image_top = pixelY;
		}
		if (pixelY > image_bottom)
		{
			image_bottom = pixelY;
		}
	}

	if (PointCount < 2)
	{
		return;
	}

	X_center = (image_left + image_right) / 2;
	Y_center = (image_bottom + image_top) / 2;

	X_first = Points->X;
	Y_first = Points->Y;

	while (--PointCount)
	{
		X = Points->X;
		Y = Points->Y;
		Points++;
		X2 = Points->X;
		Y2 = Points->Y;

		FillTriangle(X, X2, X_center, Y, Y2, Y_center);
		FillTriangle(X, X_center, X2, Y, Y_center, Y2);
		FillTriangle(X_center, X2, X, Y_center, Y2, Y);
	}

	FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
	FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
	FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first);
}

/**
 * @brief  Draws a full ellipse.
 * @param  Xpos: X position
 * @param  Ypos: Y position
 * @param  XRadius: Ellipse X radius
 * @param  YRadius: Ellipse Y radius
 * @retval None
 */
void BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius) //타원
{
	int x = 0, y = -YRadius, err = 2 - 2 * XRadius, e2;
	float k = 0, rad1 = 0, rad2 = 0;

	rad1 = XRadius;
	rad2 = YRadius;

	k = (float) (rad2 / rad1);

	do
	{
		BSP_LCD_DrawHLine((Xpos - (uint16_t) (x / k)), (Ypos + y), (2 * (uint16_t) (x / k) + 1));
		BSP_LCD_DrawHLine((Xpos - (uint16_t) (x / k)), (Ypos - y), (2 * (uint16_t) (x / k) + 1));

		e2 = err;
		if (e2 <= x)
		{
			err += ++x * 2 + 1;
			if (-y == x && e2 <= y)
				e2 = 0;
		}
		if (e2 > y)
			err += ++y * 2 + 1;
	} while (y <= 0);
}

/**
 * @brief  Enables the display.
 * @retval None
 */
void BSP_LCD_DisplayOn(void)
{
	/* Display On */
	__HAL_LTDC_ENABLE(&hltdc);
}

/**
 * @brief  Disables the display.
 * @retval None
 */
void BSP_LCD_DisplayOff(void)
{
	/* Display Off */
	__HAL_LTDC_DISABLE(&hltdc);
}

/**
 * @brief  Initializes the LTDC MSP.
 * @param  hltdc: LTDC handle
 * @param  Params: Pointer to void
 * @retval None
 */
__weak void BSP_LCD_MspInit(LTDC_HandleTypeDef *hltdc, void *Params)
{
	GPIO_InitTypeDef gpio_init_structure;

	/* Enable the LTDC and DMA2D clocks */
	__HAL_RCC_LTDC_CLK_ENABLE();
	__HAL_RCC_DMA2D_CLK_ENABLE();

	/* Enable GPIOs clock */
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	__HAL_RCC_GPIOK_CLK_ENABLE();

	/*** LTDC Pins configuration ***/
	/* GPIOI configuration */
	gpio_init_structure.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	gpio_init_structure.Mode = GPIO_MODE_AF_PP;
	gpio_init_structure.Pull = GPIO_NOPULL;
	gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_structure.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOI, &gpio_init_structure);

	/* GPIOJ configuration */
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
	GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
	GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
	GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	gpio_init_structure.Mode = GPIO_MODE_AF_PP;
	gpio_init_structure.Pull = GPIO_NOPULL;
	gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_structure.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

	/* GPIOK configuration */
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
	GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	gpio_init_structure.Mode = GPIO_MODE_AF_PP;
	gpio_init_structure.Pull = GPIO_NOPULL;
	gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_structure.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOK, &gpio_init_structure);
}

/**
 * @brief  DeInitializes BSP_LCD MSP.
 * @param  hltdc: LTDC handle
 * @param  Params: Pointer to void
 * @retval None
 */
__weak void BSP_LCD_MspDeInit(LTDC_HandleTypeDef *hltdc, void *Params)
{
	GPIO_InitTypeDef gpio_init_structure;

	/* Disable LTDC block */
	__HAL_LTDC_DISABLE(hltdc);

	/* LTDC Pins deactivation */
	/* GPIOI deactivation */
	gpio_init_structure.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_DeInit(GPIOI, gpio_init_structure.Pin);
	/* GPIOJ deactivation */
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
	GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
	GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
	GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_DeInit(GPIOJ, gpio_init_structure.Pin);
	/* GPIOK deactivation */
	gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
	GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	HAL_GPIO_DeInit(GPIOK, gpio_init_structure.Pin);

	/* Disable LTDC clock */
	__HAL_RCC_LTDC_CLK_DISABLE();

	/* GPIO pins clock can be shut down in the application
	 by surcharging this __weak function */
}

/**
 * @brief  Clock Config.
 * @param  hltdc: LTDC handle
 * @param  Params: Pointer to void
 * @note   This API is called by BSP_LCD_Init()
 *         Being __weak it can be overwritten by the application
 * @retval None
 */
__weak void BSP_LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params)
{
#if 0
  static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;
  
  if(stmpe811_ts_drv.ReadID(TS_I2C_ADDRESS) == STMPE811_ID)
  {
    /* AMPIRE480272 LCD clock configuration */
    /* LCD clock configuration */
    /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
    /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
    /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/83 = 9.63 Mhz */
    /* LTDC clock frequency = PLLLCDCLK = 9.63 Mhz */
    periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periph_clk_init_struct.PLL3.PLL3M = 5;
    periph_clk_init_struct.PLL3.PLL3N = 160;
    periph_clk_init_struct.PLL3.PLL3P = 2;
    periph_clk_init_struct.PLL3.PLL3Q = 2;
    periph_clk_init_struct.PLL3.PLL3R = 83;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
  }
  else
  {
    /* The programmed LTDC pixel clock depends on the vertical refresh rate of the panel 60Hz => 25.16MHz and
       the LCD/SDRAM bandwidth affected by the several access on the bus and the number of used layers.
    */
    if(*(uint32_t *)Params == LCD_MAX_PCLK)
    {
      /* In case of single layer the bandwidth is arround 160MBytesPerSec ==> theorical PCLK of 40MHz */
      /* AMPIRE640480 typical PCLK is 25 MHz so the PLL3R is configured to provide this clock */
      /* AMPIRE640480 LCD clock configuration */
      /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
      /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
      /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/32 = 25Mhz */
      /* LTDC clock frequency = PLLLCDCLK = 25 Mhz */
      periph_clk_init_struct.PLL3.PLL3R = 32;
    }
    else
    {
      /* In case of double layers the bandwidth is arround 80MBytesPerSec => 20MHz (<25MHz) */
      /* so the PLL3R is configured to provide this clock */
      /* AMPIRE640480 LCD clock configuration */
      /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
      /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
      /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/40 = 20Mhz */
      /* LTDC clock frequency = PLLLCDCLK = 20 Mhz */
      periph_clk_init_struct.PLL3.PLL3R = 40;
    }
    periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periph_clk_init_struct.PLL3.PLL3M = 5;
    periph_clk_init_struct.PLL3.PLL3N = 160;
    periph_clk_init_struct.PLL3.PLL3P = 2;
    periph_clk_init_struct.PLL3.PLL3Q = 2;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
  }
#endif
}

/**
 * @brief  Set the brightness value
 * @param  BrightnessValue: [00: Min (black), 100 Max]
 */
void BSP_LCD_SetBrightness(uint8_t BrightnessValue)
{

//  /* Timer Configuration */
//  TIM_OC_InitTypeDef LCD_TIM_Config;
//
//  /* Stop PWM Timer channel */
//  HAL_TIM_PWM_Stop(&LCD_TimHandle, LCD_TIMx_CHANNEL);
//
//  /* Common configuration for all channels */
//  LCD_TIM_Config.OCMode       = TIM_OCMODE_PWM1;
//  LCD_TIM_Config.OCPolarity   = TIM_OCPOLARITY_HIGH;
//  LCD_TIM_Config.OCFastMode   = TIM_OCFAST_DISABLE;
//  LCD_TIM_Config.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
//  LCD_TIM_Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
//  LCD_TIM_Config.OCIdleState  = TIM_OCIDLESTATE_RESET;
//
//  /* Set the pulse value for channel */
//  LCD_TIM_Config.Pulse =  (uint32_t)((LCD_TIMX_PERIOD_VALUE * BrightnessValue) / 100);
//
//  HAL_TIM_PWM_ConfigChannel(&LCD_TimHandle, &LCD_TIM_Config, LCD_TIMx_CHANNEL);
//
//  /* Start PWM Timer channel */
//  HAL_TIM_PWM_Start(&LCD_TimHandle, LCD_TIMx_CHANNEL);
}

/**
 * @}
 */

/*******************************************************************************
 Static Functions
 *******************************************************************************/

/** @defgroup STM32H743I_EVAL_LCD_Private_Functions LCD Private Functions
 * @{
 */

/**
 * @brief  Draws a character on LCD.
 * @param  Xpos: Line where to display the character shape
 * @param  Ypos: Start column address
 * @param  c: Pointer to the character data
 * @retval None
 */
static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
	uint32_t i = 0, j = 0;
	uint16_t height, width;
	uint8_t offset;
	uint8_t *pchar;
	uint32_t line;

	height = DrawProp[ActiveLayer].pFont->Height;
	width = DrawProp[ActiveLayer].pFont->Width;

	offset = 8 * ((width + 7) / 8) - width;

	for (i = 0; i < height; i++)
	{
		pchar = ((uint8_t*) c + (width + 7) / 8 * i);

		switch (((width + 7) / 8))
		{

		case 1:
			line = pchar[0];
			break;

		case 2:
			line = (pchar[0] << 8) | pchar[1];
			break;

		case 3:
		default:
			line = (pchar[0] << 16) | (pchar[1] << 8) | pchar[2];
			break;
		}

		for (j = 0; j < width; j++)
		{
			if (line & (1 << (width - j + offset - 1)))
			{
				BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].TextColor);
			}
			else
			{
				BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].BackColor);
			}
		}
		Ypos++;
	}
}

/**
 * @brief  Fills a triangle (between 3 points).
 * @param  x1: Point 1 X position
 * @param  y1: Point 1 Y position
 * @param  x2: Point 2 X position
 * @param  y2: Point 2 Y position
 * @param  x3: Point 3 X position
 * @param  y3: Point 3 Y position
 * @retval None
 */
static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, yinc1 = 0, yinc2 = 0, den = 0, num = 0, num_add = 0, num_pixels = 0, curpixel = 0;

	deltax = ABS(x2 - x1); /* The difference between the x's */
	deltay = ABS(y2 - y1); /* The difference between the y's */
	x = x1; /* Start x off at the first pixel */
	y = y1; /* Start y off at the first pixel */

	if (x2 >= x1) /* The x-values are increasing */
	{
		xinc1 = 1;
		xinc2 = 1;
	}
	else /* The x-values are decreasing */
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) /* The y-values are increasing */
	{
		yinc1 = 1;
		yinc2 = 1;
	}
	else /* The y-values are decreasing */
	{
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay) /* There is at least one x-value for every y-value */
	{
		xinc1 = 0; /* Don't change the x when numerator >= denominator */
		yinc2 = 0; /* Don't change the y for every iteration */
		den = deltax;
		num = deltax / 2;
		num_add = deltay;
		num_pixels = deltax; /* There are more x-values than y-values */
	}
	else /* There is at least one y-value for every x-value */
	{
		xinc2 = 0; /* Don't change the x for every iteration */
		yinc1 = 0; /* Don't change the y when numerator >= denominator */
		den = deltay;
		num = deltay / 2;
		num_add = deltax;
		num_pixels = deltay; /* There are more y-values than x-values */
	}

	for (curpixel = 0; curpixel <= num_pixels; curpixel++)
	{
		BSP_LCD_DrawLine(x, y, x3, y3);

		num += num_add; /* Increase the numerator by the top of the fraction */
		if (num >= den) /* Check if numerator >= denominator */
		{
			num -= den; /* Calculate the new numerator value */
			x += xinc1; /* Change the x as appropriate */
			y += yinc1; /* Change the y as appropriate */
		}
		x += xinc2; /* Change the x as appropriate */
		y += yinc2; /* Change the y as appropriate */
	}
}

/**
 * @brief  Fills a buffer.
 * @param  LayerIndex: Layer index
 * @param  pDst: Pointer to destination buffer
 * @param  xSize: Buffer width
 * @param  ySize: Buffer height
 * @param  OffLine: Offset
 * @param  ColorIndex: Color index
 * @retval None
 */
static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
{
	/* Register to memory mode with ARGB8888 as color Mode */
	hdma2d.Init.Mode = DMA2D_R2M;
	hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
	hdma2d.Init.OutputOffset = OffLine;

	hdma2d.Instance = DMA2D;

	/* DMA2D Initialization */
	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
		{
			if (HAL_DMA2D_Start(&hdma2d, ColorIndex, (uint32_t) pDst, xSize, ySize) == HAL_OK)
			{
				/* Polling For DMA transfer */
				HAL_DMA2D_PollForTransfer(&hdma2d, 10);
			}
		}
	}
}

/**
 * @brief  Converts a line to an ARGB8888 pixel format.
 * @param  pSrc: Pointer to source buffer
 * @param  pDst: Output color
 * @param  xSize: Buffer width
 * @param  ColorMode: Input color mode
 * @retval None
 */
static void LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode)
{
	/* Configure the DMA2D Mode, Color Mode and output offset */
	hdma2d.Init.Mode = DMA2D_M2M_PFC;
	hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
	hdma2d.Init.OutputOffset = 0;

	/* Foreground Configuration */
	hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	hdma2d.LayerCfg[1].InputAlpha = 0xFF;
	hdma2d.LayerCfg[1].InputColorMode = ColorMode;
	hdma2d.LayerCfg[1].InputOffset = 0;

	hdma2d.Instance = DMA2D;

	/* DMA2D Initialization */
	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
		{
			if (HAL_DMA2D_Start(&hdma2d, (uint32_t) pSrc, (uint32_t) pDst, xSize, 1) == HAL_OK)
			{
				/* Polling For DMA transfer */
				HAL_DMA2D_PollForTransfer(&hdma2d, 10);
			}
		}
	}
}

#if 0
#include "main.h"
#include "includes.h"
//#define INTERNAL_BUFFER                          0xD1700000    

//extern FATFS fs;  
extern _SDRAM_ADDRESS_t *pSD;
void BSP_LCD_DrawBitmapFromSD(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp_name)
{
	/* Draw BMP form SD Card */
	/* Internal Buffer defined in SDRAM memory */
	uint8_t *uwInternelBuffer;

	uwInternelBuffer = (uint8_t*) pSD->INTERNAL_BUFFER;
	//uwInternelBuffer = (uint8_t *)INTERNAL_BUFFER;
	if (f_mount(&fs, (TCHAR const*) "", 0))
	{
		return;
	}

	/* Open a file and copy its content to an internal buffer */
	Storage_OpenReadFile(uwInternelBuffer, (const char*) pbmp_name);
	/* Write bmp file on LCD frame buffer */
	BSP_LCD_DrawBitmap(Xpos, Ypos, uwInternelBuffer);

}
#endif

/* User Function */
/*
 * x_out  : 이미지 시작 x좌표
 * y_out  : 이미지 시작 y좌표
 * x_size : 이미지 x 사이즈
 * y_size : 이미지 y 사이즈
 * x_in   : LCD x 좌표
 * y_in   : LCD y 좌표
 */
void LCD_NORD_JP1_isplay(uint32_t x_out, uint32_t y_out, uint32_t x_size, uint32_t y_size, uint32_t x_in, uint32_t y_in, uint8_t *pbmp)
{

	uint32_t index = 0, image_width = 0, image_height = 0, bit_pixel = 0;
	uint32_t address;
	uint32_t input_color_mode = 0;

	if (pbmp[0] != 'B' || pbmp[1] != 'M')
		return;

	/* Get bitmap data address offset */
	index = pbmp[10] + (pbmp[11] << 8) + (pbmp[12] << 16) + (pbmp[13] << 24);

	/* Read bitmap width */
	image_width = pbmp[18] + (pbmp[19] << 8) + (pbmp[20] << 16) + (pbmp[21] << 24);

	/* Read bitmap height */
	image_height = pbmp[22] + (pbmp[23] << 8) + (pbmp[24] << 16) + (pbmp[25] << 24);

	/* Read bit/pixel */
	bit_pixel = pbmp[28] + (pbmp[29] << 8);

	/* Set the address */
	address = hltdc.LayerCfg[ActiveLayer].FBStartAdress + (((BSP_LCD_GetXSize() * y_in) + x_in) * (4));

	/* Get the layer pixel format */
	if ((bit_pixel / 8) == 4)
	{
		input_color_mode = DMA2D_INPUT_ARGB8888;
	}
	else if ((bit_pixel / 8) == 2)
	{
		input_color_mode = DMA2D_INPUT_RGB565;
	}
	else
	{
		input_color_mode = DMA2D_INPUT_RGB888;
	}

	/* Bypass the bitmap header */
	//pbmp += (index + (image_width * (image_height - 1) * (bit_pixel/8))) + (((image_width*y_out) + x_out)*(bit_pixel/8));
	// pbmp += ((image_width % 4)*(image_height - 1));
//  pbmp += (index + (1024 * (600 - 1) * (bit_pixel/8))) + (((1024*y_out) + x_out)*(bit_pixel/8));
//  pbmp += ((1024 % 4)*(600 - 1));  
	pbmp += (index + (image_width * (image_height - 1) * (bit_pixel / 8))) - (int32_t) ((int32_t) ((image_width * y_out) - x_out) * (int32_t) (bit_pixel / 8));
	pbmp += ((image_width % 4) * (image_height - 1));

	/* Convert picture to ARGB8888 pixel format */
	for (index = 0; index < y_size; index++)
	{
		/* Pixel format conversion */
		LL_ConvertLineToARGB8888((uint32_t*) pbmp, (uint32_t*) address, x_size, input_color_mode);

		/* Increment the source and destination buffers */
		address += (BSP_LCD_GetXSize() * 4);
		pbmp -= (1024 * (bit_pixel / 8) + (1024 % 4));
	}
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

