#ifndef _USART1_H
#define _USART1_H
#include "main.h"
#include "usart.h"
#include "stdio.h"

#define USART1_MAX_LEN 100

//�����ֽڡ��к��ַ���
//����Writexx����������0��ʾfalse���ú��������õ���if������װ
//����ReadChar����������ǻ����������һ����ȡ���ַ���
//����Readxx���������ಿ�֣������д��str[]���ַ�����

int fputc(int ch, FILE *f);
void USART1_Init(UART_HandleTypeDef* huart);
void USART1_WriteChar(uint8_t ch);
uint8_t USART1_Write(uint8_t *str, uint16_t len);
uint8_t USART1_WriteStr(uint8_t *str);                    //������\0��
uint8_t USART1_WriteLine(uint8_t *str);                   //�ַ�����\0���ᱻ���ͣ������������\r\n��β��ĩβû��\0��
uint8_t USART1_WriteUntil(uint8_t *str, uint8_t endChar); //�����ڴ����Զ���EndChar��MyUSART1_WriteStr����
uint8_t USART1_ReadChar(void);
uint8_t USART1_PeekChar(void);
uint32_t USART1_Read(uint8_t *str, uint16_t maxLen);

uint8_t USART1_CanReadLine(void);
uint8_t USART1_CanReadUntil(uint16_t endChar);
uint8_t USART1_CanReadStr(void);

//����ReadStr������ReadUntil������ReadLine���������EndSequence��ƥ�䣬���ǽ�����0��bufferPosҲ������ġ�
uint32_t USART1_ReadStr(uint8_t *str);
uint32_t USART1_ReadUntil(uint8_t *str, uint16_t endChar);
uint32_t USART1_ReadLine(uint8_t *str);
uint32_t USART1_ReadAll(uint8_t *str);
void __USART1_Shift(uint32_t len);
void __USART1_DumpAll(void);
void USART1_ClearBuffer(void);

void My_USART1_IRQHandler(USART_TypeDef *source);

#endif

/*
��stm32fxxx_it.c/stm32hxxx_it.c/stm32g4xx_it.c�ļ���

1.��һ��
��USER CODE BEGIN Includes��
���
#include "USART\USART1.h"

2.�ڶ���
�ں���
USARTx_IRQHandler()�����HAL_UART_IRQHandler()֮ǰ���
My_USART1_IRQHandler(USARTx);���

�ǵ���CUBEMX�д򿪴����ж�
*/
