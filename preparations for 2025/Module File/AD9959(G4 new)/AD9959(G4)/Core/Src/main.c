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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "myusart1.h"
#include "OLED_I2C.h"
#include "AD9959.H"
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

uint8_t mode_change_flag=0;
uint8_t mode_cnt=0;

uint32_t FSK_fre_ch0[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint32_t FSK_fre_ch1[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint32_t FSK_fre_ch2[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint32_t FSK_fre_ch3[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//FSK锟斤拷锟斤拷

uint16_t ASK_amp_ch0[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t ASK_amp_ch1[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t ASK_amp_ch2[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t ASK_amp_ch3[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//ASK锟斤拷锟斤拷

uint16_t PSK_phase_ch0[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t PSK_phase_ch1[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t PSK_phase_ch2[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t PSK_phase_ch3[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//PSK锟斤拷锟斤拷
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_IncTick(void)
{
	if(uwTickFreq==0)
	{
		uwTickFreq = HAL_TICK_FREQ_DEFAULT;
	}
  uwTick += uwTickFreq;
};
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  delay_init(170);
  MyUSART1_Init(&huart1);
  OLED_Init();
  OLED_CLS();
  OLED_ShowStr(20,3,(unsigned char *)"Single Tone",2);


//模式一 singel tone mode(不涉及到拓展io口)
//控幅，控相，控频

	//AD9959_Init();						//初始化控制AD9959需要用到的IO口,及寄存器
	AD9959_Set_Fre(CH0, 100000);	//设置通道0频率100000Hz
	AD9959_Set_Fre(CH1, 200000);	//设置通道1频率100000Hz
	AD9959_Set_Fre(CH2, 100000);	//设置通道2频率100000Hz
	AD9959_Set_Fre(CH3, 100000);	//设置通道3频率100000Hz
		
	AD9959_Set_Amp(CH0, 1023); 		//设置通道0幅度控制值1023，范围0~1023
	AD9959_Set_Amp(CH1, 1023); 		//设置通道1幅度控制值1023，范围0~1023
	AD9959_Set_Amp(CH2, 1023); 		//设置通道2幅度控制值1023，范围0~1023
	AD9959_Set_Amp(CH3, 1023); 		//设置通道3幅度控制值1023，范围0~1023

	AD9959_Set_Phase(CH0, 0);			//设置通道0相位控制值0(0度)，范围0~16383
	AD9959_Set_Phase(CH1, 4096);	//设置通道1相位控制值4096(90度)，范围0~16383
	AD9959_Set_Phase(CH2, 8192);	//设置通道2相位控制值8192(180度)，范围0~16383
	AD9959_Set_Phase(CH3, 12288);	//设置通道3相位控制值12288(270度)，范围0~16383 
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！                      
 



//模式二 modulation mode(涉及到拓展io口)
//2FSK调制
/*	
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,FSK,SWEEP_DISABLE,LEVEL_MOD_2);//通道0-3设置为FSK模式，不启用线性扫描，2电平调制
	FSK_fre_ch0[0]=1000;//FSK频率1：1000Hz
	FSK_fre_ch0[1]=200000;//FSK频率2：200000Hz
	AD9959_SetFSK(CH0,FSK_fre_ch0,0);//设置FKS参数，CH0通道，F1=1000Hz,F2=200000Hz，相位0(0度)
	
	FSK_fre_ch1[0]=1000;//FSK频率1：1000Hz
	FSK_fre_ch1[1]=200000;//FSK频率2：200000Hz
	AD9959_SetFSK(CH1,FSK_fre_ch1,0);//设置FKS参数，CH1通道，F1=1000Hz,F2=200000Hz，相位0(0度)
	
	FSK_fre_ch2[0]=1000;//FSK频率1：1000Hz
	FSK_fre_ch2[1]=200000;//FSK频率2：200000Hz
	AD9959_SetFSK(CH2,FSK_fre_ch2,0);//设置FKS参数，CH2通道，F1=1000Hz,F2=200000Hz，相位0(0度)
	
	FSK_fre_ch3[0]=1000;//FSK频率1：1000Hz
	FSK_fre_ch3[1]=200000;//FSK频率2：200000Hz
	AD9959_SetFSK(CH3,FSK_fre_ch3,0);//设置FKS参数，CH3通道，F1=1000Hz,F2=200000Hz，相位0(0度)        
	
	//每个通道的FSK参数可以不同，单独设置即可。
	//也可不将所有通道都设置为FSK调制模式，则未设置FSK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！                                 
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3分别控制CH0-CH3；低电平：输出频率1
		HAL_Delay(10);
		PS0_1;PS1_1;PS2_1;PS3_1;//高电平：输出频率2
		HAL_Delay(10);  
	}  
*/

 
//4FSK调制
/*
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	
//	最多只能同时有2个通道，设置成4电平调制。
//	本函数只能选择CH0-1，未做任意通道兼容，具体方法请参考AD9959芯片手册22-23页，操作FR1[14:12]为对应值。
	AD9959_Modulation_Init(CH0|CH1,FSK,SWEEP_DISABLE,LEVEL_MOD_4);//通道0-1设置为FSK模式，不启用线性扫描，4电平调制
	
	FSK_fre_ch0[0]=100;//FSK频率1：100Hz
	FSK_fre_ch0[1]=1000;//FSK频率2：1000Hz
	FSK_fre_ch0[2]=10000;//FSK频率3：10000Hz
	FSK_fre_ch0[3]=100000;//FSK频率4：100000Hz
	AD9959_SetFSK(CH0,FSK_fre_ch0,0);//设置FKS参数，CH0通道，F1=100Hz,F2=1000Hz，F3=10000Hz,F4=100000Hz，相位0(0度)
	
	FSK_fre_ch1[0]=100;//FSK频率1：100Hz
	FSK_fre_ch1[1]=1000;//FSK频率2：1000Hz
	FSK_fre_ch1[2]=10000;//FSK频率3：10000Hz
	FSK_fre_ch1[3]=100000;//FSK频率4：100000Hz
	AD9959_SetFSK(CH1,FSK_fre_ch1,0);//设置FKS参数，CH1通道，F1=100Hz,F2=1000Hz，F3=10000Hz,F4=100000Hz，相位0(0度)
	
	//每个通道的FSK参数可以不同，单独设置即可。
	//也可不将所有通道都设置为FSK调制模式，则未设置FSK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		
		PS0_0;PS1_0;//PS0、PS1分别控制CH0；CH0输出频率1
		PS2_0;PS3_0;//PS2、PS3分别控制CH1；CH1输出频率1
		HAL_Delay(10);
		
		PS0_0;PS1_1;//CH0输出频率2
		PS2_0;PS3_1;//CH1输出频率2
		HAL_Delay(10);
		
		PS0_1;PS1_0;//CH0输出频率3
		PS2_1;PS3_0;//CH1输出频率3
		HAL_Delay(10);
		
		PS0_1;PS1_1;//CH0输出频率4
		PS2_1;PS3_1;//CH1输出频率4
		HAL_Delay(10);
	}
*/



//8FSK调制
/*	
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	
//	最多只能同时有1个通道，设置成8电平调制。
//	本函数只能选择CH0做8电平调制，未做任意通道兼容，具体方法请参考AD9959芯片手册22-23页，操作FR1[14:12]为对应值。
	AD9959_Modulation_Init(CH0,FSK,SWEEP_DISABLE,LEVEL_MOD_8);//通道0设置为FSK模式，不启用线性扫描，8电平调制
	
	FSK_fre_ch0[0]=100;//FSK频率1：100Hz
	FSK_fre_ch0[1]=500;//FSK频率2：500Hz
	FSK_fre_ch0[2]=1000;//FSK频率3：1000Hz
	FSK_fre_ch0[3]=1500;//FSK频率4：1500Hz
	FSK_fre_ch0[4]=2000;//FSK频率5：2000Hz
	FSK_fre_ch0[5]=2500;//FSK频率6：2500Hz
	FSK_fre_ch0[6]=3000;//FSK频率7：3000Hz
	FSK_fre_ch0[7]=3500;//FSK频率8：3500Hz
	AD9959_SetFSK(CH0,FSK_fre_ch0,0);//设置FKS参数，CH0通道，F1=100Hz,F2=500Hz，F3=1000Hz,F4=1500Hz.........，相位0(0度)

	//每个通道的FSK参数可以不同，单独设置即可。
	//也可不将所有通道都设置为FSK调制模式，则未设置FSK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;//PS0-PS2控制CH0；CH0输出频率1
		HAL_Delay(10);
		
		PS0_0;PS1_0;PS2_1;//CH0输出频率2
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_0;//CH0输出频率3
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_1;//CH0输出频率4
		HAL_Delay(10);		
		
		PS0_1;PS1_0;PS2_0;//CH0输出频率5
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_1;//CH0输出频率6
		HAL_Delay(10);
	
		PS0_1;PS1_1;PS2_0;//CH0输出频率7
		HAL_Delay(10);
		
		PS0_1;PS1_1;PS2_1;//CH0输出频率8
		HAL_Delay(10);
	}               
*/

//16FSK调制
/*
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	
//	最多只能同时有1个通道，设置成16电平调制。
//	本函数只能选择CH0做16电平调制，未做任意通道兼容，具体方法请参考AD9959芯片手册22-23页，操作FR1[14:12]为对应值。
	AD9959_Modulation_Init(CH0,FSK,SWEEP_DISABLE,LEVEL_MOD_16);//通道0设置为FSK模式，不启用线性扫描，16电平调制
	
	FSK_fre_ch0[0]=10000;//FSK频率1：10000Hz
	FSK_fre_ch0[1]=20000;//FSK频率2：20000Hz
	FSK_fre_ch0[2]=50000;//FSK频率3：50000Hz
	FSK_fre_ch0[3]=80000;//FSK频率4：80000Hz
	FSK_fre_ch0[4]=120000;//FSK频率5：120000Hz
	FSK_fre_ch0[5]=140000;//FSK频率6：140000Hz
	FSK_fre_ch0[6]=180000;//FSK频率7：180000Hz
	FSK_fre_ch0[7]=200000;//FSK频率8：200000Hz
	
	FSK_fre_ch0[8]=250000;//FSK频率9：...
	FSK_fre_ch0[9]=270000;//FSK频率10：...
	FSK_fre_ch0[10]=300000;//FSK频率11：...
	FSK_fre_ch0[11]=320000;//FSK频率12：...
	FSK_fre_ch0[12]=350000;//FSK频率13：...
	FSK_fre_ch0[13]=400000;//FSK频率14：...
	FSK_fre_ch0[14]=600000;//FSK频率15：...
	FSK_fre_ch0[15]=10000000;//FSK频率16：...
	
	AD9959_SetFSK(CH0,FSK_fre_ch0,0);//设置FKS参数，CH0通道，F1=10000Hz,F2=20000Hz，F3=50000Hz........，相位0(0度)

	//也可不将所有通道都设置为FSK调制模式，则未设置FSK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3控制CH0；CH0输出频率1
		delay_us(15);
		
		PS0_0;PS1_0;PS2_0;PS3_1;//CH0输出频率2
		delay_us(15);
		
		PS0_0;PS1_0;PS2_1;PS3_0;//CH0输出频率3
		delay_us(15);
		
		PS0_0;PS1_0;PS2_1;PS3_1;//CH0输出频率4
		delay_us(15);		
		
		PS0_0;PS1_1;PS2_0;PS3_0;//CH0输出频率5
		delay_us(15);
		
		PS0_0;PS1_1;PS2_0;PS3_1;//CH0输出频率6
		delay_us(15);
	
		PS0_0;PS1_1;PS2_1;PS3_0;//CH0输出频率7
		delay_us(15);
		
		PS0_0;PS1_1;PS2_1;PS3_1;//CH0输出频率8
		delay_us(15);
		
		PS0_1;PS1_0;PS2_0;PS3_0;//CH0输出频率9
		delay_us(15);
		
		PS0_1;PS1_0;PS2_0;PS3_1;//CH0输出频率10
		delay_us(15);
		
		PS0_1;PS1_0;PS2_1;PS3_0;//CH0输出频率11
		delay_us(15);
		
		PS0_1;PS1_0;PS2_1;PS3_1;//CH0输出频率12
		delay_us(15);		
		
		PS0_1;PS1_1;PS2_0;PS3_0;//CH0输出频率13
		delay_us(15);
		
		PS0_1;PS1_1;PS2_0;PS3_1;//CH0输出频率14
		delay_us(15);
	
		PS0_1;PS1_1;PS2_1;PS3_0;//CH0输出频率15
		delay_us(15);
		
		PS0_1;PS1_1;PS2_1;PS3_1;//CH0输出频率16
		delay_us(15);
	}
*/


//2ASK调制
 /*
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,ASK,SWEEP_DISABLE,LEVEL_MOD_2);//通道0-3设置为ASK模式，不启用线性扫描，2电平调制
	ASK_amp_ch0[0]=500;//ASK幅度1：一半,范围（0-1023）
	ASK_amp_ch0[1]=1023;//ASK幅度2：最大
	AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//设置ASK参数，CH0通道，A1=一半,A2=最大值，频率1000hz，相位0(0度)
	
	ASK_amp_ch1[0]=500;//ASK幅度1：一半
	ASK_amp_ch1[1]=1023;//ASK幅度2：最大
	AD9959_SetASK(CH1,ASK_amp_ch1,1000,0);//设置ASK参数，CH1通道，A1=一半,A2=最大值，频率1000hz，相位0(0度)
	
	ASK_amp_ch2[0]=500;//ASK幅度1：一半
	ASK_amp_ch2[1]=1023;//ASK幅度2：最大
	AD9959_SetASK(CH2,ASK_amp_ch2,1000,0);//设置ASK参数，CH2通道，A1=一半,A2=最大值，频率1000hz，相位0(0度)
	
	ASK_amp_ch3[0]=500;//ASK幅度1：一半
	ASK_amp_ch3[1]=1023;//ASK幅度2：最大
	AD9959_SetASK(CH3,ASK_amp_ch3,1000,0);//设置ASK参数，CH3通道，A1=一半,A2=最大值，频率1000hz，相位0(0度)
	
	//每个通道的ASK参数可以不同，单独设置即可。
	//也可不将所有通道都设置为ASK调制模式，则未设置ASK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3分别控制CH0-CH3；低电平：输出幅度1
		HAL_Delay(10);
		PS0_1;PS1_1;PS2_1;PS3_1;//高电平：输出幅度2
		HAL_Delay(10);
	}
*/


//4ASK调制
/*
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	
//	最多只能同时有2个通道，设置成4电平调制。
//	本函数只能选择CH0-1，未做任意通道兼容，具体方法请参考AD9959芯片手册22-23页，操作FR1[14:12]为对应值。
	AD9959_Modulation_Init(CH0|CH1,ASK,SWEEP_DISABLE,LEVEL_MOD_4);//通道0-1设置为ASK模式，不启用线性扫描，4电平调制
	
	ASK_amp_ch0[0]=255;//ASK幅度1：四分之一，范围（0-1023）
	ASK_amp_ch0[1]=510;//ASK幅度2：
	ASK_amp_ch0[2]=765;//ASK幅度3：
	ASK_amp_ch0[3]=1023;//ASK幅度4：最大
	AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//设置ASK参数，CH0通道，写入电压幅度，频率1000hz，相位0(0度)
	
	ASK_amp_ch1[0]=255;//ASK幅度1：四分之一
	ASK_amp_ch1[1]=510;//ASK幅度2：
	ASK_amp_ch1[2]=765;//ASK幅度3：
	ASK_amp_ch1[3]=1023;//ASK幅度4：最大
	AD9959_SetASK(CH1,ASK_amp_ch1,1000,0);//设置ASK参数，CH1通道，写入电压幅度，频率1000hz，相位0(0度)
	
	//每个通道的ASK参数可以不同，单独设置即可。
	//也可不将所有通道都设置为ASK调制模式，则未设置ASK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		
		PS0_0;PS1_0;//PS0、PS1分别控制CH0；CH0输出幅度1
		PS2_0;PS3_0;//PS2、PS3分别控制CH1；CH1输出幅度1
		HAL_Delay(10);
		
		PS0_0;PS1_1;//CH0输出幅度2
		PS2_0;PS3_1;//CH1输出幅度2
		HAL_Delay(10);
		
		PS0_1;PS1_0;//CH0输出幅度3
		PS2_1;PS3_0;//CH1输出幅度3
		HAL_Delay(10);
		
		PS0_1;PS1_1;//CH0输出幅度4
		PS2_1;PS3_1;//CH1输出幅度4
		HAL_Delay(10);
	}
*/


//8ASK调制
/*
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	
//	最多只能同时有1个通道，设置成8电平调制。
//	本函数只能选择CH0做8电平调制，未做任意通道兼容，具体方法请参考AD9959芯片手册22-23页，操作FR1[14:12]为对应值。
	AD9959_Modulation_Init(CH0,ASK,SWEEP_DISABLE,LEVEL_MOD_8);//通道0设置为ASK模式，不启用线性扫描，8电平调制
	
	ASK_amp_ch0[0]=127;//ASK幅度1：八分之一 范围（0-1023）
	ASK_amp_ch0[1]=255;//ASK幅度2：八分之二
	ASK_amp_ch0[2]=383;//ASK幅度3：..
	ASK_amp_ch0[3]=511;//ASK幅度4：..
	ASK_amp_ch0[4]=639;//ASK幅度5：..
	ASK_amp_ch0[5]=767;//ASK幅度6：..
	ASK_amp_ch0[6]=895;//ASK幅度7：..
	ASK_amp_ch0[7]=1023;//ASK幅度8：最大
	AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//设置ASK参数，CH0通道，写入电压幅度，频率1000hz，相位0(0度)

	//每个通道的ASK参数可以不同，单独设置即可。
	//也可不将所有通道都设置为ASK调制模式，则未设置ASK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;//PS0-PS2控制CH0；CH0输出幅度1
		HAL_Delay(10);
		
		PS0_0;PS1_0;PS2_1;//CH0输出幅度2
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_0;//CH0输出幅度3
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_1;//CH0输出幅度4
		HAL_Delay(10);		
		
		PS0_1;PS1_0;PS2_0;//CH0输出幅度5
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_1;//CH0输出幅度6
		HAL_Delay(10);
	
		PS0_1;PS1_1;PS2_0;//CH0输出幅度7
		HAL_Delay(10);
		
		PS0_1;PS1_1;PS2_1;//CH0输出幅度8
		HAL_Delay(10);
	}
 */

//16ASK调制
/*	
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	
//	最多只能同时有1个通道，设置成16电平调制。
//	本函数只能选择CH0做16电平调制，未做任意通道兼容，具体方法请参考AD9959芯片手册22-23页，操作FR1[14:12]为对应值。
	AD9959_Modulation_Init(CH0,ASK,SWEEP_DISABLE,LEVEL_MOD_16);//通道0设置为ASK模式，不启用线性扫描，16电平调制
	
	ASK_amp_ch0[0]=64;//ASK幅度1：十六分之一，范围（0-1023）
	ASK_amp_ch0[1]=128;//ASK幅度2：十六分之二
	ASK_amp_ch0[2]=192;//ASK幅度3：...
	ASK_amp_ch0[3]=256;//ASK幅度4：...
	ASK_amp_ch0[4]=320;//ASK幅度5：...
	ASK_amp_ch0[5]=384;//ASK幅度6：...
	ASK_amp_ch0[6]=448;//ASK幅度7：...
	ASK_amp_ch0[7]=512;//ASK幅度8：...
	
	ASK_amp_ch0[8]=576;//ASK幅度9：...
	ASK_amp_ch0[9]=640;//ASK幅度10：...
	ASK_amp_ch0[10]=704;//ASK幅度11：...
	ASK_amp_ch0[11]=768;//ASK幅度12：...
	ASK_amp_ch0[12]=832;//ASK幅度13：...
	ASK_amp_ch0[13]=896;//ASK幅度14：...
	ASK_amp_ch0[14]=960;//ASK幅度15：...
	ASK_amp_ch0[15]=1023;//ASK幅度16：最大
	
	AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//设置FKS参数，CH0通道，写入电压幅度，频率1000hz，相位0(0度)
	
	//也可不将所有通道都设置为ASK调制模式，则未设置ASK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3控制CH0；CH0输出幅度1
		HAL_Delay(10);
		
		PS0_0;PS1_0;PS2_0;PS3_1;//CH0输出幅度2
		HAL_Delay(10);
		
		PS0_0;PS1_0;PS2_1;PS3_0;//CH0输出幅度3
		HAL_Delay(10);
		
		PS0_0;PS1_0;PS2_1;PS3_1;//CH0输出幅度4
		HAL_Delay(10);		
		
		PS0_0;PS1_1;PS2_0;PS3_0;//CH0输出幅度5
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_0;PS3_1;//CH0输出幅度6
		HAL_Delay(10);
	
		PS0_0;PS1_1;PS2_1;PS3_0;//CH0输出幅度7
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_1;PS3_1;//CH0输出幅度8
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_0;PS3_0;//CH0输出幅度9
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_0;PS3_1;//CH0输出幅度10
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_1;PS3_0;//CH0输出幅度11
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_1;PS3_1;//CH0输出幅度12
		HAL_Delay(10);		
		
		PS0_1;PS1_1;PS2_0;PS3_0;//CH0输出幅度13
		HAL_Delay(10);
		
		PS0_1;PS1_1;PS2_0;PS3_1;//CH0输出幅度14
		HAL_Delay(10);
	
		PS0_1;PS1_1;PS2_1;PS3_0;//CH0输出幅度15
		HAL_Delay(10);
		
		PS0_1;PS1_1;PS2_1;PS3_1;//CH0输出幅度16
		HAL_Delay(10);
	}
 */

//2PSK调制
/*
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,PSK,SWEEP_DISABLE,LEVEL_MOD_2);//通道0-3设置为PSK模式，不启用线性扫描，2电平调制
	PSK_phase_ch0[0]=0;//PSK相位1：0,范围：0~16383(对应角度：0°~360°)
	PSK_phase_ch0[1]=4095;//PSK相位2：90°
	AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//设置PSK参数，CH0通道，P1=0°,P2=90°，频率1000hz
	
	PSK_phase_ch1[0]=0;//PSK相位1：0
	PSK_phase_ch1[1]=4095;//PSK相位2：90°
	AD9959_SetPSK(CH1,PSK_phase_ch1,1000);//设置PSK参数，CH1通道，P1=0°,P2=90°，频率1000hz
	
	PSK_phase_ch2[0]=0;//PSK相位1：0
	PSK_phase_ch2[1]=4095;//PSK相位2：90°
	AD9959_SetPSK(CH2,PSK_phase_ch2,1000);//设置PSK参数，CH2通道，P1=0°,P2=90°，频率1000hz
	
	PSK_phase_ch3[0]=0;//PSK相位1：0
	PSK_phase_ch3[1]=4095;//PSK相位2：90°
	AD9959_SetPSK(CH3,PSK_phase_ch3,1000);//设置PSK参数，CH3通道，P1=0°,P2=90°，频率1000hz
	
	//每个通道的PSK参数可以不同，单独设置即可。
	//也可不将所有通道都设置为PSK调制模式，则未设置PSK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、相位、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3分别控制CH0-CH3；低电平：输出相位1
		HAL_Delay(2);
		PS0_1;PS1_1;PS2_1;PS3_1;//高电平：输出相位2
		HAL_Delay(2);
	}
*/



//4PSK调制
/*
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	
//	最多只能同时有2个通道，设置成4电平调制。
//	本函数只能选择CH0-1，未做任意通道兼容，具体方法请参考AD9959芯片手册22-23页，操作FR1[14:12]为对应值。
	AD9959_Modulation_Init(CH0|CH1,PSK,SWEEP_DISABLE,LEVEL_MOD_4);//通道0-1设置为PSK模式，不启用线性扫描，4电平调制
	
	PSK_phase_ch0[0]=0;//PSK相位1：0，范围：0~16383(对应角度：0°~360°)
	PSK_phase_ch0[1]=4095;//PSK相位2,90°
	PSK_phase_ch0[2]=8191;//PSK相位3：180°
	PSK_phase_ch0[3]=12287;//PSK相位4：270°
	AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//设置PSK参数，CH0通道，写入相位，频率1000hz
	
	PSK_phase_ch1[0]=0;//PSK相位1：0，范围：0~16383(对应角度：0°~360°)
	PSK_phase_ch1[1]=4095;//PSK相位2,90°
	PSK_phase_ch1[2]=8191;//PSK相位3：180°
	PSK_phase_ch1[3]=12287;//PSK相位4：270°
	AD9959_SetPSK(CH1,PSK_phase_ch1,1000);//设置PSK参数，CH1通道，写入相位，频率1000hz
	
	//每个通道的PSK参数可以不同，单独设置即可。
	//也可不将所有通道都设置为PSK调制模式，则未设置PSK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		
		PS0_0;PS1_0;//PS0、PS1分别控制CH0；CH0输出相位1
		PS2_0;PS3_0;//PS2、PS3分别控制CH1；CH1输出相位1
		HAL_Delay(2);
		
		PS0_0;PS1_1;//CH0输出相位2
		PS2_0;PS3_1;//CH1输出相位2
		HAL_Delay(2);
		
		PS0_1;PS1_0;//CH0输出相位3
		PS2_1;PS3_0;//CH1输出相位3
		HAL_Delay(2);
		
		PS0_1;PS1_1;//CH0输出相位4
		PS2_1;PS3_1;//CH1输出相位4
		HAL_Delay(2);
	}
*/


//8PSK调制
/*
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	
//	最多只能同时有1个通道，设置成8电平调制。
//	本函数只能选择CH0做8电平调制，未做任意通道兼容，具体方法请参考AD9959芯片手册22-23页，操作FR1[14:12]为对应值。
	AD9959_Modulation_Init(CH0,PSK,SWEEP_DISABLE,LEVEL_MOD_8);//通道0设置为PSK模式，不启用线性扫描，8电平调制
	
	PSK_phase_ch0[0]=0;//PSK相位1：0范围：0~16383(对应角度：0°~360°)
	PSK_phase_ch0[1]=2047;//PSK相位2：45°
	PSK_phase_ch0[2]=4095;//PSK相位3：90°
	PSK_phase_ch0[3]=6143;//PSK相位4：..
	PSK_phase_ch0[4]=8191;//PSK相位5：..
	PSK_phase_ch0[5]=10239;//PSK相位6：..
	PSK_phase_ch0[6]=10239;//PSK相位7：..
	PSK_phase_ch0[7]=14335;//PSK相位8：315°
	AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//设置PSK参数，CH0通道，写入相位，频率1000hz

	//每个通道的PSK参数可以不同，单独设置即可。
	//也可不将所有通道都设置为PSK调制模式，则未设置PSK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;//PS0-PS2控制CH0；CH0输出相位1
		HAL_Delay(2);
		
		PS0_0;PS1_0;PS2_1;//CH0输出相位2
		HAL_Delay(2);
		
		PS0_0;PS1_1;PS2_0;//CH0输出相位3
		HAL_Delay(2);
		
		PS0_0;PS1_1;PS2_1;//CH0输出相位4
		HAL_Delay(2);		
		
		PS0_1;PS1_0;PS2_0;//CH0输出相位5
		HAL_Delay(2);
		
		PS0_1;PS1_0;PS2_1;//CH0输出相位6
		HAL_Delay(2);
	
		PS0_1;PS1_1;PS2_0;//CH0输出相位7
		HAL_Delay(2);
		
		PS0_1;PS1_1;PS2_1;//CH0输出相位8
		HAL_Delay(2);
	}
*/


//16PSK调制
 /*
	AD9959_Init();	//初始化控制AD9959需要用到的IO口,及寄存器
	
//	最多只能同时有1个通道，设置成16电平调制。
//	本函数只能选择CH0做16电平调制，未做任意通道兼容，具体方法请参考AD9959芯片手册22-23页，操作FR1[14:12]为对应值。
	AD9959_Modulation_Init(CH0,PSK,SWEEP_DISABLE,LEVEL_MOD_16);//通道0设置为PSK模式，不启用线性扫描，16电平调制
	
	PSK_phase_ch0[0]=0;//PSK相位1：0，范围：0~16383(对应角度：0°~360°)
	PSK_phase_ch0[1]=1024;//PSK相位2：22.5°
	PSK_phase_ch0[2]=2048;//PSK相位3：45°
	PSK_phase_ch0[3]=3072;//PSK相位4：67.5°
	PSK_phase_ch0[4]=4096;//PSK相位5：90°
	PSK_phase_ch0[5]=5120;//PSK相位6：...
	PSK_phase_ch0[6]=6144;//PSK相位7：...
	PSK_phase_ch0[7]=7168;//PSK相位8：...
	
	PSK_phase_ch0[8]=8192;//PSK相位9：...
	PSK_phase_ch0[9]=9216;//PSK相位10：...
	PSK_phase_ch0[10]=10240;//PSK相位11：...
	PSK_phase_ch0[11]=11264;//PSK相位12：...
	PSK_phase_ch0[12]=12288;//PSK相位13：...
	PSK_phase_ch0[13]=13312;//PSK相位14：...
	PSK_phase_ch0[14]=14336;//PSK相位15：...
	PSK_phase_ch0[15]=15360;//PSK相位16：337.5°
	
	AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//设置PSK参数，CH1通道，写入相位，频率1000hz
	
	//也可不将所有通道都设置为PSK调制模式，则未设置PSK调制模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。
	
	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3控制CH0；CH0输出相位1
		HAL_Delay(2);
		
		PS0_0;PS1_0;PS2_0;PS3_1;//CH0输出相位2
		HAL_Delay(2);
		
		PS0_0;PS1_0;PS2_1;PS3_0;//CH0输出相位3
		HAL_Delay(2);
		
		PS0_0;PS1_0;PS2_1;PS3_1;//CH0输出相位4
		HAL_Delay(2);		
		
		PS0_0;PS1_1;PS2_0;PS3_0;//CH0输出相位5
		HAL_Delay(2);
		
		PS0_0;PS1_1;PS2_0;PS3_1;//CH0输出相位6
		HAL_Delay(2);
	
		PS0_0;PS1_1;PS2_1;PS3_0;//CH0输出相位7
		HAL_Delay(2);
		
		PS0_0;PS1_1;PS2_1;PS3_1;//CH0输出相位8
		HAL_Delay(2);
		
		PS0_1;PS1_0;PS2_0;PS3_0;//CH0输出相位9
		HAL_Delay(2);
		
		PS0_1;PS1_0;PS2_0;PS3_1;//CH0输出相位10
		HAL_Delay(2);
		
		PS0_1;PS1_0;PS2_1;PS3_0;//CH0输出相位11
		HAL_Delay(2);
		
		PS0_1;PS1_0;PS2_1;PS3_1;//CH0输出相位12
		HAL_Delay(2);		
		
		PS0_1;PS1_1;PS2_0;PS3_0;//CH0输出相位13
		HAL_Delay(2);
		
		PS0_1;PS1_1;PS2_0;PS3_1;//CH0输出相位14
		HAL_Delay(2);
	
		PS0_1;PS1_1;PS2_1;PS3_0;//CH0输出相位15
		HAL_Delay(2);
		
		PS0_1;PS1_1;PS2_1;PS3_1;//CH0输出相位16
		HAL_Delay(2);
	}
*/


//模式三 liner sweep mode(涉及到拓展io口)
// 扫频
/*	
	AD9959_Init();									//初始化控制AD9959需要用到的IO口,及寄存器
	
	//设置频率线性扫描，Nlevel只能是LEVEL_MOD_2
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,FSK,SWEEP_ENABLE,LEVEL_MOD_2);//通道0-3设置为FSK模式，启用线性扫描，2电平调制
	
										//	通道，						起始频率，结束频率，上升步长频率，下降步长频率，上升斜率，下降斜率，幅度 ，相位
	AD9959_SetFre_Sweep(CH0|CH1|CH2|CH3,		1000, 100000,			1,							1,				250,		250,			1023,		0);
	//上述函数效果为：从1000Hz扫描到100000Hz，步进1hz，扫频时间约198ms；再从100000Hz扫描到1000Hz，步进1hz，扫频时间约198ms；扫频时间计算参考函数注解
	
	//每个通道的频率线性扫描参数可以不同，单独设置即可。
	//也可不将所有通道都设置为频率线性扫描模式，则未设置频率线性扫描模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。

	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！

	while (1)
	{
	 	PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3分别控制CH0-CH3；高电平：从起始值扫到结束值
		HAL_Delay(198);
		PS0_0;PS1_0;PS2_0;PS3_0;//低电平：从结束值扫到起始值
		HAL_Delay(198);
	}      
*/



//扫幅
/*	
	AD9959_Init();									//初始化控制AD9959需要用到的IO口,及寄存器
	
	//设置频率线性扫描，Nlevel只能是LEVEL_MOD_2
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,ASK,SWEEP_ENABLE,LEVEL_MOD_2);//通道0-3设置为ASK模式，启用线性扫描，2电平调制
	
										//	通道，						起始幅度，结束幅度，上升步长幅度，下降步长幅度，上升斜率，下降斜率，频率 ，相位
	AD9959_SetAmp_Sweep(CH0|CH1|CH2|CH3,		0, 				1023,			1,							1,				250,		250,	10000,	0);
	//上述函数效果为：输出波形频率10000hz，从起始幅度扫描到结束幅度，步进1个幅度控制字，扫频时间约2ms；再从结束幅度扫描到起始幅度，步进1个幅度控制字，扫频时间约2ms；扫频时间计算参考函数注解
	
	//每个通道的幅度线性扫描参数可以不同，单独设置即可。
	//也可不将所有通道都设置为幅度线性扫描模式，则未设置幅度线性扫描模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。

	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3分别控制CH0-CH3；高电平：从起始值扫到结束值
		HAL_Delay(2);
		PS0_0;PS1_0;PS2_0;PS3_0;//低电平：从结束值扫到起始值
		HAL_Delay(2);
	}    
*/


//扫相
/*
	AD9959_Init();									//初始化控制AD9959需要用到的IO口,及寄存器

	//设置相位线性扫描，Nlevel只能是LEVEL_MOD_2
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,PSK,SWEEP_ENABLE,LEVEL_MOD_2);//通道0-3设置为PSK模式，启用线性扫描，2电平调制
	
										//	通道，						起始相位，结束相位，上升步长相位，下降步长相位，上升斜率，下降斜率，频率 ，幅度
	AD9959_SetPhase_Sweep(CH0|CH1|CH2|CH3,				0, 		16383,			1,							1,				250,		250,	10000,	1023);
	//上述函数效果为：输出波形频率10000hz，从起始相位扫描到结束相位，步进1个相位控制字，扫频时间约33ms；再从结束相位扫描到起始相位，步进1个相位控制字，扫频时间约33ms；扫频时间计算参考函数注解
	//只有两个及以上频率相同的信号，才有相位而言。
	
	//每个通道的相位线性扫描参数可以不同，单独设置即可。
	//也可不将所有通道都设置为相位线性扫描模式，则未设置相位线性扫描模式的通道，可使用AD9959_Set_Fre等函数独立设置其频率、幅度、相位;或设置成其他调制模式。

	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
	
	while(1)
	{
		PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3分别控制CH0-CH3；高电平：从起始值扫到结束值
		HAL_Delay(33);
		PS0_0;PS1_0;PS2_0;PS3_0;//低电平：从结束值扫到起始值
		HAL_Delay(33);
	}
 */



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */ 
	if(mode_change_flag==1)
	{
		OLED_CLS();
		mode_change_flag=0;
		switch(mode_cnt)
		{
			case 0:
			{
				OLED_ShowStr(20,3,(unsigned char *)"Single Tone",2);
				AD9959_Set_Fre(CH0, 100000);	//设置通道0频率100000Hz
				AD9959_Set_Fre(CH1, 200000);	//设置通道1频率100000Hz
				AD9959_Set_Fre(CH2, 100000);	//设置通道2频率100000Hz
				AD9959_Set_Fre(CH3, 100000);	//设置通道3频率100000Hz
		
				AD9959_Set_Amp(CH0, 1023); 		//设置通道0幅度控制值1023，范围0~1023
				AD9959_Set_Amp(CH1, 1023); 		//设置通道1幅度控制值1023，范围0~1023
				AD9959_Set_Amp(CH2, 1023); 		//设置通道2幅度控制值1023，范围0~1023
				AD9959_Set_Amp(CH3, 1023); 		//设置通道3幅度控制值1023，范围0~1023

				AD9959_Set_Phase(CH0, 0);			//设置通道0相位控制值0(0度)，范围0~16383
				AD9959_Set_Phase(CH1, 4096);	//设置通道1相位控制值4096(90度)，范围0~16383
				AD9959_Set_Phase(CH2, 8192);	//设置通道2相位控制值8192(180度)，范围0~16383
				AD9959_Set_Phase(CH3, 12288);	//设置通道3相位控制值12288(270度)，范围0~16383 
				IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！                    
				break;
			}
			case 1:
			{
				OLED_ShowStr(20,3,(unsigned char *)"2FSK",2); 
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,FSK,SWEEP_DISABLE,LEVEL_MOD_2);//通道0-3设置为FSK模式，不启用线性扫描，2电平调制
				FSK_fre_ch0[0]=1000;//FSK频率1：1000Hz
				FSK_fre_ch0[1]=200000;//FSK频率2：200000Hz
				AD9959_SetFSK(CH0,FSK_fre_ch0,0);//设置FKS参数，CH0通道，F1=1000Hz,F2=200000Hz，相位0(0度)
	
				FSK_fre_ch1[0]=1000;//FSK频率1：1000Hz
				FSK_fre_ch1[1]=200000;//FSK频率2：200000Hz
				AD9959_SetFSK(CH1,FSK_fre_ch1,0);//设置FKS参数，CH1通道，F1=1000Hz,F2=200000Hz，相位0(0度)
	
				FSK_fre_ch2[0]=1000;//FSK频率1：1000Hz
				FSK_fre_ch2[1]=200000;//FSK频率2：200000Hz
				AD9959_SetFSK(CH2,FSK_fre_ch2,0);//设置FKS参数，CH2通道，F1=1000Hz,F2=200000Hz，相位0(0度)
	
				FSK_fre_ch3[0]=1000;//FSK频率1：1000Hz
				FSK_fre_ch3[1]=200000;//FSK频率2：200000Hz
				AD9959_SetFSK(CH3,FSK_fre_ch3,0);//设置FKS参数，CH3通道，F1=1000Hz,F2=200000Hz，相位0(0度)        
	
				IO_Update();
				break;
			}
			case 2:
			{
				OLED_ShowStr(20,3,(unsigned char *)"4FSK",2); 
				AD9959_Modulation_Init(CH0|CH1,FSK,SWEEP_DISABLE,LEVEL_MOD_4);//通道0-1设置为FSK模式，不启用线性扫描，4电平调制
	
				FSK_fre_ch0[0]=100;//FSK频率1：100Hz
				FSK_fre_ch0[1]=1000;//FSK频率2：1000Hz
				FSK_fre_ch0[2]=10000;//FSK频率3：10000Hz
				FSK_fre_ch0[3]=100000;//FSK频率4：100000Hz
				AD9959_SetFSK(CH0,FSK_fre_ch0,0);//设置FKS参数，CH0通道，F1=100Hz,F2=1000Hz，F3=10000Hz,F4=100000Hz，相位0(0度)
	
				FSK_fre_ch1[0]=100;//FSK频率1：100Hz
				FSK_fre_ch1[1]=1000;//FSK频率2：1000Hz
				FSK_fre_ch1[2]=10000;//FSK频率3：10000Hz
				FSK_fre_ch1[3]=100000;//FSK频率4：100000Hz
				AD9959_SetFSK(CH1,FSK_fre_ch1,0);//设置FKS参数，CH1通道，F1=100Hz,F2=1000Hz，F3=10000Hz,F4=100000Hz，相位0(0度)
	
				IO_Update();
				break;
			}
			case 3:
			{
				OLED_ShowStr(20,3,(unsigned char *)"8FSK",2);
				AD9959_Modulation_Init(CH0,FSK,SWEEP_DISABLE,LEVEL_MOD_8);//通道0设置为FSK模式，不启用线性扫描，8电平调制
	
				FSK_fre_ch0[0]=100;//FSK频率1：100Hz
				FSK_fre_ch0[1]=500;//FSK频率2：500Hz
				FSK_fre_ch0[2]=1000;//FSK频率3：1000Hz
				FSK_fre_ch0[3]=1500;//FSK频率4：1500Hz
				FSK_fre_ch0[4]=2000;//FSK频率5：2000Hz
				FSK_fre_ch0[5]=2500;//FSK频率6：2500Hz
				FSK_fre_ch0[6]=3000;//FSK频率7：3000Hz
				FSK_fre_ch0[7]=3500;//FSK频率8：3500Hz
				AD9959_SetFSK(CH0,FSK_fre_ch0,0);//设置FKS参数，CH0通道，F1=100Hz,F2=500Hz，F3=1000Hz,F4=1500Hz.........，相位0(0度)
	
				IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
				break;
			}
			case 4:
			{
				OLED_ShowStr(20,3,(unsigned char *)"16FSK",2);
				AD9959_Modulation_Init(CH0,FSK,SWEEP_DISABLE,LEVEL_MOD_16);//通道0设置为FSK模式，不启用线性扫描，16电平调制
	
				FSK_fre_ch0[0]=10000;//FSK频率1：10000Hz
				FSK_fre_ch0[1]=20000;//FSK频率2：20000Hz
				FSK_fre_ch0[2]=50000;//FSK频率3：50000Hz
				FSK_fre_ch0[3]=80000;//FSK频率4：80000Hz
				FSK_fre_ch0[4]=120000;//FSK频率5：120000Hz
				FSK_fre_ch0[5]=140000;//FSK频率6：140000Hz
				FSK_fre_ch0[6]=180000;//FSK频率7：180000Hz
				FSK_fre_ch0[7]=200000;//FSK频率8：200000Hz
	
				FSK_fre_ch0[8]=250000;//FSK频率9：...
				FSK_fre_ch0[9]=270000;//FSK频率10：...
				FSK_fre_ch0[10]=300000;//FSK频率11：...
				FSK_fre_ch0[11]=320000;//FSK频率12：...
				FSK_fre_ch0[12]=350000;//FSK频率13：...
				FSK_fre_ch0[13]=400000;//FSK频率14：...
				FSK_fre_ch0[14]=600000;//FSK频率15：...
				FSK_fre_ch0[15]=10000000;//FSK频率16：...
	
				AD9959_SetFSK(CH0,FSK_fre_ch0,0);//设置FKS参数，CH0通道，F1=10000Hz,F2=20000Hz，F3=50000Hz........，相位0(0度)	
				IO_Update();	
				break;
			}
			case 5:
			{
				OLED_ShowStr(20,3,(unsigned char *)"2ASK",2);
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,ASK,SWEEP_DISABLE,LEVEL_MOD_2);//通道0-3设置为ASK模式，不启用线性扫描，2电平调制
				ASK_amp_ch0[0]=500;//ASK幅度1：一半,范围（0-1023）
				ASK_amp_ch0[1]=1023;//ASK幅度2：最大
				AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//设置ASK参数，CH0通道，A1=一半,A2=最大值，频率1000hz，相位0(0度)
	
				ASK_amp_ch1[0]=500;//ASK幅度1：一半
				ASK_amp_ch1[1]=1023;//ASK幅度2：最大
				AD9959_SetASK(CH1,ASK_amp_ch1,1000,0);//设置ASK参数，CH1通道，A1=一半,A2=最大值，频率1000hz，相位0(0度)
	
				ASK_amp_ch2[0]=500;//ASK幅度1：一半
				ASK_amp_ch2[1]=1023;//ASK幅度2：最大
				AD9959_SetASK(CH2,ASK_amp_ch2,1000,0);//设置ASK参数，CH2通道，A1=一半,A2=最大值，频率1000hz，相位0(0度)
	
				ASK_amp_ch3[0]=500;//ASK幅度1：一半
				ASK_amp_ch3[1]=1023;//ASK幅度2：最大
				AD9959_SetASK(CH3,ASK_amp_ch3,1000,0);//设置ASK参数，CH3通道，A1=一半,A2=最大值，频率1000hz，相位0(0度)

				IO_Update();	
				break;
			}
			case 6:
			{
				OLED_ShowStr(20,3,(unsigned char *)"4ASK",2);
				AD9959_Modulation_Init(CH0|CH1,ASK,SWEEP_DISABLE,LEVEL_MOD_4);//通道0-1设置为ASK模式，不启用线性扫描，4电平调制
	
				ASK_amp_ch0[0]=255;//ASK幅度1：四分之一，范围（0-1023）
				ASK_amp_ch0[1]=510;//ASK幅度2：
				ASK_amp_ch0[2]=765;//ASK幅度3：
				ASK_amp_ch0[3]=1023;//ASK幅度4：最大
				AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//设置ASK参数，CH0通道，写入电压幅度，频率1000hz，相位0(0度)
	
				ASK_amp_ch1[0]=255;//ASK幅度1：四分之一
				ASK_amp_ch1[1]=510;//ASK幅度2：
				ASK_amp_ch1[2]=765;//ASK幅度3：
				ASK_amp_ch1[3]=1023;//ASK幅度4：最大
				AD9959_SetASK(CH1,ASK_amp_ch1,1000,0);//设置ASK参数，CH1通道，写入电压幅度，频率1000hz，相位0(0度)

	
				IO_Update();	
				break;
			}
			case 7:
			{
				OLED_ShowStr(20,3,(unsigned char *)"8ASK",2);
				AD9959_Modulation_Init(CH0,ASK,SWEEP_DISABLE,LEVEL_MOD_8);//通道0设置为ASK模式，不启用线性扫描，8电平调制
	
				ASK_amp_ch0[0]=127;//ASK幅度1：八分之一 范围（0-1023）
				ASK_amp_ch0[1]=255;//ASK幅度2：八分之二
				ASK_amp_ch0[2]=383;//ASK幅度3：..
				ASK_amp_ch0[3]=511;//ASK幅度4：..
				ASK_amp_ch0[4]=639;//ASK幅度5：..
				ASK_amp_ch0[5]=767;//ASK幅度6：..
				ASK_amp_ch0[6]=895;//ASK幅度7：..
				ASK_amp_ch0[7]=1023;//ASK幅度8：最大
				AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//设置ASK参数，CH0通道，写入电压幅度，频率1000hz，相位0(0度)
	
				IO_Update();	
				break;
			}
			case 8:
			{
				OLED_ShowStr(20,3,(unsigned char *)"16ASK",2);
				AD9959_Modulation_Init(CH0,ASK,SWEEP_DISABLE,LEVEL_MOD_16);//通道0设置为ASK模式，不启用线性扫描，16电平调制
	
				ASK_amp_ch0[0]=64;//ASK幅度1：十六分之一，范围（0-1023）
				ASK_amp_ch0[1]=128;//ASK幅度2：十六分之二
				ASK_amp_ch0[2]=192;//ASK幅度3：...
				ASK_amp_ch0[3]=256;//ASK幅度4：...
				ASK_amp_ch0[4]=320;//ASK幅度5：...
				ASK_amp_ch0[5]=384;//ASK幅度6：...
				ASK_amp_ch0[6]=448;//ASK幅度7：...
				ASK_amp_ch0[7]=512;//ASK幅度8：...
	
				ASK_amp_ch0[8]=576;//ASK幅度9：...
				ASK_amp_ch0[9]=640;//ASK幅度10：...
				ASK_amp_ch0[10]=704;//ASK幅度11：...
				ASK_amp_ch0[11]=768;//ASK幅度12：...
				ASK_amp_ch0[12]=832;//ASK幅度13：...
				ASK_amp_ch0[13]=896;//ASK幅度14：...
				ASK_amp_ch0[14]=960;//ASK幅度15：...
				ASK_amp_ch0[15]=1023;//ASK幅度16：最大
	
				AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//设置FKS参数，CH0通道，写入电压幅度，频率1000hz，相位0(0度)	
				IO_Update();
				break;
			}
			case 9:
			{
				OLED_ShowStr(20,3,(unsigned char *)"2PSK",2);
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,PSK,SWEEP_DISABLE,LEVEL_MOD_2);//通道0-3设置为PSK模式，不启用线性扫描，2电平调制
				PSK_phase_ch0[0]=0;//PSK相位1：0,范围：0~16383(对应角度：0°~360°)
				PSK_phase_ch0[1]=4095;//PSK相位2：90°
				AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//设置PSK参数，CH0通道，P1=0°,P2=90°，频率1000hz
	
				PSK_phase_ch1[0]=0;//PSK相位1：0
				PSK_phase_ch1[1]=4095;//PSK相位2：90°
				AD9959_SetPSK(CH1,PSK_phase_ch1,1000);//设置PSK参数，CH1通道，P1=0°,P2=90°，频率1000hz
	
				PSK_phase_ch2[0]=0;//PSK相位1：0
				PSK_phase_ch2[1]=4095;//PSK相位2：90°
				AD9959_SetPSK(CH2,PSK_phase_ch2,1000);//设置PSK参数，CH2通道，P1=0°,P2=90°，频率1000hz
	
				PSK_phase_ch3[0]=0;//PSK相位1：0
				PSK_phase_ch3[1]=4095;//PSK相位2：90°
				AD9959_SetPSK(CH3,PSK_phase_ch3,1000);//设置PSK参数，CH3通道，P1=0°,P2=90°，频率1000hz
		
				IO_Update();	
				break;
			}
			case 10:
			{
				OLED_ShowStr(20,3,(unsigned char *)"4PSK",2);
				AD9959_Modulation_Init(CH0|CH1,PSK,SWEEP_DISABLE,LEVEL_MOD_4);//通道0-1设置为PSK模式，不启用线性扫描，4电平调制
	
				PSK_phase_ch0[0]=0;//PSK相位1：0，范围：0~16383(对应角度：0°~360°)
				PSK_phase_ch0[1]=4095;//PSK相位2,90°
				PSK_phase_ch0[2]=8191;//PSK相位3：180°
				PSK_phase_ch0[3]=12287;//PSK相位4：270°
				AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//设置PSK参数，CH0通道，写入相位，频率1000hz
	
				PSK_phase_ch1[0]=0;//PSK相位1：0，范围：0~16383(对应角度：0°~360°)
				PSK_phase_ch1[1]=4095;//PSK相位2,90°
				PSK_phase_ch1[2]=8191;//PSK相位3：180°
				PSK_phase_ch1[3]=12287;//PSK相位4：270°

				AD9959_SetPSK(CH1,PSK_phase_ch1,1000);//设置PSK参数，CH1通道，写入相位，频率1000hz
				IO_Update();
				break;
			}
			case 11:
			{
				OLED_ShowStr(20,3,(unsigned char *)"8PSK",2);
				AD9959_Modulation_Init(CH0,PSK,SWEEP_DISABLE,LEVEL_MOD_8);//通道0设置为PSK模式，不启用线性扫描，8电平调制
	
				PSK_phase_ch0[0]=0;//PSK相位1：0范围：0~16383(对应角度：0°~360°)
				PSK_phase_ch0[1]=2047;//PSK相位2：45°
				PSK_phase_ch0[2]=4095;//PSK相位3：90°
				PSK_phase_ch0[3]=6143;//PSK相位4：..
				PSK_phase_ch0[4]=8191;//PSK相位5：..
				PSK_phase_ch0[5]=10239;//PSK相位6：..
				PSK_phase_ch0[6]=10239;//PSK相位7：..
				PSK_phase_ch0[7]=14335;//PSK相位8：315°
				
				AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//设置PSK参数，CH0通道，写入相位，频率1000hz
				IO_Update();
				break;
			}
			case 12:
			{
				OLED_ShowStr(20,3,(unsigned char *)"16PSK",2);
				AD9959_Modulation_Init(CH0,PSK,SWEEP_DISABLE,LEVEL_MOD_16);
	
				PSK_phase_ch0[0]=0;//PSK相位1：0，范围：0~16383(对应角度：0°~360°)
				PSK_phase_ch0[1]=1024;//PSK相位2：22.5°
				PSK_phase_ch0[2]=2048;//PSK相位3：45°
				PSK_phase_ch0[3]=3072;//PSK相位4：67.5°
				PSK_phase_ch0[4]=4096;//PSK相位5：90°
				PSK_phase_ch0[5]=5120;//PSK相位6：...
				PSK_phase_ch0[6]=6144;//PSK相位7：...
				PSK_phase_ch0[7]=7168;//PSK相位8：...
	
				PSK_phase_ch0[8]=8192;//PSK相位9：...
				PSK_phase_ch0[9]=9216;//PSK相位10：...
				PSK_phase_ch0[10]=10240;//PSK相位11：...
				PSK_phase_ch0[11]=11264;//PSK相位12：...
				PSK_phase_ch0[12]=12288;//PSK相位13：...
				PSK_phase_ch0[13]=13312;//PSK相位14：...
				PSK_phase_ch0[14]=14336;//PSK相位15：...
				PSK_phase_ch0[15]=15360;//PSK相位16：337.5°
	
				AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//设置PSK参数，CH1通道，写入相位，频率1000hz
				IO_Update();
				break;
			}
			case 13:
			{
				OLED_ShowStr(0,3,(unsigned char *)"Sweep Frequency",2);
					//设置频率线性扫描，Nlevel只能是LEVEL_MOD_2
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,FSK,SWEEP_ENABLE,LEVEL_MOD_2);//通道0-3设置为FSK模式，启用线性扫描，2电平调制
	
										//	通道，					起始相位,   结束相位,  上升步长相位,         下降步长相位， 上升斜率,下降斜率 ,频率 ,   幅度
				AD9959_SetFre_Sweep(CH0|CH1|CH2|CH3,		1000, 100000,			1,							1,				250,		250,			1023,		0);
				IO_Update();
				break;
			}
			case 14:
			{
				OLED_ShowStr(0,3,(unsigned char *)"Sweep Ampilitude",2);
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,ASK,SWEEP_ENABLE,LEVEL_MOD_2);
		   								//	通道，					起始相位,   结束相位,  上升步长相位,         下降步长相位， 上升斜率,下降斜率 ,频率 ,   幅度
				AD9959_SetAmp_Sweep(CH0|CH1|CH2|CH3,		0, 				1023,			1,							1,				250,		250,	10000,	0);
				IO_Update();
				break;
			}
			case 15:
			{
				OLED_ShowStr(0,3,(unsigned char *)"Sweep Phase",2);
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,PSK,SWEEP_ENABLE,LEVEL_MOD_2);
												//	通道，					起始相位,   结束相位,  上升步长相位,         下降步长相位， 上升斜率,下降斜率 ,频率 ,   幅度
				AD9959_SetPhase_Sweep(CH0|CH1|CH2|CH3,				0, 		16383,			1,							1,				250,		250,	10000,	1023);
				IO_Update();
				break;
			}
		}
	  }
	  else
	  {
			switch(mode_cnt)
		{
			case 0:
			{
				break;
			}
			case 1:
			{
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3分别控制CH0-CH3；低电平：输出频率1
				HAL_Delay(10);
				PS0_1;PS1_1;PS2_1;PS3_1;//高电平：输出频率2
				HAL_Delay(10);           
				break;
			}
			case 2:
			{
				PS0_0;PS1_0;//PS0、PS1分别控制CH0；CH0输出频率1
				PS2_0;PS3_0;//PS2、PS3分别控制CH1；CH1输出频率1
				HAL_Delay(10);
		
				PS0_0;PS1_1;//CH0输出频率2
				PS2_0;PS3_1;//CH1输出频率2
				HAL_Delay(10);
		
				PS0_1;PS1_0;//CH0输出频率3
				PS2_1;PS3_0;//CH1输出频率3
				HAL_Delay(10);
		
				PS0_1;PS1_1;//CH0输出频率4
				PS2_1;PS3_1;//CH1输出频率4
				HAL_Delay(10);
				break;
			}
			case 3:
			{
				PS0_0;PS1_0;PS2_0;//PS0-PS2控制CH0；CH0输出频率1
				HAL_Delay(10);
		
				PS0_0;PS1_0;PS2_1;//CH0输出频率2
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_0;//CH0输出频率3
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_1;//CH0输出频率4
				HAL_Delay(10);		
		
				PS0_1;PS1_0;PS2_0;//CH0输出频率5
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_1;//CH0输出频率6
				HAL_Delay(10);
	
				PS0_1;PS1_1;PS2_0;//CH0输出频率7
				HAL_Delay(10);
		
				PS0_1;PS1_1;PS2_1;//CH0输出频率8
				HAL_Delay(10);
				break;
			}
			case 4:
			{
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3控制CH0；CH0输出频率1
				delay_us(15);
		
				PS0_0;PS1_0;PS2_0;PS3_1;//CH0输出频率2
				delay_us(15);
		
				PS0_0;PS1_0;PS2_1;PS3_0;//CH0输出频率3
				delay_us(15);
		
				PS0_0;PS1_0;PS2_1;PS3_1;//CH0输出频率4
				delay_us(15);		
		
				PS0_0;PS1_1;PS2_0;PS3_0;//CH0输出频率5
				delay_us(15);
		
				PS0_0;PS1_1;PS2_0;PS3_1;//CH0输出频率6
				delay_us(15);
	
				PS0_0;PS1_1;PS2_1;PS3_0;//CH0输出频率7
				delay_us(15);
		
				PS0_0;PS1_1;PS2_1;PS3_1;//CH0输出频率8
				delay_us(15);
		
				PS0_1;PS1_0;PS2_0;PS3_0;//CH0输出频率9
				delay_us(15);
		
				PS0_1;PS1_0;PS2_0;PS3_1;//CH0输出频率10
				delay_us(15);
		
				PS0_1;PS1_0;PS2_1;PS3_0;//CH0输出频率11
				delay_us(15);
		
				PS0_1;PS1_0;PS2_1;PS3_1;//CH0输出频率12
				delay_us(15);		
		
				PS0_1;PS1_1;PS2_0;PS3_0;//CH0输出频率13
				delay_us(15);
		
				PS0_1;PS1_1;PS2_0;PS3_1;//CH0输出频率14
				delay_us(15);
	
				PS0_1;PS1_1;PS2_1;PS3_0;//CH0输出频率15
				delay_us(15);
		
				PS0_1;PS1_1;PS2_1;PS3_1;//CH0输出频率16
				delay_us(15);
				break;
			}
			case 5:
			{
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3分别控制CH0-CH3；低电平：输出幅度1
				HAL_Delay(10);
				PS0_1;PS1_1;PS2_1;PS3_1;//高电平：输出幅度2
				HAL_Delay(10);
				break;
			}
			case 6:
			{
				PS0_0;PS1_0;//PS0、PS1分别控制CH0；CH0输出幅度1
				PS2_0;PS3_0;//PS2、PS3分别控制CH1；CH1输出幅度1
				HAL_Delay(10);
		
				PS0_0;PS1_1;//CH0输出幅度2
				PS2_0;PS3_1;//CH1输出幅度2
				HAL_Delay(10);
		
				PS0_1;PS1_0;//CH0输出幅度3
				PS2_1;PS3_0;//CH1输出幅度3
				HAL_Delay(10);
		
				PS0_1;PS1_1;//CH0输出幅度4
				PS2_1;PS3_1;//CH1输出幅度4
				HAL_Delay(10);
				break;
			}
			case 7:
			{		
				PS0_0;PS1_0;PS2_0;//PS0-PS2控制CH0；CH0输出幅度1
				HAL_Delay(10);
		
				PS0_0;PS1_0;PS2_1;//CH0输出幅度2
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_0;//CH0输出幅度3
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_1;//CH0输出幅度4
				HAL_Delay(10);		
		
				PS0_1;PS1_0;PS2_0;//CH0输出幅度5
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_1;//CH0输出幅度6
				HAL_Delay(10);
	
				PS0_1;PS1_1;PS2_0;//CH0输出幅度7
				HAL_Delay(10);
			
				PS0_1;PS1_1;PS2_1;//CH0输出幅度8
				HAL_Delay(10);
				break;
			}
			case 8:
			{
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3控制CH0；CH0输出幅度1
				HAL_Delay(10);
		
				PS0_0;PS1_0;PS2_0;PS3_1;//CH0输出幅度2
				HAL_Delay(10);
		
				PS0_0;PS1_0;PS2_1;PS3_0;//CH0输出幅度3
				HAL_Delay(10);
		
				PS0_0;PS1_0;PS2_1;PS3_1;//CH0输出幅度4
				HAL_Delay(10);		
		
				PS0_0;PS1_1;PS2_0;PS3_0;//CH0输出幅度5
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_0;PS3_1;//CH0输出幅度6
				HAL_Delay(10);
	
				PS0_0;PS1_1;PS2_1;PS3_0;//CH0输出幅度7
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_1;PS3_1;//CH0输出幅度8
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_0;PS3_0;//CH0输出幅度9
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_0;PS3_1;//CH0输出幅度10
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_1;PS3_0;//CH0输出幅度11
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_1;PS3_1;//CH0输出幅度12
				HAL_Delay(10);		
		
				PS0_1;PS1_1;PS2_0;PS3_0;//CH0输出幅度13
				HAL_Delay(10);
		
				PS0_1;PS1_1;PS2_0;PS3_1;//CH0输出幅度14
				HAL_Delay(10);
	
				PS0_1;PS1_1;PS2_1;PS3_0;//CH0输出幅度15
				HAL_Delay(10);
		
				PS0_1;PS1_1;PS2_1;PS3_1;//CH0输出幅度16
				HAL_Delay(10);
				break;
			}
			case 9:
			{
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3分别控制CH0-CH3；低电平：输出相位1
				HAL_Delay(2);
				PS0_1;PS1_1;PS2_1;PS3_1;//高电平：输出相位2
				HAL_Delay(2);
				break;
			}
			case 10:
			{
				PS0_0;PS1_0;//PS0、PS1分别控制CH0；CH0输出相位1
				PS2_0;PS3_0;//PS2、PS3分别控制CH1；CH1输出相位1
				HAL_Delay(2);
		
				PS0_0;PS1_1;//CH0输出相位2
				PS2_0;PS3_1;//CH1输出相位2
				HAL_Delay(2);
		
				PS0_1;PS1_0;//CH0输出相位3
				PS2_1;PS3_0;//CH1输出相位3
				HAL_Delay(2);
		
				PS0_1;PS1_1;//CH0输出相位4
				PS2_1;PS3_1;//CH1输出相位4
				HAL_Delay(2);
				break;
			}
			case 11:
			{
				PS0_0;PS1_0;PS2_0;//PS0-PS2控制CH0；CH0输出相位1
				HAL_Delay(2);
		
				PS0_0;PS1_0;PS2_1;//CH0输出相位2
				HAL_Delay(2);
		
				PS0_0;PS1_1;PS2_0;//CH0输出相位3
				HAL_Delay(2);
		
				PS0_0;PS1_1;PS2_1;//CH0输出相位4
				HAL_Delay(2);		
		
				PS0_1;PS1_0;PS2_0;//CH0输出相位5
				HAL_Delay(2);
		
				PS0_1;PS1_0;PS2_1;//CH0输出相位6
				HAL_Delay(2);
			
				PS0_1;PS1_1;PS2_0;//CH0输出相位7
				HAL_Delay(2);
		
				PS0_1;PS1_1;PS2_1;//CH0输出相位8
				HAL_Delay(2);
				break;
			}
			case 12:
			{
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3控制CH0；CH0输出相位1
				HAL_Delay(2);
		
				PS0_0;PS1_0;PS2_0;PS3_1;//CH0输出相位2
				HAL_Delay(2);
		
				PS0_0;PS1_0;PS2_1;PS3_0;//CH0输出相位3
				HAL_Delay(2);
		
				PS0_0;PS1_0;PS2_1;PS3_1;//CH0输出相位4
				HAL_Delay(2);		
			
				PS0_0;PS1_1;PS2_0;PS3_0;//CH0输出相位5
				HAL_Delay(2);
		
				PS0_0;PS1_1;PS2_0;PS3_1;//CH0输出相位6
				HAL_Delay(2);
	
				PS0_0;PS1_1;PS2_1;PS3_0;//CH0输出相位7
				HAL_Delay(2);
		
				PS0_0;PS1_1;PS2_1;PS3_1;//CH0输出相位8
				HAL_Delay(2);
		
				PS0_1;PS1_0;PS2_0;PS3_0;//CH0输出相位9
				HAL_Delay(2);
		
				PS0_1;PS1_0;PS2_0;PS3_1;//CH0输出相位10
				HAL_Delay(2);
		
				PS0_1;PS1_0;PS2_1;PS3_0;//CH0输出相位11
				HAL_Delay(2);
		
				PS0_1;PS1_0;PS2_1;PS3_1;//CH0输出相位12
				HAL_Delay(2);		
		
				PS0_1;PS1_1;PS2_0;PS3_0;//CH0输出相位13
				HAL_Delay(2);
		
				PS0_1;PS1_1;PS2_0;PS3_1;//CH0输出相位14
				HAL_Delay(2);
	
				PS0_1;PS1_1;PS2_1;PS3_0;//CH0输出相位15
				HAL_Delay(2);
		
				PS0_1;PS1_1;PS2_1;PS3_1;//CH0输出相位16
				HAL_Delay(2);
				break;
			}
			case 13:
			{
				PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3分别控制CH0-CH3；高电平：从起始值扫到结束值
				HAL_Delay(198);
				PS0_0;PS1_0;PS2_0;PS3_0;//低电平：从结束值扫到起始值
				HAL_Delay(198);
				break;
			}
			case 14:
			{
				PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3分别控制CH0-CH3；高电平：从起始值扫到结束值
				HAL_Delay(2);
				PS0_0;PS1_0;PS2_0;PS3_0;//低电平：从结束值扫到起始值
				HAL_Delay(2);
				break;
			}
			case 15:
			{
				PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3分别控制CH0-CH3；高电平：从起始值扫到结束值
				HAL_Delay(33);
				PS0_0;PS1_0;PS2_0;PS3_0;//低电平：从结束值扫到起始值
				HAL_Delay(33);
				break;
			}
		}
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
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
	if(KEY_Pin)
	{
		mode_change_flag=1;
		mode_cnt=(mode_cnt+1)%16;
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
