/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ltdc.c
  * @brief   This file provides code for the configuration
  *          of the LTDC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "ltdc.h"
#include "main.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc;

/* LTDC init function */
void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */
//  HAL_LTDC_MspInit(&hltdc);
  /* USER CODE END LTDC_Init 0 */

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 4;
  hltdc.Init.VerticalSync = 4;
  hltdc.Init.AccumulatedHBP = 12;
  hltdc.Init.AccumulatedVBP = 12;
  hltdc.Init.AccumulatedActiveW = 812;
  hltdc.Init.AccumulatedActiveH = 492;
  hltdc.Init.TotalWidth = 820;
  hltdc.Init.TotalHeigh = 506;
  hltdc.Init.Backcolor.Blue = 0x40;
  hltdc.Init.Backcolor.Green = 0x00;
  hltdc.Init.Backcolor.Red = 0x00;
#if 0
#if 1
  hltdc.Init.HorizontalSync = 3;
  hltdc.Init.VerticalSync = 3;
  hltdc.Init.AccumulatedHBP = 11;
  hltdc.Init.AccumulatedVBP = 11;
  hltdc.Init.AccumulatedActiveW = 811;
  hltdc.Init.AccumulatedActiveH = 491;
  hltdc.Init.TotalWidth = 817;
  hltdc.Init.TotalHeigh = 497;
#else
  hltdc.Init.HorizontalSync = 3;
  hltdc.Init.VerticalSync = 3;
  hltdc.Init.AccumulatedHBP = 7;
  hltdc.Init.AccumulatedVBP = 7;
  hltdc.Init.AccumulatedActiveW = 807;
  hltdc.Init.AccumulatedActiveH = 487;
  hltdc.Init.TotalWidth = 811;
  hltdc.Init.TotalHeigh = 491;
#endif
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
#endif
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }

  LTDC_LayerCfgTypeDef pLayerCfg1 = {0};
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = DISPLAY_SIZE_W;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = DISPLAY_SIZE_H;
#if (DISPLAY_BYTES_PER_PIXEL == 2)
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
#elif (DISPLAY_BYTES_PER_PIXEL == 3)
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
#elif (DISPLAY_BYTES_PER_PIXEL == 4)
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
#endif
  pLayerCfg1.Alpha = 0xFF;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
//  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
//  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR1_PAxCA;

  pLayerCfg1.FBStartAdress = (uint32_t)FrameBuffer[0];
  pLayerCfg1.ImageWidth = DISPLAY_SIZE_W;
  pLayerCfg1.ImageHeight = DISPLAY_SIZE_H;
  pLayerCfg1.Backcolor.Blue = 0x40;
  pLayerCfg1.Backcolor.Green = 0x00;
  pLayerCfg1.Backcolor.Red = 0x00;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, LTDC_LAYER_1) != HAL_OK)
  {
    Error_Handler();
  }

#if (LTDC_USE_LAYER_2)
  LTDC_LayerCfgTypeDef pLayerCfg2 = {0};
  pLayerCfg2.WindowX0 = 0;
  pLayerCfg2.WindowX1 = 0;
  pLayerCfg2.WindowY0 = 0;
  pLayerCfg2.WindowY1 = 0;
#if (DISPLAY_BYTES_PER_PIXEL == 2)
  pLayerCfg2.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
#elif (DISPLAY_BYTES_PER_PIXEL == 3)
  pLayerCfg2.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
#elif (DISPLAY_BYTES_PER_PIXEL == 4)
  pLayerCfg2.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
#endif
  pLayerCfg2.Alpha = 0xFF;
  pLayerCfg2.Alpha0 = 0;
  pLayerCfg2.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg2.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg2.FBStartAdress = (uint32_t) FrameBuffer[1];
  pLayerCfg2.ImageWidth = DISPLAY_SIZE_W;
  pLayerCfg2.ImageHeight = DISPLAY_SIZE_H;
  pLayerCfg2.Backcolor.Blue = 0;
  pLayerCfg2.Backcolor.Green = 0;
  pLayerCfg2.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg2, LTDC_LAYER_2) != HAL_OK)
  {
    Error_Handler();
  }
#endif
  /* USER CODE BEGIN LTDC_Init 2 */

  /* USER CODE END LTDC_Init 2 */

}


/**
  * @brief  Initialize the BSP LTDC Msp.
  * @param  hltdc  LTDC handle
  * @retval None
  */
void LTDC_MspInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct = {0};

  if (hltdc.Instance == LTDC)
  {
    __HAL_RCC_LTDC_CLK_ENABLE();

    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();

    /* STM32N6570-DK MB1860A RK050HR18 */
    /* PG0  R0          */
    /* PD9  R1          */
    /* PD15 R2          */
    /* PB4  R3          */ // JTAG
    /* PH4  R4          */
    /* PA15 R5          */ // JTAG JTDI
    /* PG11 R6          */
    /* PD8  R7          */
    /* PG12 G0          */
    /* PG1  G1          */
    /* PA1  G2          */
    /* PA0  G3          */
    /* PB15 G4          */
    /* PB12 G5          */
    /* PB11 G6          */
    /* PG8  G7          */
    /* P15  B0          */
    /* PA7  B1          */
    /* PB2  B2          */
    /* PG6  B3          */
    /* PH3  B4          */
    /* PH6  B5          */
    /* PA8  B6          */
    /* PA2  B7          */
    /*                  */
    /* PG13 LCD_DE      */
    /* PQ3  LCD_ONOFF   */
    /* PB14 LCD_HSYNC   */
    /* PE11 PCD_VSYNC   */
    /* PB13 LCD_CLK     */
    /* PQ4  LCD_INT     */
    /* PQ6  LCD_BL_CTRL */
    /* PE1  NRST        */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOQ_CLK_ENABLE();

    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

    /* G3, G2, B7, B1, B6, R5 */
    GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_15;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* LCD_CLK, LCD_HSYNC B2, R3, G6, G5, G4 */
    GPIO_InitStruct.Pin       = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* R7, R1, R2 */
    GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_15;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* LCD_VSYNC */
    GPIO_InitStruct.Pin       = GPIO_PIN_11;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* R0, G1, B3, G7, R6, G0 */
    GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12 ;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* B4, R4, B5 */
    GPIO_InitStruct.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /* NRST */
    GPIO_InitStruct.Pin       = GPIO_PIN_1;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* LCD_ONOFF, LCD_BL_CTRL */
    GPIO_InitStruct.Pin       = GPIO_PIN_3 | GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOQ, &GPIO_InitStruct);

    /* LCD_DE */
    GPIO_InitStruct.Pin       = LCD_DE_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(LCD_DE_GPIO_Port, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOQ, GPIO_PIN_3, GPIO_PIN_SET); /* LCD On */ /* PQ3  LCD_ONOFF   */
    HAL_GPIO_WritePin(LCD_DE_GPIO_Port, LCD_DE_Pin, GPIO_PIN_SET); /* Display Enable */ /* PG13 LCD_DE      */
    HAL_GPIO_WritePin(GPIOQ, GPIO_PIN_6, GPIO_PIN_SET); /* 100% Brightness */ /* PQ6  LCD_BL_CTRL */

  }
}

HAL_StatusTypeDef MX_LTDC_ClockConfig(void)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hltdc);

  HAL_StatusTypeDef   status =  HAL_OK;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};

  /* LCD clock configuration */
  /* Typical PCLK is 25 MHz so the PLL4 is configured to provide this clock */
  /* LTDC - PLL4 */
  /* Configure LTDC clock to IC16 with PLL4  */

  /* LCD clock configuration */
  /* Typical PCLK is 25 MHz so the PLL4 is configured to provide this clock */
  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLLM = 48 Mhz / 192 = 0.25 */
  /* PLL3_VCO Output = PLL3_VCO Input * PLLN = 0.25 Mhz * 100 = 25 */
  /* PLLLCDCLK = PLL3_VCO Output/(PLLP1 * PLLP2) = 25/1 = 25Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 25 Mhz */

  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  RCC_PeriphCLKInitStruct.LtdcClockSelection = RCC_LTDCCLKSOURCE_IC16;
  RCC_PeriphCLKInitStruct.ICSelection[RCC_IC16].ClockSelection = RCC_ICCLKSOURCE_PLL4;
  RCC_PeriphCLKInitStruct.ICSelection[RCC_IC16].ClockDivider = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    status = HAL_ERROR;
  }

  return status;
}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* ltdcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(ltdcHandle->Instance==LTDC)
  {
#if 1
  /* USER CODE BEGIN LTDC_MspInit 0 */

  /* USER CODE END LTDC_MspInit 0 */

  /** Initializes the peripherals clock
  */
#if 1
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.LtdcClockSelection = RCC_LTDCCLKSOURCE_IC16;
    PeriphClkInitStruct.ICSelection[RCC_IC16].ClockSelection = RCC_ICCLKSOURCE_PLL3;
    PeriphClkInitStruct.ICSelection[RCC_IC16].ClockDivider = 64;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
#else
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.LtdcClockSelection = RCC_LTDCCLKSOURCE_IC16;
    PeriphClkInitStruct.ICSelection[RCC_IC16].ClockSelection = RCC_ICCLKSOURCE_PLL4;
    PeriphClkInitStruct.ICSelection[RCC_IC16].ClockDivider = 2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
#endif

    /* LTDC clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();

    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOQ_CLK_ENABLE();
    /**LTDC GPIO Configuration
    PH3     ------> LTDC_B4
    PH6     ------> LTDC_B5
    PD15    ------> LTDC_R2
    PB14    ------> LTDC_HSYNC
    PB13    ------> LTDC_CLK
    PB2     ------> LTDC_B2
    PB15    ------> LTDC_G4
    PE11    ------> LTDC_VSYNC
    PD8     ------> LTDC_R7
    PH4     ------> LTDC_R4
    PD9     ------> LTDC_R1
    PG6     ------> LTDC_B3
    PA1     ------> LTDC_G2
    PB11    ------> LTDC_G6
    PA15(JTDI)     ------> LTDC_R5
    PG15    ------> LTDC_B0
    PG1     ------> LTDC_G1
    PB12    ------> LTDC_G5
    PA7     ------> LTDC_B1
    PG0     ------> LTDC_R0
    PA2     ------> LTDC_B7
    PG12    ------> LTDC_G0
    PB4(NJTRST)     ------> LTDC_R3
    PG8     ------> LTDC_G7
    PA8     ------> LTDC_B6
    PG13    ------> LTDC_DE
    PA0     ------> LTDC_G3
    PG11    ------> LTDC_R6
    */
    GPIO_InitStruct.Pin = LCD_B4_Pin|LCD_B5_Pin|LCD_R4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_R2_Pin|LCD_R7_Pin|LCD_R1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_HSYNC_Pin|GPIO_PIN_13|LCD_B2_Pin|LCD_G4_Pin
                          |LCD_G6_Pin|LCD_G5_Pin|LCD_R3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_VSYNC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(LCD_VSYNC_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_B3_Pin|LCD_B0_Pin|LCD_G1_Pin|LCD_R0_Pin
                          |LCD_G0_Pin|LCd_G7_Pin|LCD_DE_Pin|LCD_R6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_G2_Pin|LCD_R5_Pin|LCD_B1_Pin|LCD_B7_Pin
                          |LCD_B6_Pin|LCD_G3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN LTDC_MspInit 1 */

    HAL_GPIO_WritePin(LCD_ON_GPIO_Port, LCD_ON_Pin, GPIO_PIN_SET); /* LCD On */ /* PQ3  LCD_ONOFF   */
    HAL_GPIO_WritePin(LCD_DE_GPIO_Port, LCD_DE_Pin, GPIO_PIN_SET); /* Display Enable */ /* PG13 LCD_DE      */
    HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET); /* 100% Brightness */ /* PQ6  LCD_BL_CTRL */

    /* USER CODE END LTDC_MspInit 1 */
#else
    __HAL_RCC_LTDC_CLK_ENABLE();

    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();

    /* STM32N6570-DK MB1860A RK050HR18 */
    /* PG0  R0          */
    /* PD9  R1          */
    /* PD15 R2          */
    /* PB4  R3          */ // JTAG
    /* PH4  R4          */
    /* PA15 R5          */ // JTAG JTDI
    /* PG11 R6          */
    /* PD8  R7          */
    /* PG12 G0          */
    /* PG1  G1          */
    /* PA1  G2          */
    /* PA0  G3          */
    /* PB15 G4          */
    /* PB12 G5          */
    /* PB11 G6          */
    /* PG8  G7          */
    /* P15  B0          */
    /* PA7  B1          */
    /* PB2  B2          */
    /* PG6  B3          */
    /* PH3  B4          */
    /* PH6  B5          */
    /* PA8  B6          */
    /* PA2  B7          */
    /*                  */
    /* PG13 LCD_DE      */
    /* PQ3  LCD_ONOFF   */
    /* PB14 LCD_HSYNC   */
    /* PE11 PCD_VSYNC   */
    /* PB13 LCD_CLK     */
    /* PQ4  LCD_INT     */
    /* PQ6  LCD_BL_CTRL */
    /* PE1  NRST        */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOQ_CLK_ENABLE();

    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

    /* G3, G2, B7, B1, B6, R5 */
    GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_15;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* LCD_CLK, LCD_HSYNC B2, R3, G6, G5, G4 */
    GPIO_InitStruct.Pin       = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* R7, R1, R2 */
    GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_15;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* LCD_VSYNC */
    GPIO_InitStruct.Pin       = GPIO_PIN_11;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* R0, G1, B3, G7, R6, G0 */
    GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12 ;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* B4, R4, B5 */
    GPIO_InitStruct.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /* NRST */
    GPIO_InitStruct.Pin       = GPIO_PIN_1;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* LCD_ONOFF, LCD_BL_CTRL */
    GPIO_InitStruct.Pin       = GPIO_PIN_3 | GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOQ, &GPIO_InitStruct);

    /* LCD_DE */
    GPIO_InitStruct.Pin       = GPIO_PIN_13;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOQ, GPIO_PIN_3, GPIO_PIN_SET); /* LCD On */ /* PQ3  LCD_ONOFF   */
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET); /* Display Enable */ /* PG13 LCD_DE      */
    HAL_GPIO_WritePin(GPIOQ, GPIO_PIN_6, GPIO_PIN_SET); /* 100% Brightness */ /* PQ6  LCD_BL_CTRL */
#endif
  }

}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* ltdcHandle)
{

  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspDeInit 0 */

  /* USER CODE END LTDC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LTDC_CLK_DISABLE();

    /**LTDC GPIO Configuration
    PH3     ------> LTDC_B4
    PH6     ------> LTDC_B5
    PD15     ------> LTDC_R2
    PB14     ------> LTDC_HSYNC
    PB13     ------> LTDC_CLK
    PB2     ------> LTDC_B2
    PB15     ------> LTDC_G4
    PE11     ------> LTDC_VSYNC
    PD8     ------> LTDC_R7
    PH4     ------> LTDC_R4
    PD9     ------> LTDC_R1
    PG6     ------> LTDC_B3
    PA1     ------> LTDC_G2
    PB11     ------> LTDC_G6
    PA15(JTDI)     ------> LTDC_R5
    PG15     ------> LTDC_B0
    PG1     ------> LTDC_G1
    PB12     ------> LTDC_G5
    PA7     ------> LTDC_B1
    PG0     ------> LTDC_R0
    PA2     ------> LTDC_B7
    PG12     ------> LTDC_G0
    PB4(NJTRST)     ------> LTDC_R3
    PG8     ------> LTDC_G7
    PA8     ------> LTDC_B6
    PG13     ------> LTDC_DE
    PA0     ------> LTDC_G3
    PG11     ------> LTDC_R6
    */
    HAL_GPIO_DeInit(GPIOH, LCD_B4_Pin|LCD_B5_Pin|LCD_R4_Pin);

    HAL_GPIO_DeInit(GPIOD, LCD_R2_Pin|LCD_R7_Pin|LCD_R1_Pin);

    HAL_GPIO_DeInit(GPIOB, LCD_HSYNC_Pin|GPIO_PIN_13|LCD_B2_Pin|LCD_G4_Pin
                          |LCD_G6_Pin|LCD_G5_Pin|LCD_R3_Pin);

    HAL_GPIO_DeInit(LCD_VSYNC_GPIO_Port, LCD_VSYNC_Pin);

    HAL_GPIO_DeInit(GPIOG, LCD_B3_Pin|LCD_B0_Pin|LCD_G1_Pin|LCD_R0_Pin
                          |LCD_G0_Pin|LCd_G7_Pin|LCD_DE_Pin|LCD_R6_Pin);

    HAL_GPIO_DeInit(GPIOA, LCD_G2_Pin|LCD_R5_Pin|LCD_B1_Pin|LCD_B7_Pin
                          |LCD_B6_Pin|LCD_G3_Pin);

  /* USER CODE BEGIN LTDC_MspDeInit 1 */

  /* USER CODE END LTDC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
