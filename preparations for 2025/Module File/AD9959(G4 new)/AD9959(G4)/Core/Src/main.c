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
uint32_t FSK_fre_ch3[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//FSK����

uint16_t ASK_amp_ch0[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t ASK_amp_ch1[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t ASK_amp_ch2[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t ASK_amp_ch3[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//ASK����

uint16_t PSK_phase_ch0[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t PSK_phase_ch1[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t PSK_phase_ch2[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t PSK_phase_ch3[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//PSK����
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


//ģʽһ singel tone mode(���漰����չio��)
//�ط������࣬��Ƶ

	//AD9959_Init();						//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	AD9959_Set_Fre(CH0, 100000);	//����ͨ��0Ƶ��100000Hz
	AD9959_Set_Fre(CH1, 200000);	//����ͨ��1Ƶ��100000Hz
	AD9959_Set_Fre(CH2, 100000);	//����ͨ��2Ƶ��100000Hz
	AD9959_Set_Fre(CH3, 100000);	//����ͨ��3Ƶ��100000Hz
		
	AD9959_Set_Amp(CH0, 1023); 		//����ͨ��0���ȿ���ֵ1023����Χ0~1023
	AD9959_Set_Amp(CH1, 1023); 		//����ͨ��1���ȿ���ֵ1023����Χ0~1023
	AD9959_Set_Amp(CH2, 1023); 		//����ͨ��2���ȿ���ֵ1023����Χ0~1023
	AD9959_Set_Amp(CH3, 1023); 		//����ͨ��3���ȿ���ֵ1023����Χ0~1023

	AD9959_Set_Phase(CH0, 0);			//����ͨ��0��λ����ֵ0(0��)����Χ0~16383
	AD9959_Set_Phase(CH1, 4096);	//����ͨ��1��λ����ֵ4096(90��)����Χ0~16383
	AD9959_Set_Phase(CH2, 8192);	//����ͨ��2��λ����ֵ8192(180��)����Χ0~16383
	AD9959_Set_Phase(CH3, 12288);	//����ͨ��3��λ����ֵ12288(270��)����Χ0~16383 
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������                      
 



//ģʽ�� modulation mode(�漰����չio��)
//2FSK����
/*	
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,FSK,SWEEP_DISABLE,LEVEL_MOD_2);//ͨ��0-3����ΪFSKģʽ������������ɨ�裬2��ƽ����
	FSK_fre_ch0[0]=1000;//FSKƵ��1��1000Hz
	FSK_fre_ch0[1]=200000;//FSKƵ��2��200000Hz
	AD9959_SetFSK(CH0,FSK_fre_ch0,0);//����FKS������CH0ͨ����F1=1000Hz,F2=200000Hz����λ0(0��)
	
	FSK_fre_ch1[0]=1000;//FSKƵ��1��1000Hz
	FSK_fre_ch1[1]=200000;//FSKƵ��2��200000Hz
	AD9959_SetFSK(CH1,FSK_fre_ch1,0);//����FKS������CH1ͨ����F1=1000Hz,F2=200000Hz����λ0(0��)
	
	FSK_fre_ch2[0]=1000;//FSKƵ��1��1000Hz
	FSK_fre_ch2[1]=200000;//FSKƵ��2��200000Hz
	AD9959_SetFSK(CH2,FSK_fre_ch2,0);//����FKS������CH2ͨ����F1=1000Hz,F2=200000Hz����λ0(0��)
	
	FSK_fre_ch3[0]=1000;//FSKƵ��1��1000Hz
	FSK_fre_ch3[1]=200000;//FSKƵ��2��200000Hz
	AD9959_SetFSK(CH3,FSK_fre_ch3,0);//����FKS������CH3ͨ����F1=1000Hz,F2=200000Hz����λ0(0��)        
	
	//ÿ��ͨ����FSK�������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������ΪFSK����ģʽ����δ����FSK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������                                 
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3�ֱ����CH0-CH3���͵�ƽ�����Ƶ��1
		HAL_Delay(10);
		PS0_1;PS1_1;PS2_1;PS3_1;//�ߵ�ƽ�����Ƶ��2
		HAL_Delay(10);  
	}  
*/

 
//4FSK����
/*
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	
//	���ֻ��ͬʱ��2��ͨ�������ó�4��ƽ���ơ�
//	������ֻ��ѡ��CH0-1��δ������ͨ�����ݣ����巽����ο�AD9959оƬ�ֲ�22-23ҳ������FR1[14:12]Ϊ��Ӧֵ��
	AD9959_Modulation_Init(CH0|CH1,FSK,SWEEP_DISABLE,LEVEL_MOD_4);//ͨ��0-1����ΪFSKģʽ������������ɨ�裬4��ƽ����
	
	FSK_fre_ch0[0]=100;//FSKƵ��1��100Hz
	FSK_fre_ch0[1]=1000;//FSKƵ��2��1000Hz
	FSK_fre_ch0[2]=10000;//FSKƵ��3��10000Hz
	FSK_fre_ch0[3]=100000;//FSKƵ��4��100000Hz
	AD9959_SetFSK(CH0,FSK_fre_ch0,0);//����FKS������CH0ͨ����F1=100Hz,F2=1000Hz��F3=10000Hz,F4=100000Hz����λ0(0��)
	
	FSK_fre_ch1[0]=100;//FSKƵ��1��100Hz
	FSK_fre_ch1[1]=1000;//FSKƵ��2��1000Hz
	FSK_fre_ch1[2]=10000;//FSKƵ��3��10000Hz
	FSK_fre_ch1[3]=100000;//FSKƵ��4��100000Hz
	AD9959_SetFSK(CH1,FSK_fre_ch1,0);//����FKS������CH1ͨ����F1=100Hz,F2=1000Hz��F3=10000Hz,F4=100000Hz����λ0(0��)
	
	//ÿ��ͨ����FSK�������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������ΪFSK����ģʽ����δ����FSK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		
		PS0_0;PS1_0;//PS0��PS1�ֱ����CH0��CH0���Ƶ��1
		PS2_0;PS3_0;//PS2��PS3�ֱ����CH1��CH1���Ƶ��1
		HAL_Delay(10);
		
		PS0_0;PS1_1;//CH0���Ƶ��2
		PS2_0;PS3_1;//CH1���Ƶ��2
		HAL_Delay(10);
		
		PS0_1;PS1_0;//CH0���Ƶ��3
		PS2_1;PS3_0;//CH1���Ƶ��3
		HAL_Delay(10);
		
		PS0_1;PS1_1;//CH0���Ƶ��4
		PS2_1;PS3_1;//CH1���Ƶ��4
		HAL_Delay(10);
	}
*/



//8FSK����
/*	
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	
//	���ֻ��ͬʱ��1��ͨ�������ó�8��ƽ���ơ�
//	������ֻ��ѡ��CH0��8��ƽ���ƣ�δ������ͨ�����ݣ����巽����ο�AD9959оƬ�ֲ�22-23ҳ������FR1[14:12]Ϊ��Ӧֵ��
	AD9959_Modulation_Init(CH0,FSK,SWEEP_DISABLE,LEVEL_MOD_8);//ͨ��0����ΪFSKģʽ������������ɨ�裬8��ƽ����
	
	FSK_fre_ch0[0]=100;//FSKƵ��1��100Hz
	FSK_fre_ch0[1]=500;//FSKƵ��2��500Hz
	FSK_fre_ch0[2]=1000;//FSKƵ��3��1000Hz
	FSK_fre_ch0[3]=1500;//FSKƵ��4��1500Hz
	FSK_fre_ch0[4]=2000;//FSKƵ��5��2000Hz
	FSK_fre_ch0[5]=2500;//FSKƵ��6��2500Hz
	FSK_fre_ch0[6]=3000;//FSKƵ��7��3000Hz
	FSK_fre_ch0[7]=3500;//FSKƵ��8��3500Hz
	AD9959_SetFSK(CH0,FSK_fre_ch0,0);//����FKS������CH0ͨ����F1=100Hz,F2=500Hz��F3=1000Hz,F4=1500Hz.........����λ0(0��)

	//ÿ��ͨ����FSK�������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������ΪFSK����ģʽ����δ����FSK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;//PS0-PS2����CH0��CH0���Ƶ��1
		HAL_Delay(10);
		
		PS0_0;PS1_0;PS2_1;//CH0���Ƶ��2
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_0;//CH0���Ƶ��3
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_1;//CH0���Ƶ��4
		HAL_Delay(10);		
		
		PS0_1;PS1_0;PS2_0;//CH0���Ƶ��5
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_1;//CH0���Ƶ��6
		HAL_Delay(10);
	
		PS0_1;PS1_1;PS2_0;//CH0���Ƶ��7
		HAL_Delay(10);
		
		PS0_1;PS1_1;PS2_1;//CH0���Ƶ��8
		HAL_Delay(10);
	}               
*/

//16FSK����
/*
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	
//	���ֻ��ͬʱ��1��ͨ�������ó�16��ƽ���ơ�
//	������ֻ��ѡ��CH0��16��ƽ���ƣ�δ������ͨ�����ݣ����巽����ο�AD9959оƬ�ֲ�22-23ҳ������FR1[14:12]Ϊ��Ӧֵ��
	AD9959_Modulation_Init(CH0,FSK,SWEEP_DISABLE,LEVEL_MOD_16);//ͨ��0����ΪFSKģʽ������������ɨ�裬16��ƽ����
	
	FSK_fre_ch0[0]=10000;//FSKƵ��1��10000Hz
	FSK_fre_ch0[1]=20000;//FSKƵ��2��20000Hz
	FSK_fre_ch0[2]=50000;//FSKƵ��3��50000Hz
	FSK_fre_ch0[3]=80000;//FSKƵ��4��80000Hz
	FSK_fre_ch0[4]=120000;//FSKƵ��5��120000Hz
	FSK_fre_ch0[5]=140000;//FSKƵ��6��140000Hz
	FSK_fre_ch0[6]=180000;//FSKƵ��7��180000Hz
	FSK_fre_ch0[7]=200000;//FSKƵ��8��200000Hz
	
	FSK_fre_ch0[8]=250000;//FSKƵ��9��...
	FSK_fre_ch0[9]=270000;//FSKƵ��10��...
	FSK_fre_ch0[10]=300000;//FSKƵ��11��...
	FSK_fre_ch0[11]=320000;//FSKƵ��12��...
	FSK_fre_ch0[12]=350000;//FSKƵ��13��...
	FSK_fre_ch0[13]=400000;//FSKƵ��14��...
	FSK_fre_ch0[14]=600000;//FSKƵ��15��...
	FSK_fre_ch0[15]=10000000;//FSKƵ��16��...
	
	AD9959_SetFSK(CH0,FSK_fre_ch0,0);//����FKS������CH0ͨ����F1=10000Hz,F2=20000Hz��F3=50000Hz........����λ0(0��)

	//Ҳ�ɲ�������ͨ��������ΪFSK����ģʽ����δ����FSK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3����CH0��CH0���Ƶ��1
		delay_us(15);
		
		PS0_0;PS1_0;PS2_0;PS3_1;//CH0���Ƶ��2
		delay_us(15);
		
		PS0_0;PS1_0;PS2_1;PS3_0;//CH0���Ƶ��3
		delay_us(15);
		
		PS0_0;PS1_0;PS2_1;PS3_1;//CH0���Ƶ��4
		delay_us(15);		
		
		PS0_0;PS1_1;PS2_0;PS3_0;//CH0���Ƶ��5
		delay_us(15);
		
		PS0_0;PS1_1;PS2_0;PS3_1;//CH0���Ƶ��6
		delay_us(15);
	
		PS0_0;PS1_1;PS2_1;PS3_0;//CH0���Ƶ��7
		delay_us(15);
		
		PS0_0;PS1_1;PS2_1;PS3_1;//CH0���Ƶ��8
		delay_us(15);
		
		PS0_1;PS1_0;PS2_0;PS3_0;//CH0���Ƶ��9
		delay_us(15);
		
		PS0_1;PS1_0;PS2_0;PS3_1;//CH0���Ƶ��10
		delay_us(15);
		
		PS0_1;PS1_0;PS2_1;PS3_0;//CH0���Ƶ��11
		delay_us(15);
		
		PS0_1;PS1_0;PS2_1;PS3_1;//CH0���Ƶ��12
		delay_us(15);		
		
		PS0_1;PS1_1;PS2_0;PS3_0;//CH0���Ƶ��13
		delay_us(15);
		
		PS0_1;PS1_1;PS2_0;PS3_1;//CH0���Ƶ��14
		delay_us(15);
	
		PS0_1;PS1_1;PS2_1;PS3_0;//CH0���Ƶ��15
		delay_us(15);
		
		PS0_1;PS1_1;PS2_1;PS3_1;//CH0���Ƶ��16
		delay_us(15);
	}
*/


//2ASK����
 /*
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,ASK,SWEEP_DISABLE,LEVEL_MOD_2);//ͨ��0-3����ΪASKģʽ������������ɨ�裬2��ƽ����
	ASK_amp_ch0[0]=500;//ASK����1��һ��,��Χ��0-1023��
	ASK_amp_ch0[1]=1023;//ASK����2�����
	AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//����ASK������CH0ͨ����A1=һ��,A2=���ֵ��Ƶ��1000hz����λ0(0��)
	
	ASK_amp_ch1[0]=500;//ASK����1��һ��
	ASK_amp_ch1[1]=1023;//ASK����2�����
	AD9959_SetASK(CH1,ASK_amp_ch1,1000,0);//����ASK������CH1ͨ����A1=һ��,A2=���ֵ��Ƶ��1000hz����λ0(0��)
	
	ASK_amp_ch2[0]=500;//ASK����1��һ��
	ASK_amp_ch2[1]=1023;//ASK����2�����
	AD9959_SetASK(CH2,ASK_amp_ch2,1000,0);//����ASK������CH2ͨ����A1=һ��,A2=���ֵ��Ƶ��1000hz����λ0(0��)
	
	ASK_amp_ch3[0]=500;//ASK����1��һ��
	ASK_amp_ch3[1]=1023;//ASK����2�����
	AD9959_SetASK(CH3,ASK_amp_ch3,1000,0);//����ASK������CH3ͨ����A1=һ��,A2=���ֵ��Ƶ��1000hz����λ0(0��)
	
	//ÿ��ͨ����ASK�������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������ΪASK����ģʽ����δ����ASK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3�ֱ����CH0-CH3���͵�ƽ���������1
		HAL_Delay(10);
		PS0_1;PS1_1;PS2_1;PS3_1;//�ߵ�ƽ���������2
		HAL_Delay(10);
	}
*/


//4ASK����
/*
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	
//	���ֻ��ͬʱ��2��ͨ�������ó�4��ƽ���ơ�
//	������ֻ��ѡ��CH0-1��δ������ͨ�����ݣ����巽����ο�AD9959оƬ�ֲ�22-23ҳ������FR1[14:12]Ϊ��Ӧֵ��
	AD9959_Modulation_Init(CH0|CH1,ASK,SWEEP_DISABLE,LEVEL_MOD_4);//ͨ��0-1����ΪASKģʽ������������ɨ�裬4��ƽ����
	
	ASK_amp_ch0[0]=255;//ASK����1���ķ�֮һ����Χ��0-1023��
	ASK_amp_ch0[1]=510;//ASK����2��
	ASK_amp_ch0[2]=765;//ASK����3��
	ASK_amp_ch0[3]=1023;//ASK����4�����
	AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//����ASK������CH0ͨ����д���ѹ���ȣ�Ƶ��1000hz����λ0(0��)
	
	ASK_amp_ch1[0]=255;//ASK����1���ķ�֮һ
	ASK_amp_ch1[1]=510;//ASK����2��
	ASK_amp_ch1[2]=765;//ASK����3��
	ASK_amp_ch1[3]=1023;//ASK����4�����
	AD9959_SetASK(CH1,ASK_amp_ch1,1000,0);//����ASK������CH1ͨ����д���ѹ���ȣ�Ƶ��1000hz����λ0(0��)
	
	//ÿ��ͨ����ASK�������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������ΪASK����ģʽ����δ����ASK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		
		PS0_0;PS1_0;//PS0��PS1�ֱ����CH0��CH0�������1
		PS2_0;PS3_0;//PS2��PS3�ֱ����CH1��CH1�������1
		HAL_Delay(10);
		
		PS0_0;PS1_1;//CH0�������2
		PS2_0;PS3_1;//CH1�������2
		HAL_Delay(10);
		
		PS0_1;PS1_0;//CH0�������3
		PS2_1;PS3_0;//CH1�������3
		HAL_Delay(10);
		
		PS0_1;PS1_1;//CH0�������4
		PS2_1;PS3_1;//CH1�������4
		HAL_Delay(10);
	}
*/


//8ASK����
/*
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	
//	���ֻ��ͬʱ��1��ͨ�������ó�8��ƽ���ơ�
//	������ֻ��ѡ��CH0��8��ƽ���ƣ�δ������ͨ�����ݣ����巽����ο�AD9959оƬ�ֲ�22-23ҳ������FR1[14:12]Ϊ��Ӧֵ��
	AD9959_Modulation_Init(CH0,ASK,SWEEP_DISABLE,LEVEL_MOD_8);//ͨ��0����ΪASKģʽ������������ɨ�裬8��ƽ����
	
	ASK_amp_ch0[0]=127;//ASK����1���˷�֮һ ��Χ��0-1023��
	ASK_amp_ch0[1]=255;//ASK����2���˷�֮��
	ASK_amp_ch0[2]=383;//ASK����3��..
	ASK_amp_ch0[3]=511;//ASK����4��..
	ASK_amp_ch0[4]=639;//ASK����5��..
	ASK_amp_ch0[5]=767;//ASK����6��..
	ASK_amp_ch0[6]=895;//ASK����7��..
	ASK_amp_ch0[7]=1023;//ASK����8�����
	AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//����ASK������CH0ͨ����д���ѹ���ȣ�Ƶ��1000hz����λ0(0��)

	//ÿ��ͨ����ASK�������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������ΪASK����ģʽ����δ����ASK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;//PS0-PS2����CH0��CH0�������1
		HAL_Delay(10);
		
		PS0_0;PS1_0;PS2_1;//CH0�������2
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_0;//CH0�������3
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_1;//CH0�������4
		HAL_Delay(10);		
		
		PS0_1;PS1_0;PS2_0;//CH0�������5
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_1;//CH0�������6
		HAL_Delay(10);
	
		PS0_1;PS1_1;PS2_0;//CH0�������7
		HAL_Delay(10);
		
		PS0_1;PS1_1;PS2_1;//CH0�������8
		HAL_Delay(10);
	}
 */

//16ASK����
/*	
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	
//	���ֻ��ͬʱ��1��ͨ�������ó�16��ƽ���ơ�
//	������ֻ��ѡ��CH0��16��ƽ���ƣ�δ������ͨ�����ݣ����巽����ο�AD9959оƬ�ֲ�22-23ҳ������FR1[14:12]Ϊ��Ӧֵ��
	AD9959_Modulation_Init(CH0,ASK,SWEEP_DISABLE,LEVEL_MOD_16);//ͨ��0����ΪASKģʽ������������ɨ�裬16��ƽ����
	
	ASK_amp_ch0[0]=64;//ASK����1��ʮ����֮һ����Χ��0-1023��
	ASK_amp_ch0[1]=128;//ASK����2��ʮ����֮��
	ASK_amp_ch0[2]=192;//ASK����3��...
	ASK_amp_ch0[3]=256;//ASK����4��...
	ASK_amp_ch0[4]=320;//ASK����5��...
	ASK_amp_ch0[5]=384;//ASK����6��...
	ASK_amp_ch0[6]=448;//ASK����7��...
	ASK_amp_ch0[7]=512;//ASK����8��...
	
	ASK_amp_ch0[8]=576;//ASK����9��...
	ASK_amp_ch0[9]=640;//ASK����10��...
	ASK_amp_ch0[10]=704;//ASK����11��...
	ASK_amp_ch0[11]=768;//ASK����12��...
	ASK_amp_ch0[12]=832;//ASK����13��...
	ASK_amp_ch0[13]=896;//ASK����14��...
	ASK_amp_ch0[14]=960;//ASK����15��...
	ASK_amp_ch0[15]=1023;//ASK����16�����
	
	AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//����FKS������CH0ͨ����д���ѹ���ȣ�Ƶ��1000hz����λ0(0��)
	
	//Ҳ�ɲ�������ͨ��������ΪASK����ģʽ����δ����ASK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3����CH0��CH0�������1
		HAL_Delay(10);
		
		PS0_0;PS1_0;PS2_0;PS3_1;//CH0�������2
		HAL_Delay(10);
		
		PS0_0;PS1_0;PS2_1;PS3_0;//CH0�������3
		HAL_Delay(10);
		
		PS0_0;PS1_0;PS2_1;PS3_1;//CH0�������4
		HAL_Delay(10);		
		
		PS0_0;PS1_1;PS2_0;PS3_0;//CH0�������5
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_0;PS3_1;//CH0�������6
		HAL_Delay(10);
	
		PS0_0;PS1_1;PS2_1;PS3_0;//CH0�������7
		HAL_Delay(10);
		
		PS0_0;PS1_1;PS2_1;PS3_1;//CH0�������8
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_0;PS3_0;//CH0�������9
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_0;PS3_1;//CH0�������10
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_1;PS3_0;//CH0�������11
		HAL_Delay(10);
		
		PS0_1;PS1_0;PS2_1;PS3_1;//CH0�������12
		HAL_Delay(10);		
		
		PS0_1;PS1_1;PS2_0;PS3_0;//CH0�������13
		HAL_Delay(10);
		
		PS0_1;PS1_1;PS2_0;PS3_1;//CH0�������14
		HAL_Delay(10);
	
		PS0_1;PS1_1;PS2_1;PS3_0;//CH0�������15
		HAL_Delay(10);
		
		PS0_1;PS1_1;PS2_1;PS3_1;//CH0�������16
		HAL_Delay(10);
	}
 */

//2PSK����
/*
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,PSK,SWEEP_DISABLE,LEVEL_MOD_2);//ͨ��0-3����ΪPSKģʽ������������ɨ�裬2��ƽ����
	PSK_phase_ch0[0]=0;//PSK��λ1��0,��Χ��0~16383(��Ӧ�Ƕȣ�0��~360��)
	PSK_phase_ch0[1]=4095;//PSK��λ2��90��
	AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//����PSK������CH0ͨ����P1=0��,P2=90�㣬Ƶ��1000hz
	
	PSK_phase_ch1[0]=0;//PSK��λ1��0
	PSK_phase_ch1[1]=4095;//PSK��λ2��90��
	AD9959_SetPSK(CH1,PSK_phase_ch1,1000);//����PSK������CH1ͨ����P1=0��,P2=90�㣬Ƶ��1000hz
	
	PSK_phase_ch2[0]=0;//PSK��λ1��0
	PSK_phase_ch2[1]=4095;//PSK��λ2��90��
	AD9959_SetPSK(CH2,PSK_phase_ch2,1000);//����PSK������CH2ͨ����P1=0��,P2=90�㣬Ƶ��1000hz
	
	PSK_phase_ch3[0]=0;//PSK��λ1��0
	PSK_phase_ch3[1]=4095;//PSK��λ2��90��
	AD9959_SetPSK(CH3,PSK_phase_ch3,1000);//����PSK������CH3ͨ����P1=0��,P2=90�㣬Ƶ��1000hz
	
	//ÿ��ͨ����PSK�������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������ΪPSK����ģʽ����δ����PSK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ���λ����λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3�ֱ����CH0-CH3���͵�ƽ�������λ1
		HAL_Delay(2);
		PS0_1;PS1_1;PS2_1;PS3_1;//�ߵ�ƽ�������λ2
		HAL_Delay(2);
	}
*/



//4PSK����
/*
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	
//	���ֻ��ͬʱ��2��ͨ�������ó�4��ƽ���ơ�
//	������ֻ��ѡ��CH0-1��δ������ͨ�����ݣ����巽����ο�AD9959оƬ�ֲ�22-23ҳ������FR1[14:12]Ϊ��Ӧֵ��
	AD9959_Modulation_Init(CH0|CH1,PSK,SWEEP_DISABLE,LEVEL_MOD_4);//ͨ��0-1����ΪPSKģʽ������������ɨ�裬4��ƽ����
	
	PSK_phase_ch0[0]=0;//PSK��λ1��0����Χ��0~16383(��Ӧ�Ƕȣ�0��~360��)
	PSK_phase_ch0[1]=4095;//PSK��λ2,90��
	PSK_phase_ch0[2]=8191;//PSK��λ3��180��
	PSK_phase_ch0[3]=12287;//PSK��λ4��270��
	AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//����PSK������CH0ͨ����д����λ��Ƶ��1000hz
	
	PSK_phase_ch1[0]=0;//PSK��λ1��0����Χ��0~16383(��Ӧ�Ƕȣ�0��~360��)
	PSK_phase_ch1[1]=4095;//PSK��λ2,90��
	PSK_phase_ch1[2]=8191;//PSK��λ3��180��
	PSK_phase_ch1[3]=12287;//PSK��λ4��270��
	AD9959_SetPSK(CH1,PSK_phase_ch1,1000);//����PSK������CH1ͨ����д����λ��Ƶ��1000hz
	
	//ÿ��ͨ����PSK�������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������ΪPSK����ģʽ����δ����PSK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		
		PS0_0;PS1_0;//PS0��PS1�ֱ����CH0��CH0�����λ1
		PS2_0;PS3_0;//PS2��PS3�ֱ����CH1��CH1�����λ1
		HAL_Delay(2);
		
		PS0_0;PS1_1;//CH0�����λ2
		PS2_0;PS3_1;//CH1�����λ2
		HAL_Delay(2);
		
		PS0_1;PS1_0;//CH0�����λ3
		PS2_1;PS3_0;//CH1�����λ3
		HAL_Delay(2);
		
		PS0_1;PS1_1;//CH0�����λ4
		PS2_1;PS3_1;//CH1�����λ4
		HAL_Delay(2);
	}
*/


//8PSK����
/*
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	
//	���ֻ��ͬʱ��1��ͨ�������ó�8��ƽ���ơ�
//	������ֻ��ѡ��CH0��8��ƽ���ƣ�δ������ͨ�����ݣ����巽����ο�AD9959оƬ�ֲ�22-23ҳ������FR1[14:12]Ϊ��Ӧֵ��
	AD9959_Modulation_Init(CH0,PSK,SWEEP_DISABLE,LEVEL_MOD_8);//ͨ��0����ΪPSKģʽ������������ɨ�裬8��ƽ����
	
	PSK_phase_ch0[0]=0;//PSK��λ1��0��Χ��0~16383(��Ӧ�Ƕȣ�0��~360��)
	PSK_phase_ch0[1]=2047;//PSK��λ2��45��
	PSK_phase_ch0[2]=4095;//PSK��λ3��90��
	PSK_phase_ch0[3]=6143;//PSK��λ4��..
	PSK_phase_ch0[4]=8191;//PSK��λ5��..
	PSK_phase_ch0[5]=10239;//PSK��λ6��..
	PSK_phase_ch0[6]=10239;//PSK��λ7��..
	PSK_phase_ch0[7]=14335;//PSK��λ8��315��
	AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//����PSK������CH0ͨ����д����λ��Ƶ��1000hz

	//ÿ��ͨ����PSK�������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������ΪPSK����ģʽ����δ����PSK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		
		PS0_0;PS1_0;PS2_0;//PS0-PS2����CH0��CH0�����λ1
		HAL_Delay(2);
		
		PS0_0;PS1_0;PS2_1;//CH0�����λ2
		HAL_Delay(2);
		
		PS0_0;PS1_1;PS2_0;//CH0�����λ3
		HAL_Delay(2);
		
		PS0_0;PS1_1;PS2_1;//CH0�����λ4
		HAL_Delay(2);		
		
		PS0_1;PS1_0;PS2_0;//CH0�����λ5
		HAL_Delay(2);
		
		PS0_1;PS1_0;PS2_1;//CH0�����λ6
		HAL_Delay(2);
	
		PS0_1;PS1_1;PS2_0;//CH0�����λ7
		HAL_Delay(2);
		
		PS0_1;PS1_1;PS2_1;//CH0�����λ8
		HAL_Delay(2);
	}
*/


//16PSK����
 /*
	AD9959_Init();	//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	
//	���ֻ��ͬʱ��1��ͨ�������ó�16��ƽ���ơ�
//	������ֻ��ѡ��CH0��16��ƽ���ƣ�δ������ͨ�����ݣ����巽����ο�AD9959оƬ�ֲ�22-23ҳ������FR1[14:12]Ϊ��Ӧֵ��
	AD9959_Modulation_Init(CH0,PSK,SWEEP_DISABLE,LEVEL_MOD_16);//ͨ��0����ΪPSKģʽ������������ɨ�裬16��ƽ����
	
	PSK_phase_ch0[0]=0;//PSK��λ1��0����Χ��0~16383(��Ӧ�Ƕȣ�0��~360��)
	PSK_phase_ch0[1]=1024;//PSK��λ2��22.5��
	PSK_phase_ch0[2]=2048;//PSK��λ3��45��
	PSK_phase_ch0[3]=3072;//PSK��λ4��67.5��
	PSK_phase_ch0[4]=4096;//PSK��λ5��90��
	PSK_phase_ch0[5]=5120;//PSK��λ6��...
	PSK_phase_ch0[6]=6144;//PSK��λ7��...
	PSK_phase_ch0[7]=7168;//PSK��λ8��...
	
	PSK_phase_ch0[8]=8192;//PSK��λ9��...
	PSK_phase_ch0[9]=9216;//PSK��λ10��...
	PSK_phase_ch0[10]=10240;//PSK��λ11��...
	PSK_phase_ch0[11]=11264;//PSK��λ12��...
	PSK_phase_ch0[12]=12288;//PSK��λ13��...
	PSK_phase_ch0[13]=13312;//PSK��λ14��...
	PSK_phase_ch0[14]=14336;//PSK��λ15��...
	PSK_phase_ch0[15]=15360;//PSK��λ16��337.5��
	
	AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//����PSK������CH1ͨ����д����λ��Ƶ��1000hz
	
	//Ҳ�ɲ�������ͨ��������ΪPSK����ģʽ����δ����PSK����ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��
	
	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3����CH0��CH0�����λ1
		HAL_Delay(2);
		
		PS0_0;PS1_0;PS2_0;PS3_1;//CH0�����λ2
		HAL_Delay(2);
		
		PS0_0;PS1_0;PS2_1;PS3_0;//CH0�����λ3
		HAL_Delay(2);
		
		PS0_0;PS1_0;PS2_1;PS3_1;//CH0�����λ4
		HAL_Delay(2);		
		
		PS0_0;PS1_1;PS2_0;PS3_0;//CH0�����λ5
		HAL_Delay(2);
		
		PS0_0;PS1_1;PS2_0;PS3_1;//CH0�����λ6
		HAL_Delay(2);
	
		PS0_0;PS1_1;PS2_1;PS3_0;//CH0�����λ7
		HAL_Delay(2);
		
		PS0_0;PS1_1;PS2_1;PS3_1;//CH0�����λ8
		HAL_Delay(2);
		
		PS0_1;PS1_0;PS2_0;PS3_0;//CH0�����λ9
		HAL_Delay(2);
		
		PS0_1;PS1_0;PS2_0;PS3_1;//CH0�����λ10
		HAL_Delay(2);
		
		PS0_1;PS1_0;PS2_1;PS3_0;//CH0�����λ11
		HAL_Delay(2);
		
		PS0_1;PS1_0;PS2_1;PS3_1;//CH0�����λ12
		HAL_Delay(2);		
		
		PS0_1;PS1_1;PS2_0;PS3_0;//CH0�����λ13
		HAL_Delay(2);
		
		PS0_1;PS1_1;PS2_0;PS3_1;//CH0�����λ14
		HAL_Delay(2);
	
		PS0_1;PS1_1;PS2_1;PS3_0;//CH0�����λ15
		HAL_Delay(2);
		
		PS0_1;PS1_1;PS2_1;PS3_1;//CH0�����λ16
		HAL_Delay(2);
	}
*/


//ģʽ�� liner sweep mode(�漰����չio��)
// ɨƵ
/*	
	AD9959_Init();									//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	
	//����Ƶ������ɨ�裬Nlevelֻ����LEVEL_MOD_2
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,FSK,SWEEP_ENABLE,LEVEL_MOD_2);//ͨ��0-3����ΪFSKģʽ����������ɨ�裬2��ƽ����
	
										//	ͨ����						��ʼƵ�ʣ�����Ƶ�ʣ���������Ƶ�ʣ��½�����Ƶ�ʣ�����б�ʣ��½�б�ʣ����� ����λ
	AD9959_SetFre_Sweep(CH0|CH1|CH2|CH3,		1000, 100000,			1,							1,				250,		250,			1023,		0);
	//��������Ч��Ϊ����1000Hzɨ�赽100000Hz������1hz��ɨƵʱ��Լ198ms���ٴ�100000Hzɨ�赽1000Hz������1hz��ɨƵʱ��Լ198ms��ɨƵʱ�����ο�����ע��
	
	//ÿ��ͨ����Ƶ������ɨ��������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������ΪƵ������ɨ��ģʽ����δ����Ƶ������ɨ��ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��

	IO_Update();	//AD9959��������,���ô˺���������������Ч��������

	while (1)
	{
	 	PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3�ֱ����CH0-CH3���ߵ�ƽ������ʼֵɨ������ֵ
		HAL_Delay(198);
		PS0_0;PS1_0;PS2_0;PS3_0;//�͵�ƽ���ӽ���ֵɨ����ʼֵ
		HAL_Delay(198);
	}      
*/



//ɨ��
/*	
	AD9959_Init();									//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���
	
	//����Ƶ������ɨ�裬Nlevelֻ����LEVEL_MOD_2
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,ASK,SWEEP_ENABLE,LEVEL_MOD_2);//ͨ��0-3����ΪASKģʽ����������ɨ�裬2��ƽ����
	
										//	ͨ����						��ʼ���ȣ��������ȣ������������ȣ��½��������ȣ�����б�ʣ��½�б�ʣ�Ƶ�� ����λ
	AD9959_SetAmp_Sweep(CH0|CH1|CH2|CH3,		0, 				1023,			1,							1,				250,		250,	10000,	0);
	//��������Ч��Ϊ���������Ƶ��10000hz������ʼ����ɨ�赽�������ȣ�����1�����ȿ����֣�ɨƵʱ��Լ2ms���ٴӽ�������ɨ�赽��ʼ���ȣ�����1�����ȿ����֣�ɨƵʱ��Լ2ms��ɨƵʱ�����ο�����ע��
	
	//ÿ��ͨ���ķ�������ɨ��������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������Ϊ��������ɨ��ģʽ����δ���÷�������ɨ��ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��

	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3�ֱ����CH0-CH3���ߵ�ƽ������ʼֵɨ������ֵ
		HAL_Delay(2);
		PS0_0;PS1_0;PS2_0;PS3_0;//�͵�ƽ���ӽ���ֵɨ����ʼֵ
		HAL_Delay(2);
	}    
*/


//ɨ��
/*
	AD9959_Init();									//��ʼ������AD9959��Ҫ�õ���IO��,���Ĵ���

	//������λ����ɨ�裬Nlevelֻ����LEVEL_MOD_2
	AD9959_Modulation_Init(CH0|CH1|CH2|CH3,PSK,SWEEP_ENABLE,LEVEL_MOD_2);//ͨ��0-3����ΪPSKģʽ����������ɨ�裬2��ƽ����
	
										//	ͨ����						��ʼ��λ��������λ������������λ���½�������λ������б�ʣ��½�б�ʣ�Ƶ�� ������
	AD9959_SetPhase_Sweep(CH0|CH1|CH2|CH3,				0, 		16383,			1,							1,				250,		250,	10000,	1023);
	//��������Ч��Ϊ���������Ƶ��10000hz������ʼ��λɨ�赽������λ������1����λ�����֣�ɨƵʱ��Լ33ms���ٴӽ�����λɨ�赽��ʼ��λ������1����λ�����֣�ɨƵʱ��Լ33ms��ɨƵʱ�����ο�����ע��
	//ֻ������������Ƶ����ͬ���źţ�������λ���ԡ�
	
	//ÿ��ͨ������λ����ɨ��������Բ�ͬ���������ü��ɡ�
	//Ҳ�ɲ�������ͨ��������Ϊ��λ����ɨ��ģʽ����δ������λ����ɨ��ģʽ��ͨ������ʹ��AD9959_Set_Fre�Ⱥ�������������Ƶ�ʡ����ȡ���λ;�����ó���������ģʽ��

	IO_Update();	//AD9959��������,���ô˺���������������Ч��������
	
	while(1)
	{
		PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3�ֱ����CH0-CH3���ߵ�ƽ������ʼֵɨ������ֵ
		HAL_Delay(33);
		PS0_0;PS1_0;PS2_0;PS3_0;//�͵�ƽ���ӽ���ֵɨ����ʼֵ
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
				AD9959_Set_Fre(CH0, 100000);	//����ͨ��0Ƶ��100000Hz
				AD9959_Set_Fre(CH1, 200000);	//����ͨ��1Ƶ��100000Hz
				AD9959_Set_Fre(CH2, 100000);	//����ͨ��2Ƶ��100000Hz
				AD9959_Set_Fre(CH3, 100000);	//����ͨ��3Ƶ��100000Hz
		
				AD9959_Set_Amp(CH0, 1023); 		//����ͨ��0���ȿ���ֵ1023����Χ0~1023
				AD9959_Set_Amp(CH1, 1023); 		//����ͨ��1���ȿ���ֵ1023����Χ0~1023
				AD9959_Set_Amp(CH2, 1023); 		//����ͨ��2���ȿ���ֵ1023����Χ0~1023
				AD9959_Set_Amp(CH3, 1023); 		//����ͨ��3���ȿ���ֵ1023����Χ0~1023

				AD9959_Set_Phase(CH0, 0);			//����ͨ��0��λ����ֵ0(0��)����Χ0~16383
				AD9959_Set_Phase(CH1, 4096);	//����ͨ��1��λ����ֵ4096(90��)����Χ0~16383
				AD9959_Set_Phase(CH2, 8192);	//����ͨ��2��λ����ֵ8192(180��)����Χ0~16383
				AD9959_Set_Phase(CH3, 12288);	//����ͨ��3��λ����ֵ12288(270��)����Χ0~16383 
				IO_Update();	//AD9959��������,���ô˺���������������Ч��������                    
				break;
			}
			case 1:
			{
				OLED_ShowStr(20,3,(unsigned char *)"2FSK",2); 
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,FSK,SWEEP_DISABLE,LEVEL_MOD_2);//ͨ��0-3����ΪFSKģʽ������������ɨ�裬2��ƽ����
				FSK_fre_ch0[0]=1000;//FSKƵ��1��1000Hz
				FSK_fre_ch0[1]=200000;//FSKƵ��2��200000Hz
				AD9959_SetFSK(CH0,FSK_fre_ch0,0);//����FKS������CH0ͨ����F1=1000Hz,F2=200000Hz����λ0(0��)
	
				FSK_fre_ch1[0]=1000;//FSKƵ��1��1000Hz
				FSK_fre_ch1[1]=200000;//FSKƵ��2��200000Hz
				AD9959_SetFSK(CH1,FSK_fre_ch1,0);//����FKS������CH1ͨ����F1=1000Hz,F2=200000Hz����λ0(0��)
	
				FSK_fre_ch2[0]=1000;//FSKƵ��1��1000Hz
				FSK_fre_ch2[1]=200000;//FSKƵ��2��200000Hz
				AD9959_SetFSK(CH2,FSK_fre_ch2,0);//����FKS������CH2ͨ����F1=1000Hz,F2=200000Hz����λ0(0��)
	
				FSK_fre_ch3[0]=1000;//FSKƵ��1��1000Hz
				FSK_fre_ch3[1]=200000;//FSKƵ��2��200000Hz
				AD9959_SetFSK(CH3,FSK_fre_ch3,0);//����FKS������CH3ͨ����F1=1000Hz,F2=200000Hz����λ0(0��)        
	
				IO_Update();
				break;
			}
			case 2:
			{
				OLED_ShowStr(20,3,(unsigned char *)"4FSK",2); 
				AD9959_Modulation_Init(CH0|CH1,FSK,SWEEP_DISABLE,LEVEL_MOD_4);//ͨ��0-1����ΪFSKģʽ������������ɨ�裬4��ƽ����
	
				FSK_fre_ch0[0]=100;//FSKƵ��1��100Hz
				FSK_fre_ch0[1]=1000;//FSKƵ��2��1000Hz
				FSK_fre_ch0[2]=10000;//FSKƵ��3��10000Hz
				FSK_fre_ch0[3]=100000;//FSKƵ��4��100000Hz
				AD9959_SetFSK(CH0,FSK_fre_ch0,0);//����FKS������CH0ͨ����F1=100Hz,F2=1000Hz��F3=10000Hz,F4=100000Hz����λ0(0��)
	
				FSK_fre_ch1[0]=100;//FSKƵ��1��100Hz
				FSK_fre_ch1[1]=1000;//FSKƵ��2��1000Hz
				FSK_fre_ch1[2]=10000;//FSKƵ��3��10000Hz
				FSK_fre_ch1[3]=100000;//FSKƵ��4��100000Hz
				AD9959_SetFSK(CH1,FSK_fre_ch1,0);//����FKS������CH1ͨ����F1=100Hz,F2=1000Hz��F3=10000Hz,F4=100000Hz����λ0(0��)
	
				IO_Update();
				break;
			}
			case 3:
			{
				OLED_ShowStr(20,3,(unsigned char *)"8FSK",2);
				AD9959_Modulation_Init(CH0,FSK,SWEEP_DISABLE,LEVEL_MOD_8);//ͨ��0����ΪFSKģʽ������������ɨ�裬8��ƽ����
	
				FSK_fre_ch0[0]=100;//FSKƵ��1��100Hz
				FSK_fre_ch0[1]=500;//FSKƵ��2��500Hz
				FSK_fre_ch0[2]=1000;//FSKƵ��3��1000Hz
				FSK_fre_ch0[3]=1500;//FSKƵ��4��1500Hz
				FSK_fre_ch0[4]=2000;//FSKƵ��5��2000Hz
				FSK_fre_ch0[5]=2500;//FSKƵ��6��2500Hz
				FSK_fre_ch0[6]=3000;//FSKƵ��7��3000Hz
				FSK_fre_ch0[7]=3500;//FSKƵ��8��3500Hz
				AD9959_SetFSK(CH0,FSK_fre_ch0,0);//����FKS������CH0ͨ����F1=100Hz,F2=500Hz��F3=1000Hz,F4=1500Hz.........����λ0(0��)
	
				IO_Update();	//AD9959��������,���ô˺���������������Ч��������
				break;
			}
			case 4:
			{
				OLED_ShowStr(20,3,(unsigned char *)"16FSK",2);
				AD9959_Modulation_Init(CH0,FSK,SWEEP_DISABLE,LEVEL_MOD_16);//ͨ��0����ΪFSKģʽ������������ɨ�裬16��ƽ����
	
				FSK_fre_ch0[0]=10000;//FSKƵ��1��10000Hz
				FSK_fre_ch0[1]=20000;//FSKƵ��2��20000Hz
				FSK_fre_ch0[2]=50000;//FSKƵ��3��50000Hz
				FSK_fre_ch0[3]=80000;//FSKƵ��4��80000Hz
				FSK_fre_ch0[4]=120000;//FSKƵ��5��120000Hz
				FSK_fre_ch0[5]=140000;//FSKƵ��6��140000Hz
				FSK_fre_ch0[6]=180000;//FSKƵ��7��180000Hz
				FSK_fre_ch0[7]=200000;//FSKƵ��8��200000Hz
	
				FSK_fre_ch0[8]=250000;//FSKƵ��9��...
				FSK_fre_ch0[9]=270000;//FSKƵ��10��...
				FSK_fre_ch0[10]=300000;//FSKƵ��11��...
				FSK_fre_ch0[11]=320000;//FSKƵ��12��...
				FSK_fre_ch0[12]=350000;//FSKƵ��13��...
				FSK_fre_ch0[13]=400000;//FSKƵ��14��...
				FSK_fre_ch0[14]=600000;//FSKƵ��15��...
				FSK_fre_ch0[15]=10000000;//FSKƵ��16��...
	
				AD9959_SetFSK(CH0,FSK_fre_ch0,0);//����FKS������CH0ͨ����F1=10000Hz,F2=20000Hz��F3=50000Hz........����λ0(0��)	
				IO_Update();	
				break;
			}
			case 5:
			{
				OLED_ShowStr(20,3,(unsigned char *)"2ASK",2);
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,ASK,SWEEP_DISABLE,LEVEL_MOD_2);//ͨ��0-3����ΪASKģʽ������������ɨ�裬2��ƽ����
				ASK_amp_ch0[0]=500;//ASK����1��һ��,��Χ��0-1023��
				ASK_amp_ch0[1]=1023;//ASK����2�����
				AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//����ASK������CH0ͨ����A1=һ��,A2=���ֵ��Ƶ��1000hz����λ0(0��)
	
				ASK_amp_ch1[0]=500;//ASK����1��һ��
				ASK_amp_ch1[1]=1023;//ASK����2�����
				AD9959_SetASK(CH1,ASK_amp_ch1,1000,0);//����ASK������CH1ͨ����A1=һ��,A2=���ֵ��Ƶ��1000hz����λ0(0��)
	
				ASK_amp_ch2[0]=500;//ASK����1��һ��
				ASK_amp_ch2[1]=1023;//ASK����2�����
				AD9959_SetASK(CH2,ASK_amp_ch2,1000,0);//����ASK������CH2ͨ����A1=һ��,A2=���ֵ��Ƶ��1000hz����λ0(0��)
	
				ASK_amp_ch3[0]=500;//ASK����1��һ��
				ASK_amp_ch3[1]=1023;//ASK����2�����
				AD9959_SetASK(CH3,ASK_amp_ch3,1000,0);//����ASK������CH3ͨ����A1=һ��,A2=���ֵ��Ƶ��1000hz����λ0(0��)

				IO_Update();	
				break;
			}
			case 6:
			{
				OLED_ShowStr(20,3,(unsigned char *)"4ASK",2);
				AD9959_Modulation_Init(CH0|CH1,ASK,SWEEP_DISABLE,LEVEL_MOD_4);//ͨ��0-1����ΪASKģʽ������������ɨ�裬4��ƽ����
	
				ASK_amp_ch0[0]=255;//ASK����1���ķ�֮һ����Χ��0-1023��
				ASK_amp_ch0[1]=510;//ASK����2��
				ASK_amp_ch0[2]=765;//ASK����3��
				ASK_amp_ch0[3]=1023;//ASK����4�����
				AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//����ASK������CH0ͨ����д���ѹ���ȣ�Ƶ��1000hz����λ0(0��)
	
				ASK_amp_ch1[0]=255;//ASK����1���ķ�֮һ
				ASK_amp_ch1[1]=510;//ASK����2��
				ASK_amp_ch1[2]=765;//ASK����3��
				ASK_amp_ch1[3]=1023;//ASK����4�����
				AD9959_SetASK(CH1,ASK_amp_ch1,1000,0);//����ASK������CH1ͨ����д���ѹ���ȣ�Ƶ��1000hz����λ0(0��)

	
				IO_Update();	
				break;
			}
			case 7:
			{
				OLED_ShowStr(20,3,(unsigned char *)"8ASK",2);
				AD9959_Modulation_Init(CH0,ASK,SWEEP_DISABLE,LEVEL_MOD_8);//ͨ��0����ΪASKģʽ������������ɨ�裬8��ƽ����
	
				ASK_amp_ch0[0]=127;//ASK����1���˷�֮һ ��Χ��0-1023��
				ASK_amp_ch0[1]=255;//ASK����2���˷�֮��
				ASK_amp_ch0[2]=383;//ASK����3��..
				ASK_amp_ch0[3]=511;//ASK����4��..
				ASK_amp_ch0[4]=639;//ASK����5��..
				ASK_amp_ch0[5]=767;//ASK����6��..
				ASK_amp_ch0[6]=895;//ASK����7��..
				ASK_amp_ch0[7]=1023;//ASK����8�����
				AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//����ASK������CH0ͨ����д���ѹ���ȣ�Ƶ��1000hz����λ0(0��)
	
				IO_Update();	
				break;
			}
			case 8:
			{
				OLED_ShowStr(20,3,(unsigned char *)"16ASK",2);
				AD9959_Modulation_Init(CH0,ASK,SWEEP_DISABLE,LEVEL_MOD_16);//ͨ��0����ΪASKģʽ������������ɨ�裬16��ƽ����
	
				ASK_amp_ch0[0]=64;//ASK����1��ʮ����֮һ����Χ��0-1023��
				ASK_amp_ch0[1]=128;//ASK����2��ʮ����֮��
				ASK_amp_ch0[2]=192;//ASK����3��...
				ASK_amp_ch0[3]=256;//ASK����4��...
				ASK_amp_ch0[4]=320;//ASK����5��...
				ASK_amp_ch0[5]=384;//ASK����6��...
				ASK_amp_ch0[6]=448;//ASK����7��...
				ASK_amp_ch0[7]=512;//ASK����8��...
	
				ASK_amp_ch0[8]=576;//ASK����9��...
				ASK_amp_ch0[9]=640;//ASK����10��...
				ASK_amp_ch0[10]=704;//ASK����11��...
				ASK_amp_ch0[11]=768;//ASK����12��...
				ASK_amp_ch0[12]=832;//ASK����13��...
				ASK_amp_ch0[13]=896;//ASK����14��...
				ASK_amp_ch0[14]=960;//ASK����15��...
				ASK_amp_ch0[15]=1023;//ASK����16�����
	
				AD9959_SetASK(CH0,ASK_amp_ch0,1000,0);//����FKS������CH0ͨ����д���ѹ���ȣ�Ƶ��1000hz����λ0(0��)	
				IO_Update();
				break;
			}
			case 9:
			{
				OLED_ShowStr(20,3,(unsigned char *)"2PSK",2);
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,PSK,SWEEP_DISABLE,LEVEL_MOD_2);//ͨ��0-3����ΪPSKģʽ������������ɨ�裬2��ƽ����
				PSK_phase_ch0[0]=0;//PSK��λ1��0,��Χ��0~16383(��Ӧ�Ƕȣ�0��~360��)
				PSK_phase_ch0[1]=4095;//PSK��λ2��90��
				AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//����PSK������CH0ͨ����P1=0��,P2=90�㣬Ƶ��1000hz
	
				PSK_phase_ch1[0]=0;//PSK��λ1��0
				PSK_phase_ch1[1]=4095;//PSK��λ2��90��
				AD9959_SetPSK(CH1,PSK_phase_ch1,1000);//����PSK������CH1ͨ����P1=0��,P2=90�㣬Ƶ��1000hz
	
				PSK_phase_ch2[0]=0;//PSK��λ1��0
				PSK_phase_ch2[1]=4095;//PSK��λ2��90��
				AD9959_SetPSK(CH2,PSK_phase_ch2,1000);//����PSK������CH2ͨ����P1=0��,P2=90�㣬Ƶ��1000hz
	
				PSK_phase_ch3[0]=0;//PSK��λ1��0
				PSK_phase_ch3[1]=4095;//PSK��λ2��90��
				AD9959_SetPSK(CH3,PSK_phase_ch3,1000);//����PSK������CH3ͨ����P1=0��,P2=90�㣬Ƶ��1000hz
		
				IO_Update();	
				break;
			}
			case 10:
			{
				OLED_ShowStr(20,3,(unsigned char *)"4PSK",2);
				AD9959_Modulation_Init(CH0|CH1,PSK,SWEEP_DISABLE,LEVEL_MOD_4);//ͨ��0-1����ΪPSKģʽ������������ɨ�裬4��ƽ����
	
				PSK_phase_ch0[0]=0;//PSK��λ1��0����Χ��0~16383(��Ӧ�Ƕȣ�0��~360��)
				PSK_phase_ch0[1]=4095;//PSK��λ2,90��
				PSK_phase_ch0[2]=8191;//PSK��λ3��180��
				PSK_phase_ch0[3]=12287;//PSK��λ4��270��
				AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//����PSK������CH0ͨ����д����λ��Ƶ��1000hz
	
				PSK_phase_ch1[0]=0;//PSK��λ1��0����Χ��0~16383(��Ӧ�Ƕȣ�0��~360��)
				PSK_phase_ch1[1]=4095;//PSK��λ2,90��
				PSK_phase_ch1[2]=8191;//PSK��λ3��180��
				PSK_phase_ch1[3]=12287;//PSK��λ4��270��

				AD9959_SetPSK(CH1,PSK_phase_ch1,1000);//����PSK������CH1ͨ����д����λ��Ƶ��1000hz
				IO_Update();
				break;
			}
			case 11:
			{
				OLED_ShowStr(20,3,(unsigned char *)"8PSK",2);
				AD9959_Modulation_Init(CH0,PSK,SWEEP_DISABLE,LEVEL_MOD_8);//ͨ��0����ΪPSKģʽ������������ɨ�裬8��ƽ����
	
				PSK_phase_ch0[0]=0;//PSK��λ1��0��Χ��0~16383(��Ӧ�Ƕȣ�0��~360��)
				PSK_phase_ch0[1]=2047;//PSK��λ2��45��
				PSK_phase_ch0[2]=4095;//PSK��λ3��90��
				PSK_phase_ch0[3]=6143;//PSK��λ4��..
				PSK_phase_ch0[4]=8191;//PSK��λ5��..
				PSK_phase_ch0[5]=10239;//PSK��λ6��..
				PSK_phase_ch0[6]=10239;//PSK��λ7��..
				PSK_phase_ch0[7]=14335;//PSK��λ8��315��
				
				AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//����PSK������CH0ͨ����д����λ��Ƶ��1000hz
				IO_Update();
				break;
			}
			case 12:
			{
				OLED_ShowStr(20,3,(unsigned char *)"16PSK",2);
				AD9959_Modulation_Init(CH0,PSK,SWEEP_DISABLE,LEVEL_MOD_16);
	
				PSK_phase_ch0[0]=0;//PSK��λ1��0����Χ��0~16383(��Ӧ�Ƕȣ�0��~360��)
				PSK_phase_ch0[1]=1024;//PSK��λ2��22.5��
				PSK_phase_ch0[2]=2048;//PSK��λ3��45��
				PSK_phase_ch0[3]=3072;//PSK��λ4��67.5��
				PSK_phase_ch0[4]=4096;//PSK��λ5��90��
				PSK_phase_ch0[5]=5120;//PSK��λ6��...
				PSK_phase_ch0[6]=6144;//PSK��λ7��...
				PSK_phase_ch0[7]=7168;//PSK��λ8��...
	
				PSK_phase_ch0[8]=8192;//PSK��λ9��...
				PSK_phase_ch0[9]=9216;//PSK��λ10��...
				PSK_phase_ch0[10]=10240;//PSK��λ11��...
				PSK_phase_ch0[11]=11264;//PSK��λ12��...
				PSK_phase_ch0[12]=12288;//PSK��λ13��...
				PSK_phase_ch0[13]=13312;//PSK��λ14��...
				PSK_phase_ch0[14]=14336;//PSK��λ15��...
				PSK_phase_ch0[15]=15360;//PSK��λ16��337.5��
	
				AD9959_SetPSK(CH0,PSK_phase_ch0,1000);//����PSK������CH1ͨ����д����λ��Ƶ��1000hz
				IO_Update();
				break;
			}
			case 13:
			{
				OLED_ShowStr(0,3,(unsigned char *)"Sweep Frequency",2);
					//����Ƶ������ɨ�裬Nlevelֻ����LEVEL_MOD_2
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,FSK,SWEEP_ENABLE,LEVEL_MOD_2);//ͨ��0-3����ΪFSKģʽ����������ɨ�裬2��ƽ����
	
										//	ͨ����					��ʼ��λ,   ������λ,  ����������λ,         �½�������λ�� ����б��,�½�б�� ,Ƶ�� ,   ����
				AD9959_SetFre_Sweep(CH0|CH1|CH2|CH3,		1000, 100000,			1,							1,				250,		250,			1023,		0);
				IO_Update();
				break;
			}
			case 14:
			{
				OLED_ShowStr(0,3,(unsigned char *)"Sweep Ampilitude",2);
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,ASK,SWEEP_ENABLE,LEVEL_MOD_2);
		   								//	ͨ����					��ʼ��λ,   ������λ,  ����������λ,         �½�������λ�� ����б��,�½�б�� ,Ƶ�� ,   ����
				AD9959_SetAmp_Sweep(CH0|CH1|CH2|CH3,		0, 				1023,			1,							1,				250,		250,	10000,	0);
				IO_Update();
				break;
			}
			case 15:
			{
				OLED_ShowStr(0,3,(unsigned char *)"Sweep Phase",2);
				AD9959_Modulation_Init(CH0|CH1|CH2|CH3,PSK,SWEEP_ENABLE,LEVEL_MOD_2);
												//	ͨ����					��ʼ��λ,   ������λ,  ����������λ,         �½�������λ�� ����б��,�½�б�� ,Ƶ�� ,   ����
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
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3�ֱ����CH0-CH3���͵�ƽ�����Ƶ��1
				HAL_Delay(10);
				PS0_1;PS1_1;PS2_1;PS3_1;//�ߵ�ƽ�����Ƶ��2
				HAL_Delay(10);           
				break;
			}
			case 2:
			{
				PS0_0;PS1_0;//PS0��PS1�ֱ����CH0��CH0���Ƶ��1
				PS2_0;PS3_0;//PS2��PS3�ֱ����CH1��CH1���Ƶ��1
				HAL_Delay(10);
		
				PS0_0;PS1_1;//CH0���Ƶ��2
				PS2_0;PS3_1;//CH1���Ƶ��2
				HAL_Delay(10);
		
				PS0_1;PS1_0;//CH0���Ƶ��3
				PS2_1;PS3_0;//CH1���Ƶ��3
				HAL_Delay(10);
		
				PS0_1;PS1_1;//CH0���Ƶ��4
				PS2_1;PS3_1;//CH1���Ƶ��4
				HAL_Delay(10);
				break;
			}
			case 3:
			{
				PS0_0;PS1_0;PS2_0;//PS0-PS2����CH0��CH0���Ƶ��1
				HAL_Delay(10);
		
				PS0_0;PS1_0;PS2_1;//CH0���Ƶ��2
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_0;//CH0���Ƶ��3
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_1;//CH0���Ƶ��4
				HAL_Delay(10);		
		
				PS0_1;PS1_0;PS2_0;//CH0���Ƶ��5
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_1;//CH0���Ƶ��6
				HAL_Delay(10);
	
				PS0_1;PS1_1;PS2_0;//CH0���Ƶ��7
				HAL_Delay(10);
		
				PS0_1;PS1_1;PS2_1;//CH0���Ƶ��8
				HAL_Delay(10);
				break;
			}
			case 4:
			{
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3����CH0��CH0���Ƶ��1
				delay_us(15);
		
				PS0_0;PS1_0;PS2_0;PS3_1;//CH0���Ƶ��2
				delay_us(15);
		
				PS0_0;PS1_0;PS2_1;PS3_0;//CH0���Ƶ��3
				delay_us(15);
		
				PS0_0;PS1_0;PS2_1;PS3_1;//CH0���Ƶ��4
				delay_us(15);		
		
				PS0_0;PS1_1;PS2_0;PS3_0;//CH0���Ƶ��5
				delay_us(15);
		
				PS0_0;PS1_1;PS2_0;PS3_1;//CH0���Ƶ��6
				delay_us(15);
	
				PS0_0;PS1_1;PS2_1;PS3_0;//CH0���Ƶ��7
				delay_us(15);
		
				PS0_0;PS1_1;PS2_1;PS3_1;//CH0���Ƶ��8
				delay_us(15);
		
				PS0_1;PS1_0;PS2_0;PS3_0;//CH0���Ƶ��9
				delay_us(15);
		
				PS0_1;PS1_0;PS2_0;PS3_1;//CH0���Ƶ��10
				delay_us(15);
		
				PS0_1;PS1_0;PS2_1;PS3_0;//CH0���Ƶ��11
				delay_us(15);
		
				PS0_1;PS1_0;PS2_1;PS3_1;//CH0���Ƶ��12
				delay_us(15);		
		
				PS0_1;PS1_1;PS2_0;PS3_0;//CH0���Ƶ��13
				delay_us(15);
		
				PS0_1;PS1_1;PS2_0;PS3_1;//CH0���Ƶ��14
				delay_us(15);
	
				PS0_1;PS1_1;PS2_1;PS3_0;//CH0���Ƶ��15
				delay_us(15);
		
				PS0_1;PS1_1;PS2_1;PS3_1;//CH0���Ƶ��16
				delay_us(15);
				break;
			}
			case 5:
			{
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3�ֱ����CH0-CH3���͵�ƽ���������1
				HAL_Delay(10);
				PS0_1;PS1_1;PS2_1;PS3_1;//�ߵ�ƽ���������2
				HAL_Delay(10);
				break;
			}
			case 6:
			{
				PS0_0;PS1_0;//PS0��PS1�ֱ����CH0��CH0�������1
				PS2_0;PS3_0;//PS2��PS3�ֱ����CH1��CH1�������1
				HAL_Delay(10);
		
				PS0_0;PS1_1;//CH0�������2
				PS2_0;PS3_1;//CH1�������2
				HAL_Delay(10);
		
				PS0_1;PS1_0;//CH0�������3
				PS2_1;PS3_0;//CH1�������3
				HAL_Delay(10);
		
				PS0_1;PS1_1;//CH0�������4
				PS2_1;PS3_1;//CH1�������4
				HAL_Delay(10);
				break;
			}
			case 7:
			{		
				PS0_0;PS1_0;PS2_0;//PS0-PS2����CH0��CH0�������1
				HAL_Delay(10);
		
				PS0_0;PS1_0;PS2_1;//CH0�������2
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_0;//CH0�������3
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_1;//CH0�������4
				HAL_Delay(10);		
		
				PS0_1;PS1_0;PS2_0;//CH0�������5
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_1;//CH0�������6
				HAL_Delay(10);
	
				PS0_1;PS1_1;PS2_0;//CH0�������7
				HAL_Delay(10);
			
				PS0_1;PS1_1;PS2_1;//CH0�������8
				HAL_Delay(10);
				break;
			}
			case 8:
			{
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3����CH0��CH0�������1
				HAL_Delay(10);
		
				PS0_0;PS1_0;PS2_0;PS3_1;//CH0�������2
				HAL_Delay(10);
		
				PS0_0;PS1_0;PS2_1;PS3_0;//CH0�������3
				HAL_Delay(10);
		
				PS0_0;PS1_0;PS2_1;PS3_1;//CH0�������4
				HAL_Delay(10);		
		
				PS0_0;PS1_1;PS2_0;PS3_0;//CH0�������5
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_0;PS3_1;//CH0�������6
				HAL_Delay(10);
	
				PS0_0;PS1_1;PS2_1;PS3_0;//CH0�������7
				HAL_Delay(10);
		
				PS0_0;PS1_1;PS2_1;PS3_1;//CH0�������8
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_0;PS3_0;//CH0�������9
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_0;PS3_1;//CH0�������10
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_1;PS3_0;//CH0�������11
				HAL_Delay(10);
		
				PS0_1;PS1_0;PS2_1;PS3_1;//CH0�������12
				HAL_Delay(10);		
		
				PS0_1;PS1_1;PS2_0;PS3_0;//CH0�������13
				HAL_Delay(10);
		
				PS0_1;PS1_1;PS2_0;PS3_1;//CH0�������14
				HAL_Delay(10);
	
				PS0_1;PS1_1;PS2_1;PS3_0;//CH0�������15
				HAL_Delay(10);
		
				PS0_1;PS1_1;PS2_1;PS3_1;//CH0�������16
				HAL_Delay(10);
				break;
			}
			case 9:
			{
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3�ֱ����CH0-CH3���͵�ƽ�������λ1
				HAL_Delay(2);
				PS0_1;PS1_1;PS2_1;PS3_1;//�ߵ�ƽ�������λ2
				HAL_Delay(2);
				break;
			}
			case 10:
			{
				PS0_0;PS1_0;//PS0��PS1�ֱ����CH0��CH0�����λ1
				PS2_0;PS3_0;//PS2��PS3�ֱ����CH1��CH1�����λ1
				HAL_Delay(2);
		
				PS0_0;PS1_1;//CH0�����λ2
				PS2_0;PS3_1;//CH1�����λ2
				HAL_Delay(2);
		
				PS0_1;PS1_0;//CH0�����λ3
				PS2_1;PS3_0;//CH1�����λ3
				HAL_Delay(2);
		
				PS0_1;PS1_1;//CH0�����λ4
				PS2_1;PS3_1;//CH1�����λ4
				HAL_Delay(2);
				break;
			}
			case 11:
			{
				PS0_0;PS1_0;PS2_0;//PS0-PS2����CH0��CH0�����λ1
				HAL_Delay(2);
		
				PS0_0;PS1_0;PS2_1;//CH0�����λ2
				HAL_Delay(2);
		
				PS0_0;PS1_1;PS2_0;//CH0�����λ3
				HAL_Delay(2);
		
				PS0_0;PS1_1;PS2_1;//CH0�����λ4
				HAL_Delay(2);		
		
				PS0_1;PS1_0;PS2_0;//CH0�����λ5
				HAL_Delay(2);
		
				PS0_1;PS1_0;PS2_1;//CH0�����λ6
				HAL_Delay(2);
			
				PS0_1;PS1_1;PS2_0;//CH0�����λ7
				HAL_Delay(2);
		
				PS0_1;PS1_1;PS2_1;//CH0�����λ8
				HAL_Delay(2);
				break;
			}
			case 12:
			{
				PS0_0;PS1_0;PS2_0;PS3_0;//PS0-PS3����CH0��CH0�����λ1
				HAL_Delay(2);
		
				PS0_0;PS1_0;PS2_0;PS3_1;//CH0�����λ2
				HAL_Delay(2);
		
				PS0_0;PS1_0;PS2_1;PS3_0;//CH0�����λ3
				HAL_Delay(2);
		
				PS0_0;PS1_0;PS2_1;PS3_1;//CH0�����λ4
				HAL_Delay(2);		
			
				PS0_0;PS1_1;PS2_0;PS3_0;//CH0�����λ5
				HAL_Delay(2);
		
				PS0_0;PS1_1;PS2_0;PS3_1;//CH0�����λ6
				HAL_Delay(2);
	
				PS0_0;PS1_1;PS2_1;PS3_0;//CH0�����λ7
				HAL_Delay(2);
		
				PS0_0;PS1_1;PS2_1;PS3_1;//CH0�����λ8
				HAL_Delay(2);
		
				PS0_1;PS1_0;PS2_0;PS3_0;//CH0�����λ9
				HAL_Delay(2);
		
				PS0_1;PS1_0;PS2_0;PS3_1;//CH0�����λ10
				HAL_Delay(2);
		
				PS0_1;PS1_0;PS2_1;PS3_0;//CH0�����λ11
				HAL_Delay(2);
		
				PS0_1;PS1_0;PS2_1;PS3_1;//CH0�����λ12
				HAL_Delay(2);		
		
				PS0_1;PS1_1;PS2_0;PS3_0;//CH0�����λ13
				HAL_Delay(2);
		
				PS0_1;PS1_1;PS2_0;PS3_1;//CH0�����λ14
				HAL_Delay(2);
	
				PS0_1;PS1_1;PS2_1;PS3_0;//CH0�����λ15
				HAL_Delay(2);
		
				PS0_1;PS1_1;PS2_1;PS3_1;//CH0�����λ16
				HAL_Delay(2);
				break;
			}
			case 13:
			{
				PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3�ֱ����CH0-CH3���ߵ�ƽ������ʼֵɨ������ֵ
				HAL_Delay(198);
				PS0_0;PS1_0;PS2_0;PS3_0;//�͵�ƽ���ӽ���ֵɨ����ʼֵ
				HAL_Delay(198);
				break;
			}
			case 14:
			{
				PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3�ֱ����CH0-CH3���ߵ�ƽ������ʼֵɨ������ֵ
				HAL_Delay(2);
				PS0_0;PS1_0;PS2_0;PS3_0;//�͵�ƽ���ӽ���ֵɨ����ʼֵ
				HAL_Delay(2);
				break;
			}
			case 15:
			{
				PS0_1;PS1_1;PS2_1;PS3_1;//PS0-PS3�ֱ����CH0-CH3���ߵ�ƽ������ʼֵɨ������ֵ
				HAL_Delay(33);
				PS0_0;PS1_0;PS2_0;PS3_0;//�͵�ƽ���ӽ���ֵɨ����ʼֵ
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
