#ifndef _MYIIC_H
#define _MYIIC_H
#include "main.h"

#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//����ģʽ
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //���ģʽ

//����G4�ٷ���ֱ�� OLED_12864
#if defined (STM32G4)
    #define IIC_SCL_0 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9, GPIO_PIN_RESET)
    #define IIC_SCL_1 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9, GPIO_PIN_SET)
    #define IIC_SDA_0 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_RESET)
    #define IIC_SDA_1 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_SET)
    #define READ_SDA  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8)
#endif
#if defined (STM32F407xx)  
    #define IIC_SCL_0 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_RESET)
    #define IIC_SCL_1 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_SET)
    #define IIC_SDA_0 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9, GPIO_PIN_RESET)
    #define IIC_SDA_1 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9, GPIO_PIN_SET)
    #define READ_SDA  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)
#endif

void IIC_Init(void);                      //��ʼ��IIC��IO��				 
void IIC_Start(void);				              //����IIC��ʼ�ź�
void IIC_Stop(void);	  			            //����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			    //IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack); //IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				      //IIC�ȴ�ACK�ź�
void IIC_Ack(void);					              //IIC����ACK�ź�
void IIC_NAck(void);				              //IIC������ACK�ź�

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 
#endif

