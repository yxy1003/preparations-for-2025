#ifndef _AD9833_H
#define _AD9833_H

#include "main.h"

#define Sine_Wave     0X2008	//���Ҳ�
#define Triangle_Wave 0x2002	//���ǲ�
#define Square_Wave   0x2028	//����

#define AD9834_MCLK 75000000UL	//AD9834����Ƶ��

//���ƽ�A3-A6��FS��PS��B0,B1ֱ���õͣ��ɿ���λ����

#define AD9834_FSYNC_SET			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)

#define AD9834_FSYNC_CLR   			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)

#define AD9834_SCLK_SET				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET) 

#define AD9834_SCLK_CLR				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET) 

#define AD9834_SDATA_SET   			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET) 

#define AD9834_SDATA_CLR   			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET)

#define AD9834_RESET_SET   			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET) 

#define AD9834_RESET_CLR   			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET)

#define AD9834_FS_SET       		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)

#define AD9834_FS_CLR      	 		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)

#define AD9833_PS_SET				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)

#define AD9833_PS_CLR				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)



//Ƶ�ʼĴ���
#define FREQ_0      0 

#define FREQ_1      1 

/* AD9834�������� */ 
	
void AD9834_Write_Data(unsigned int data);			 //д16λ����

void AD9834_Select_Wave(unsigned int initdata) ;   //ѡ��������� 

void AD9834_Init(void);  													 //��ʼ������ 

void AD9834_Set_Freq(unsigned char freq_number, unsigned long freq) ;		//ѡ������Ĵ��������Ƶ��ֵ 
	
#endif
