/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "OLED_I2C.h"
#include "FFT.h"
#include "stdio.h"
#include "string.h"
#include "myusart1.h"
#include "AD9959.h"
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

//extern变量
extern uint32_t FRE;
extern uint32_t Fn;
extern uint16_t cnt;
extern uint16_t vmax;
extern uint16_t vmin;
extern uint16_t amp;
extern uint16_t line_idex;
extern float fft_outputbuf;

//输入捕获变量
uint32_t FQ;//频率 			
uint32_t CNT_1 = 0;//tim4 	
uint32_t CNT_2 = 0;//tim4计时计满一个(2^32-1)次 

//串口变量	
uint8_t data_rx[7] = {0};
uint8_t rx_byte = 0; 

//标志变量
uint8_t capture_cnt=0;
uint8_t adc1_flag=1;
uint8_t capture_flag=1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_IncTick(void)
{
	if(uwTickFreq==0)
	{
		uwTickFreq = HAL_TICK_FREQ_DEFAULT;
	}
  uwTick += uwTickFreq;
};

void FPGA_Write_Reg(uint8_t addr, uint8_t data)
{
    uint16_t tx_word = (0x1 << 13) | ((addr & 0x1F) << 8) | data; 
    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_RESET); 
    HAL_SPI_Transmit(&hspi2, (uint8_t*)&tx_word, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);  
    HAL_Delay(1); 
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  delay_init(170);
  MyUSART1_Init(&huart1);
  OLED_Init();
  OLED_CLS();
  HAL_UART_Receive_IT(&huart1, data_rx, 1);
  					
	AD9959_Set_Fre(CH0, 100000);	
	AD9959_Set_Fre(CH1, 200000);	
	AD9959_Set_Fre(CH2, 100000);	
	AD9959_Set_Fre(CH3, 100000);	
		
	AD9959_Set_Amp(CH0, 1023); 	
	AD9959_Set_Amp(CH1, 1023); 	
	AD9959_Set_Amp(CH3, 1023); 		

	AD9959_Set_Phase(CH0, 0);		
	AD9959_Set_Phase(CH1, 4096);
	AD9959_Set_Phase(CH2, 8192);
	AD9959_Set_Phase(CH3, 12288);	
	IO_Update();	
  
  printf("main\xff\xff\xff");
  delay_ms(1000);
  printf("main\xff\xff\xff");
  FRE=1024000;
  ADC1_Get(FRE);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {  
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(adc1_flag==0)
		{
			HAL_ADC_Stop_DMA(&hadc1);
			HAL_TIM_Base_Stop(&htim3);
			capture_flag=1;
			HAL_TIM_Base_Start_IT(&htim4);
			HAL_TIM_Base_Start_IT(&htim2);
			HAL_Delay(1);
			adc1_flag=1;
		}
		if (capture_flag==0) 
		{
			HAL_TIM_Base_Stop_IT(&htim4); 
		   HAL_TIM_Base_Stop_IT(&htim2); 
			FRE=1024000;
			HAL_Delay(10);
			ADC1_Get(FRE);
		}
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc == &hadc1)
	{
		fft_spectrogram_judge();
		adc1_flag=0;
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        for (int i = 0; i < 6; i++) 
		  {
            data_rx[i] = data_rx[i + 1];
        }
        data_rx[6] = rx_byte;

        if (data_rx[5] == 0x0D && data_rx[6] == 0x0A)
        {
            memset(data_rx, 0, sizeof(data_rx));
        }
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim4)) 
    {
        CNT_1 = __HAL_TIM_GET_COUNTER(&htim2); 							
        FQ = CNT_1 + CNT_2 * (__HAL_TIM_GET_AUTORELOAD(&htim2) + 1); 
        __HAL_TIM_SET_COUNTER(&htim2, 0);// 设置CNT为0
			if(capture_cnt!=0)
			{
				OLED_ShowInt(10,5,FQ,2);
			}
			
			capture_cnt=1;
			CNT_1 = 0;  
			CNT_2 = 0; 
			capture_flag=0;
    }
    if (htim == (&htim2)) // 计数值计满，产生更新中断
    {
        CNT_2++; // 记满后，+1，标志多了一个满量程数
    }
}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
