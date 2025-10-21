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
#include <extmem.h>
#include <stdio.h>
#include <stm32n6xx_hal_rif.h>
#include <string.h>
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "extmem_manager.h"
#include "app_filex.h"
#include "i2c.h"
#include "icache.h"
#include "mdf.h"
#include "rng.h"
#include "sai.h"
#include "sdmmc.h"
#include "tim.h"
#include "ucpd.h"
#include "usart.h"
#include "usb_otg.h"
#include "xspi.h"
#include "xspim.h"
#include "gpio.h"
#include "ltdc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NO_OTP_FUSE

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
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
static void SystemIsolation_Config(void);
static int32_t OTP_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if 1
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
#endif

/* USER CODE END 0 */

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  SCB_InvalidateDCache();
  SCB_InvalidateICache();

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable the CPU Cache */

  /* Enable I-Cache-----------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache-----------------------------------------------------------*/
//  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */
//  OTP_Config();

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//  MX_ADC1_Init();
//  MX_I2C1_Init();
//  MX_I2C2_Init();
  MX_ICACHE_Init();
//  MX_MDF1_Init();
//  MX_SAI1_Init();
//  MX_SDMMC2_SD_Init();
//  MX_TIM2_Init();
//  MX_UCPD1_Init();

  SystemIsolation_Config();

  MX_LTDC_ClockConfig();
  MX_LTDC_Init();
//  LTDC_MspInit();

  HAL_GPIO_WritePin(LCD_DE_GPIO_Port, LCD_DE_Pin, GPIO_PIN_SET); /* PG13 LCD_DE */
  HAL_GPIO_WritePin(LCD_ON_GPIO_Port, LCD_ON_Pin, GPIO_PIN_SET); /* PQ3  LCD_ON  */
  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET); /* PQ6  LCD_BL  */

  MX_USART1_UART_Init();

  printf("\nFSBL: Hello STM32N6 world! (FSBL 005)\n");

//  MX_USB1_OTG_HS_USB_Init();
//  MX_USB2_OTG_HS_HCD_Init();
  MX_XSPI1_Init();
  MX_XSPI2_Init();
//  MX_CRC_Init();
//  MX_RNG_Init();
//  MX_FileX_Init();


#if 1
  printf("Configure_APMemory\n");
  Map_APMemory();
#endif

#if 1
  printf("Clear framebuffers\n");
  for (uint8_t i = 0; i < DISPLAY_NO_OF_FRAMEBUFFERS; i++)
  {
    memset(FrameBuffer[i], 0x55, sizeof(FrameBuffer[i]));
  }
  printf("Done\n");
#endif

#if 0
  RIMC_MasterConfig_t RIMC_master = {0};
  RIMC_master.SecPriv = RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV;
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_USART1, RIMC_master.SecPriv);
#endif

  /* USER CODE BEGIN 2 */

#if 1
  printf("Writing framebuffer\n");
  for (uint32_t y = 0; y < 48; y++)
  {
    for (uint32_t x = 0; x < 80; x++)
    {
//      printf("W");
      FrameBuffer[0][y][x].R = 0x80;
//      printf("w");
      FrameBuffer[0][y][x].G = 0x20;
      FrameBuffer[0][y][x].B = 0x40;
//      FrameBuffer[0][y][x].A = 0xFF;
//      SCB_CleanDCache();
    }
  }
#endif

#if 1
  printf("----------\n");
  printf("Accessing external memory ");
  *((uint32_t *) XSPI1_BASE) = 0x12345678;
  uint32_t data = *((uint32_t *) XSPI1_BASE);
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

#if 1
  printf("Entering FSBL Mainloop\n");
  while (1)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    HAL_Delay(400);
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    HAL_Delay(400);
  }
#endif

  /* Initialize the serial memory */
  printf("Initialize the serial memory\n");
  MX_EXTMEM_Init(); /* Makes jump possible */
//  MX_EXTMEM_MANAGER_Init();

  /* USER CODE END 2 */

  /* Launch the application */
  printf("BOOT_Application\n");
  if (BOOT_OK != BOOT_Application())
  {
    Error_Handler();
  }
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
/* USER CODE BEGIN CLK 1 */
/* USER CODE END CLK 1 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
#define SYSTEM_CLOCK_TOUCHGFX    1

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the System Power Supply
  */
  if (HAL_PWREx_ConfigSupply(PWR_EXTERNAL_SOURCE_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable HSI */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Wait HSE stabilization time before its selection as PLL source. */
  HAL_Delay(HSE_STARTUP_TIMEOUT);

  /** Get current CPU/System buses clocks configuration and if necessary switch
 to intermediate HSI clock to ensure target clock can be set
  */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct);
  if ((RCC_ClkInitStruct.CPUCLKSource == RCC_CPUCLKSOURCE_IC1) ||
     (RCC_ClkInitStruct.SYSCLKSource == RCC_SYSCLKSOURCE_IC2_IC6_IC11))
  {
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK);
    RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_HSI;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
#if (SYSTEM_CLOCK_TOUCHGFX == 0)
  /* Non-TouchGFX */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLM = 4;
  RCC_OscInitStruct.PLL1.PLLN = 100;
  RCC_OscInitStruct.PLL1.PLLFractional = 0;
  RCC_OscInitStruct.PLL1.PLLP1 = 1;
  RCC_OscInitStruct.PLL1.PLLP2 = 1;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL3.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL3.PLLM = 1;
  RCC_OscInitStruct.PLL3.PLLN = 25;
  RCC_OscInitStruct.PLL3.PLLFractional = 0;
  RCC_OscInitStruct.PLL3.PLLP1 = 1;
  RCC_OscInitStruct.PLL3.PLLP2 = 1;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL4.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL4.PLLM = 3;
  RCC_OscInitStruct.PLL4.PLLN = 250;
  RCC_OscInitStruct.PLL4.PLLFractional = 0;
  RCC_OscInitStruct.PLL4.PLLP1 = 1;
  RCC_OscInitStruct.PLL4.PLLP2 = 1;
#else
  /* TouchGFX */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL1.PLLM = 6;
  RCC_OscInitStruct.PLL1.PLLN = 100;
  RCC_OscInitStruct.PLL1.PLLFractional = 0;
  RCC_OscInitStruct.PLL1.PLLP1 = 1;
  RCC_OscInitStruct.PLL1.PLLP2 = 1;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL2.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL2.PLLM = 4;
  RCC_OscInitStruct.PLL2.PLLN = 100;
  RCC_OscInitStruct.PLL2.PLLFractional = 0;
  RCC_OscInitStruct.PLL2.PLLP1 = 2;
  RCC_OscInitStruct.PLL2.PLLP2 = 1;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL3.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL3.PLLM = 6;
  RCC_OscInitStruct.PLL3.PLLN = 225;
  RCC_OscInitStruct.PLL3.PLLFractional = 0;
  RCC_OscInitStruct.PLL3.PLLP1 = 2;
  RCC_OscInitStruct.PLL3.PLLP2 = 1;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL4.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL4.PLLM = 6;
  RCC_OscInitStruct.PLL4.PLLN = 250;
  RCC_OscInitStruct.PLL4.PLLFractional = 0;
  RCC_OscInitStruct.PLL4.PLLP1 = 2;
  RCC_OscInitStruct.PLL4.PLLP2 = 1;
#endif

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_CPUCLK|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2|RCC_CLOCKTYPE_PCLK5
                              |RCC_CLOCKTYPE_PCLK4;
  RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_IC1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_IC2_IC6_IC11;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV1;
#if (SYSTEM_CLOCK_TOUCHGFX == 0)
  /* Non-TouchGFX */
  RCC_ClkInitStruct.IC1Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC1Selection.ClockDivider = 2;
  RCC_ClkInitStruct.IC2Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC2Selection.ClockDivider = 4;
  RCC_ClkInitStruct.IC6Selection.ClockSelection = RCC_ICCLKSOURCE_PLL4;
  RCC_ClkInitStruct.IC6Selection.ClockDivider = 2;
  RCC_ClkInitStruct.IC11Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC11Selection.ClockDivider = 2;
#else
  /* TouchGFX */
  RCC_ClkInitStruct.IC1Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC1Selection.ClockDivider = 1;
  RCC_ClkInitStruct.IC2Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC2Selection.ClockDivider = 2;
  RCC_ClkInitStruct.IC6Selection.ClockSelection = RCC_ICCLKSOURCE_PLL4;
  RCC_ClkInitStruct.IC6Selection.ClockDivider = 1;
  RCC_ClkInitStruct.IC11Selection.ClockSelection = RCC_ICCLKSOURCE_PLL3;
  RCC_ClkInitStruct.IC11Selection.ClockDivider = 1;
#endif

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
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

  /* Update the RIF config for the used peripherals */

  /* Set all required IPs as secure privileged */
  __HAL_RCC_RIFSC_CLK_ENABLE();

  RIMC_MasterConfig_t RIMC_master = {0};
  RIMC_master.MasterCID = RIF_CID_1;
  RIMC_master.SecPriv = RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_PRIV;
  uint32_t SecPriv = GPIO_PIN_SEC | GPIO_PIN_NPRIV;

  /*RIMC configuration*/
//  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_DMA2D, &RIMC_master);
//  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_ETH1, &RIMC_master);
//  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_ETR, &RIMC_master);
//  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_GPU2D, &RIMC_master);
  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_LTDC1, &RIMC_master);
  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_LTDC2, &RIMC_master);
//  HAL_RIF_RIMC_ConfigMasterAttributes(RIF_MASTER_INDEX_SDMMC2, &RIMC_master);

  /*RISUP configuration*/
//  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_I2C1, RIMC_master.SecPriv);
//  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_DMA2D, RIMC_master.SecPriv);
//  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_GPU2D, RIMC_master.SecPriv);
//  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_JPEG, RIMC_master.SecPriv);
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_LTDC, RIMC_master.SecPriv);
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_LTDCL1, RIMC_master.SecPriv);
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_LTDCL2, RIMC_master.SecPriv);
//  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_USART1, RIMC_master.SecPriv);

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

/**
  * @brief  User OTP fuse Configuration
  *         The User Option Bytes are configured as follows :
  *            VDDIO_HSLV = 1 (enable the configuration of pads below 2.5V,
  *                            I/O speed otpmization at low-voltage allowed)
  *            XSPI1_HSLV = 1 (enable I/O XSPIM Port 1 high-speed option)
  *            XSPI2_HSLV = 1 (enable I/O XSPIM Port 2 high-speed option)
  *            Other User Option Bytes remain unchanged
  * @retval None
  */
static int32_t OTP_Config(void)
{
  #define BSEC_HW_CONFIG_ID        124U
  #define BSEC_HWS_HSLV_VDDIO3     (1U<<15)
  #define BSEC_HWS_HSLV_VDDIO2     (1U<<16)
  
  int32_t retr = 0;

#ifndef NO_OTP_FUSE
  uint32_t fuse_id, bit_mask, data;
  BSEC_HandleTypeDef sBsecHandler;

  /* Enable BSEC & SYSCFG clocks to ensure BSEC data accesses */
  __HAL_RCC_BSEC_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  sBsecHandler.Instance = BSEC;

  /* Read current value of fuse */
  fuse_id = BSEC_HW_CONFIG_ID;
  if (HAL_BSEC_OTP_Read(&sBsecHandler, fuse_id, &data) == HAL_OK)
  {
    /* Check if bit has already been set */
    bit_mask = BSEC_HWS_HSLV_VDDIO3 | BSEC_HWS_HSLV_VDDIO2;
    if ((data & bit_mask) != bit_mask)
    {
      data |= bit_mask;
      /* Bitwise programming of lower bits */
      if (HAL_BSEC_OTP_Program(&sBsecHandler, fuse_id, data, HAL_BSEC_NORMAL_PROG) == HAL_OK)
      {
        /* Read lower bits to verify the correct programming */
        if (HAL_BSEC_OTP_Read(&sBsecHandler, fuse_id, &data) == HAL_OK)
        {
          if ((data & bit_mask) != bit_mask)
          {
            /* Error : Fuse programming not taken in account */
            retr = -1;
          }
        }
        else
        {
          /* Error : Fuse read unsuccessful */
          retr = -2;
        }
      }
      else
      {
        /* Error : Fuse programming unsuccessful */
        retr = -3;
      }
    }
  }
  else
  {
    /* Error  : Fuse read unsuccessful */
    retr = -4;
  }
#endif /* NO_OTP_FUSE */

  return retr;
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

static void MPU_Config(void)
{
  /*
   * XSPI2 0x70000000 128MB FLASH (1-Gbit   Octo-SPI)
   * XSPI1 0x90000000  32MB PSRAM (256-Mbit Hexadeca-SPI)

-   */
  MPU_Region_InitTypeDef default_config = {0};
  MPU_Attributes_InitTypeDef attr_config = {0};
  uint32_t primask_bit = __get_PRIMASK();
  __disable_irq();

  /* disable the MPU */
  HAL_MPU_Disable();

  /* create an attribute configuration for the MPU */
  attr_config.Attributes = INNER_OUTER(MPU_WRITE_BACK | MPU_NON_TRANSIENT | MPU_RW_ALLOCATE);
  attr_config.Number = MPU_ATTRIBUTES_NUMBER0;

  HAL_MPU_ConfigMemoryAttributes(&attr_config);

  /* Create a region associated with memory address 0x34000000 */
  /*Normal memory type, code execution allowed */
  default_config.Number = MPU_REGION_NUMBER0;
  default_config.Enable = MPU_REGION_ENABLE;
  default_config.BaseAddress = 0x34000000; /* RAM 1756*kB */
  default_config.LimitAddress = 0x34000000 + 0x001B7000-1;
  default_config.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  default_config.AccessPermission = MPU_REGION_ALL_RW;
  default_config.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  default_config.AttributesIndex = MPU_ATTRIBUTES_NUMBER0;
  HAL_MPU_ConfigRegion(&default_config);

  /* Create a region associated with memory address 0x70000000 */
  /*Normal memory type, code execution allowed */
  default_config.Number = MPU_REGION_NUMBER1;
  default_config.Enable = MPU_REGION_ENABLE;
  default_config.BaseAddress = XSPI2_BASE; /* FLASH 128MB 0x70000000 */
  default_config.LimitAddress = XSPI2_BASE + 0x08000000-1;
  default_config.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  default_config.AccessPermission = MPU_REGION_ALL_RO;
  default_config.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  default_config.AttributesIndex = MPU_ATTRIBUTES_NUMBER0;
  HAL_MPU_ConfigRegion(&default_config);

  /* Ensure the FSBL enables MPU access for the PSRAM region before mapping it: */
  default_config.Number = MPU_REGION_NUMBER2;
  default_config.Enable = MPU_REGION_ENABLE;
  default_config.BaseAddress = XSPI1_BASE; /* PSRAM 32MB 0x90000000 */
  default_config.LimitAddress = XSPI1_BASE + 0x02000000-1;
  default_config.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  default_config.AccessPermission = MPU_REGION_ALL_RW;
  default_config.IsShareable = MPU_ACCESS_INNER_SHAREABLE | MPU_ACCESS_OUTER_SHAREABLE;
  default_config.AttributesIndex = MPU_ATTRIBUTES_NUMBER0;
  HAL_MPU_ConfigRegion(&default_config);

  /* enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

  /* Exit critical section to lock the system and avoid any issue around MPU mechanisme */
  __set_PRIMASK(primask_bit);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
//  __disable_irq();
  while(1)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
//    for (volatile uint32_t i = 0; i < 1000000; i++);
    HAL_Delay(200);
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

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */
