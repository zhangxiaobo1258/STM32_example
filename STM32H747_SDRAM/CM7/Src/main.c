/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
#define HSEM_ID_0 (0U) /* HW semaphore 0*/

#define BUFFER_SIZE                 10
#define SDRAM_WRITE_READ_ADDR       0xD0000000

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t WriteBuffer[BUFFER_SIZE] ={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA};
uint8_t ReadBuffer[BUFFER_SIZE] = {0};
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FMC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */
    /* USER CODE BEGIN Boot_Mode_Sequence_0 */
    int32_t timeout; 
    /* USER CODE END Boot_Mode_Sequence_0 */

    /* USER CODE BEGIN Boot_Mode_Sequence_1 */
    /* Wait until CPU2 boots and enters in stop mode or timeout*/
    timeout = 0xFFFF;
    while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
    if ( timeout < 0 )
    {
        Error_Handler();
    }
    /* USER CODE END Boot_Mode_Sequence_1 */
    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();
    /* USER CODE BEGIN Boot_Mode_Sequence_2 */
    /* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
    HSEM notification */
    /*HW semaphore Clock enable*/
    __HAL_RCC_HSEM_CLK_ENABLE();
    /*Take HSEM */
    HAL_HSEM_FastTake(HSEM_ID_0);
    /*Release HSEM in order to notify the CPU2(CM4)*/
    HAL_HSEM_Release(HSEM_ID_0,0);
    /* wait until CPU2 wakes up from stop mode */
    timeout = 0xFFFF;
    while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
    if ( timeout < 0 )
    {
        Error_Handler();
    }
    /* USER CODE END Boot_Mode_Sequence_2 */

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_FMC_Init();
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_12|GPIO_PIN_13,GPIO_PIN_SET);//LED灯灭
    /* USER CODE BEGIN 2 */
    HAL_SDRAM_Write_32b(&hsdram1,(uint32_t *)SDRAM_WRITE_READ_ADDR,(uint32_t *)WriteBuffer,BUFFER_SIZE);
    HAL_SDRAM_Read_32b(&hsdram1,(uint32_t *)SDRAM_WRITE_READ_ADDR,(uint32_t *)ReadBuffer,BUFFER_SIZE);
    if( memcmp(WriteBuffer,ReadBuffer,BUFFER_SIZE) == 0)//如果读写成功，黄灯和绿灯闪烁
    {
        while(1)
        {
            HAL_GPIO_TogglePin(GPIOI,GPIO_PIN_12|GPIO_PIN_13);
            HAL_Delay(1000);
        }
    }
    else
    {
        HAL_GPIO_TogglePin(GPIOI,GPIO_PIN_12|GPIO_PIN_13);
        while(1);
    }
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
//    while (1)
//    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
 //   }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Supply configuration update enable 
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
  PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

static void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
    __IO uint32_t tmpmrd = 0;
    FMC_SDRAM_CommandTypeDef Command;

    /* Configure a clock configuration enable command */
    Command.CommandMode 		 = FMC_SDRAM_CMD_CLK_ENABLE;
    Command.CommandTarget 		 = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber 	 = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, &Command, 0x1000);

    /* Insert 10 ms delay */
    HAL_Delay(10);

    /* Configure a PALL (precharge all) command */
    Command.CommandMode 		 = FMC_SDRAM_CMD_PALL;
    Command.CommandTarget 	     = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber 	 = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, &Command, 0x1000);

    /* Configure a Auto-Refresh command */
    Command.CommandMode 		 = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    Command.CommandTarget 		 = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber 	 = 8;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, &Command, 0x1000);

    /* Program the external memory mode register */
    tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1|
             SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
             SDRAM_MODEREG_CAS_LATENCY_3           |
             SDRAM_MODEREG_OPERATING_MODE_STANDARD |
             SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    Command.CommandTarget 		 = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber 	 = 1;
    Command.ModeRegisterDefinition = tmpmrd;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, &Command, 0x1000);

    /* Set the refresh rate counter */
    /* (7.81us x Freq) - 20 */
    /* Set the device refresh counter */
    HAL_SDRAM_ProgramRefreshRate(hsdram, 920);
}
/* FMC initialization function */
void MX_FMC_Init(void)
{

    /* USER CODE BEGIN FMC_Init 0 */

    /* USER CODE END FMC_Init 0 */

    FMC_SDRAM_TimingTypeDef SdramTiming = {0};
    //FMC_SDRAM_CommandTypeDef SdramCommand = {0};

    /* USER CODE BEGIN FMC_Init 1 */

    /* USER CODE END FMC_Init 1 */

    /** Perform the SDRAM1 memory initialization sequence
    */
    hsdram1.Instance = FMC_SDRAM_DEVICE;
    /* hsdram1.Init */
    hsdram1.Init.SDBank = FMC_SDRAM_BANK2;                            /* 硬件设计上用的BANAK2*/
    hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;      /* 8列 */
    hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;           /* 12行 */
    hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;        /* 32位带宽 */ 
    hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;   /* SDRAM有4个BANK */
    hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;                /* CAS Latency可以设置Latency1，2和3,设置为3时，IS42S32800G为166MHz，设置为2时，IS42S32800G为100MHz*/
    hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;/* 禁止写保护 */
    hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;            /* FMC时钟200MHz，2分频后给SDRAM，即100MHz */
    hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;                 /* 使能读突发 */
    hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;             /* 此位定CAS延时后延后多少个SDRAM时钟周期读取数据，实际测此位可以设置无需延迟 */
    /* SdramTiming */
    SdramTiming.LoadToActiveDelay = 2;                /* 12ns, TMRD定义加载模式寄存器的命令与激活命令或刷新命令之间的延迟 */
    SdramTiming.ExitSelfRefreshDelay = 7;             /* 70ns, TXSR定义从发出自刷新命令到发出激活命令之间的延迟 */
    SdramTiming.SelfRefreshTime = 5;                  /* 42ns, TRAS定义最短的自刷新周期 */
    SdramTiming.RowCycleDelay = 6;                    /* 60ns, TRC定义刷新命令和激活命令之间的延迟 */
    SdramTiming.WriteRecoveryTime = 2;                /* 12ns, TWR定义在写命令和预充电命令之间的延迟*//*对于IS42S32800G，为TDPL*/
    SdramTiming.RPDelay = 2;                          /* 18ns, TRP定义预充电命令与其它命令之间的延迟*/
    SdramTiming.RCDDelay = 2;                         /* 18ns, TRCD定义激活命令与读/写命令之间的延迟*/

    if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
    {
        Error_Handler( );
    }

    /* USER CODE BEGIN FMC_Init 2 */
    /* 完成SDRAM序列初始化 */
    SDRAM_Initialization_Sequence(&hsdram1);
    /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOI,&GPIO_InitStruct);

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

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
