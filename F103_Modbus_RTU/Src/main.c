/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "math.h"
#include "usercmd.h"
#include "userstruct.h"
#include "string.h"
#include "crc.h"
#include "services.h"
#include "time.h" 
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define DEBUG
//#define TICK_MODBUS

#define OK_FLAG				0xAA
#define NO_FLAG				0x55

#define DATANUMBER          30						//寄存器的数量

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
struct ModbusRegister DataBuffer[DATANUMBER];
uint8_t RxBuffer;
uint8_t ModbusRxData[RXBUFF_LENGTH];
uint8_t ModbusTxData[TXBUFF_LENGTH];
union WordtoBytes CRCResult;

uint8_t ModbusRxBufferStartFlag;
uint8_t ModbusRxBufferEndFlag;
uint16_t ModbusRxDataLength=0;

uint32_t RTU_Time;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim3;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);
void MX_TIM3_Init(void);
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


	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_TIM3_Init();
	/* USER CODE BEGIN 2 */
	for(uint16_t i=0;i< 20 ;i++)
	{
		DataBuffer[i].RegisterAddress.Word = RTU_REG_BASE+i;
		DataBuffer[i].RegisterData.Word = 0x11*i;
	}

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
#ifdef DEBUG 																			///测试Tick的周期
		RTU_Time = 0;
		RTU_Time = HAL_GetTick();			//HAL_GetTick is 1ms
		HAL_Delay(1000);
		RTU_Time = HAL_GetTick()-RTU_Time;
		printf("RTU_time is %d.\n",RTU_Time);
#endif
		//回复MODBUS主机的命令
#ifdef TICK_MODBUS
		RTU_Time = HAL_GetTick()-RTU_Time;												//查询是否超时，如果超时，则认为主机命令发送结束，对主机命令进行回复
		if(RTU_Time >= RTU_TIMEOUT && ModbusRxBufferStartFlag == OK_FLAG  && ModbusRxDataLength >= RXBUFF_MIN )
		{
			CRCResult.Word = CRC16Bit(ModbusRxData,ModbusRxDataLength-2);			//对数据组进行CRC校验
			if(CRCResult.Bytes.LSB == ModbusRxData[ModbusRxDataLength-1] && CRCResult.Bytes.MSB == ModbusRxData[ModbusRxDataLength-2])
			{
				if(ModbusRxData[RTU_BASE_ADD] == LOCAL_BASE_ADD)
				{
					switch(ModbusRxData[RTU_CMD_FUN])
					{
						case RTU_R01_CMD:
							Host01Service();
							break;
						case RTU_R02_CMD:
							Host02Service();
							break;
						case RTU_R03_CMD:
							Host03Service();
							break;
						case RTU_R04_CMD:
							Host04Service();
							break;
						case RTU_W10_CMD:
							Host10Service();
							break;
						default:break;
					}
				}
			}
			memset(ModbusRxData,0x00,RXBUFF_LENGTH);
			ModbusRxDataLength = 0;
			ModbusRxBufferEndFlag = NO_FLAG;
			ModbusRxBufferStartFlag = NO_FLAG;
		}
#endif
		/* USER CODE END WHILE */
		/* USER CODE BEGIN 3 */
		
	}
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

	/** Initializes the CPU, AHB and APB busses clocks 
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks 
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}
/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM3_Init(void)
{

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;				
	htim3.Init.Prescaler = 36000-1;							//设置定时器时间为5ms，为MODBUS的通信时间服务
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 10;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM4_Init 2 */

	/* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */
	HAL_UART_Receive_IT(&huart2,&RxBuffer,1);
	/* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
#ifndef TICK_MODBUS
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{	
	if(htim->Instance == TIM3)
	{
		if( ModbusRxBufferStartFlag == OK_FLAG  && ModbusRxDataLength >= RXBUFF_MIN )
		{
			CRCResult.Word = CRC16Bit(ModbusRxData,ModbusRxDataLength-2);			//对数据组进行CRC校验
			if(CRCResult.Bytes.LSB == ModbusRxData[ModbusRxDataLength-1] && CRCResult.Bytes.MSB == ModbusRxData[ModbusRxDataLength-2])
			{
				if(ModbusRxData[RTU_BASE_ADD] == LOCAL_BASE_ADD)
				{
					switch(ModbusRxData[RTU_CMD_FUN])
					{
						case RTU_R01_CMD:
							Host01Service();
							break;
						case RTU_R02_CMD:
							Host02Service();
							break;
						case RTU_R03_CMD:
							Host03Service();
							break;
						case RTU_R04_CMD:
							Host04Service();
							break;
						case RTU_W10_CMD:
							Host10Service();
							break;
						default:break;
					}
				}
			}
			memset(ModbusRxData,0x00,RXBUFF_LENGTH);
			ModbusRxDataLength = 0;
			ModbusRxBufferEndFlag = NO_FLAG;
			ModbusRxBufferStartFlag = NO_FLAG;
		}
		HAL_TIM_Base_Stop_IT(&htim3);
	}
}
#endif
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		HAL_TIM_Base_Stop_IT(&htim3);
		if(ModbusRxBufferStartFlag != OK_FLAG)
		{
			ModbusRxBufferStartFlag = OK_FLAG;
		}
		ModbusRxData[ModbusRxDataLength] = RxBuffer; 		
		ModbusRxDataLength ++;
		HAL_UART_Receive_IT(&huart2,&RxBuffer,1);
		HAL_TIM_Base_Start_IT(&htim3);
#ifdef  TICK_MODBUS
		RTU_Time = HAL_GetTick();														//接收数据后将RTU_Time值清零
#endif
	}
}

#ifdef DEBUG
int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart2,(uint8_t *)&ch,1,0xFFFFFFFF);
	return ch;
}
#endif
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
