/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include <stdio.h>
#include "main.h"
#include "crc.h"
#include "app_filex.h"
#include "i2c.h"
#include "ltdc.h"
#include "rng.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#if (DISPLAY_BYTES_PER_PIXEL == 2)
Color_RGB565_t FrameBuffer[DISPLAY_NO_OF_FRAMEBUFFERS][DISPLAY_SIZE_H][DISPLAY_SIZE_W] __attribute__((section("Nemagfx_Framebuffer")));
#elif (DISPLAY_BYTES_PER_PIXEL == 3)
Color_RGB888_t FrameBuffer[DISPLAY_NO_OF_FRAMEBUFFERS][DISPLAY_SIZE_H][DISPLAY_SIZE_W] __attribute__((section("Nemagfx_Framebuffer")));
#elif (DISPLAY_BYTES_PER_PIXEL == 4)
Color_RGBA8888_t FrameBuffer[DISPLAY_NO_OF_FRAMEBUFFERS][DISPLAY_SIZE_H][DISPLAY_SIZE_W] __attribute__((section("Nemagfx_Framebuffer")));
#endif

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void SystemIsolation_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//  MX_I2C1_Init();
//  MX_I2C2_Init();
  MX_LTDC_Init();
//  MX_SDMMC2_SD_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
//  MX_CRC_Init();
//  MX_RNG_Init();
  MX_TIM3_Init();
//  MX_FileX_Init();
  SystemIsolation_Config();
  /* USER CODE BEGIN 2 */

  printf("\nAppl: Hello STM32N6 world! (Appl 005)\n");

#if 0
  for (uint32_t y = 0; y < 2; y++)
  {
    for (uint32_t x = 0; x < 2; x++)
    {
      printf("W");
      FrameBuffer[0][y][x].R = 0xFF;
      printf("w");
      FrameBuffer[0][y][x].G = 0xFF;
      FrameBuffer[0][y][x].B = 0xFF;
//      FrameBuffer[0][y][x].A = 0xFF;
      SCB_CleanDCache();
    }
  }
#endif

#if 1
  printf("----------\n");
  printf("Accessing external memory ");
  uint32_t address = XSPI1_BASE + 400*2;
  *((uint32_t *) address) = 0x12345678;
  uint32_t data = *((uint32_t *) address);
  if (data == 0x12345678)
  {
    printf("PASSED!\n");
  }
  else
  {
    printf("FAILED!\n");
  }
  printf("----------\n");

  *((uint32_t *) 0x90000000 + (800*240+400) * 4) = 0x55;
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  printf("Entering Mainloop\n");
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    HAL_Delay(250);
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    HAL_Delay(750);
  }
  /* USER CODE END 3 */
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_TIM;
  PeriphClkInitStruct.TIMPresSelection = RCC_TIMPRES_DIV1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RIF Initialization Function
  * @param None
  * @retval None
  */
static void SystemIsolation_Config(void)
{

/* USER CODE BEGIN RIF_Init 0 */

/* USER CODE END RIF_Init 0 */

  /* Set all required IPs as secure privileged */
  __HAL_RCC_RIFSC_CLK_ENABLE();
  RIMC_MasterConfig_t RIMC_master = {0};
  RIMC_master.MasterCID = RIF_CID_1;
  RIMC_master.SecPriv = RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV;
  uint32_t SecPriv = GPIO_PIN_SEC | GPIO_PIN_NPRIV;

  /*RIMC configuration*/
  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_DMA2D, &RIMC_master);
  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_ETH1, &RIMC_master);
  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_ETR, &RIMC_master);
  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_GPU2D, &RIMC_master);
  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_LTDC1, &RIMC_master);
  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_LTDC2, &RIMC_master);
  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_SDMMC2, &RIMC_master);

  /*RISUP configuration*/
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_DMA2D , RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV);
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_GPU2D , RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV);
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_I2C1 , RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV);
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_JPEG , RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV);
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_LTDC , RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV);
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_LTDCL1 , RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV);
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_LTDCL2 , RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV);
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_USART1 , RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV);

  /* RIF-Aware IPs Config */

  /* set up GPIO configuration */
  HAL_GPIO_ConfigPinAttributes(GPIOA, GPIO_PIN_0, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOA, GPIO_PIN_1, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOA, GPIO_PIN_2, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOA, GPIO_PIN_7, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOA, GPIO_PIN_8, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOA, GPIO_PIN_11, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOA, GPIO_PIN_15, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOB, GPIO_PIN_0, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOB, GPIO_PIN_2, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOB, GPIO_PIN_4, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOB, GPIO_PIN_6, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOB, GPIO_PIN_7, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOB, GPIO_PIN_11, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOB, GPIO_PIN_12, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOB, GPIO_PIN_13, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOB, GPIO_PIN_14, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOB, GPIO_PIN_15, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_0, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_1, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_2, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_3, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_4, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_5, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_10, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_11, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_12, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_13, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOD, GPIO_PIN_4, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOD, GPIO_PIN_8, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOD, GPIO_PIN_9, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOD, GPIO_PIN_14, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOD, GPIO_PIN_15, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOE, GPIO_PIN_2, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOE, GPIO_PIN_3, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOE, GPIO_PIN_4, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOE, GPIO_PIN_5, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOE, GPIO_PIN_6, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOE, GPIO_PIN_7, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOE, GPIO_PIN_8, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOE, GPIO_PIN_11, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_0, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_1, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_6, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_7, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_8, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_10, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_11, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_12, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_13, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_15, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOH, GPIO_PIN_2, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOH, GPIO_PIN_3, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOH, GPIO_PIN_4, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOH, GPIO_PIN_6, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOH, GPIO_PIN_9, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_0, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_1, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_2, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_3, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_4, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_5, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_6, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_8, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_9, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_10, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_11, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOO, GPIO_PIN_0, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOO, GPIO_PIN_1, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOO, GPIO_PIN_2, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOO, GPIO_PIN_3, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOO, GPIO_PIN_4, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_0, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_1, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_2, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_3, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_4, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_5, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_6, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_7, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_8, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_9, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_10, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_11, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_12, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_13, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_14, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_15, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOQ, GPIO_PIN_3, SecPriv);
  HAL_GPIO_ConfigPinAttributes(GPIOQ, GPIO_PIN_6, SecPriv);

/* USER CODE BEGIN RIF_Init 1 */

/* USER CODE END RIF_Init 1 */
/* USER CODE BEGIN RIF_Init 2 */

/* USER CODE END RIF_Init 2 */

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
