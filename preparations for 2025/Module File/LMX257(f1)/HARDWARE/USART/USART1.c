#include "USART1.h"

uint8_t 	USART1_buffer[USART1_MAX_LEN];
uint32_t 	USART1_bufferPos = 0;
uint8_t 	USART1_bufferOverflowFlag = 0;
UART_HandleTypeDef *USART1_dummyHandle;

struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       

//重定义fputc函数 
int fputc(int ch, FILE *f)
{
  while (!__HAL_UART_GET_FLAG(USART1_dummyHandle, UART_FLAG_TC))
    ;
#if defined(STM32H750xx) || defined(STM32L431xx)|| defined(STM32G4)
  USART1->TDR = (uint8_t)ch;
#endif
#if defined(STM32F407xx) || defined(STM32F103xB) 
  USART1->DR = (uint8_t)ch;
#endif
  return ch;
}

void USART1_Init(UART_HandleTypeDef *huart)
{
  USART1_dummyHandle = huart;
  __HAL_UART_ENABLE_IT(USART1_dummyHandle, UART_IT_RXNE);
  ;
}

void USART1_WriteChar(uint8_t ch)
{
  while (!__HAL_UART_GET_FLAG(USART1_dummyHandle, UART_FLAG_TC))
    ;
#if defined(STM32H750xx) || defined(STM32L431xx)|| defined(STM32G4)
  USART1->TDR = ch;
#endif
#if defined(STM32F407xx) || defined(STM32F103xB)
  USART1->DR = ch;
#endif
}

uint8_t USART1_Write(uint8_t *str, uint16_t len)
{
  if (len > USART1_MAX_LEN)
    return 0;
  for (uint32_t i = 0; i < len; i++)
  {
    USART1_WriteChar(*(str + i));
  }
  return len;
}
uint8_t USART1_WriteStr(uint8_t *str) 		//将发送\0；
{
  return USART1_WriteUntil(str, '\0');
}

uint8_t USART1_WriteLine(uint8_t *str) 		//字符串的\0不会被发送，传输的数据以\r\n结尾（末尾没有\0）
{
  uint32_t i;
  for (i = 0; i < USART1_MAX_LEN; i++)
  {
    if (*(str + i) == '\0')
      break;
    USART1_WriteChar(*(str + i));
  }
  USART1_WriteChar('\r');
  USART1_WriteChar('\n');
  return i + 2 < USART1_MAX_LEN;
}

uint8_t USART1_WriteUntil(uint8_t *str, uint8_t endChar) //类似于带有自定义EndChar的MyUSART1_WriteStr（）
{
  uint32_t i;
  for (i = 0; i < USART1_MAX_LEN; i++)
  {
    USART1_WriteChar(*(str + i));
    if (*(str + i) == endChar)
      break;
  }
  return i < USART1_MAX_LEN;
}

uint8_t USART1_ReadChar(void)
{
  if (USART1_bufferPos != 0)
    USART1_bufferPos--;
  else
    USART1_buffer[0] = 0;
  return USART1_buffer[USART1_bufferPos];
}

uint8_t USART1_PeekChar(void)
{
  if (USART1_bufferPos == 0)
    return USART1_buffer[USART1_MAX_LEN - 1];
  return USART1_buffer[USART1_bufferPos - 1];
}

uint8_t USART1_CanReadLine(void)
{
  uint32_t i;
  for (i = 1; i < USART1_bufferPos; i++)
  {
    if (USART1_buffer[i - 1] == '\r' && USART1_buffer[i] == '\n')
      return 1;
  }
  return 0;
}

uint8_t USART1_CanReadUntil(uint16_t endChar)
{
  uint32_t i;
  for (i = 0; i < USART1_bufferPos; i++)
  {
    if (USART1_buffer[i] == endChar)
      return 1;
  }
  return 0;
}

uint8_t USART1_CanReadStr()
{
  return USART1_CanReadUntil('\0');
}

uint32_t USART1_Read(uint8_t *str, uint16_t maxLen)
{
  maxLen = maxLen < USART1_bufferPos ? maxLen : USART1_bufferPos;
  uint32_t i;
  for (i = 0; i < maxLen; i++)
  {
    *(str + i) = USART1_buffer[i];
  }
  __USART1_Shift(maxLen);
  return maxLen;
}

uint32_t USART1_ReadStr(uint8_t *str)
{
  return USART1_ReadUntil(str, '\0');
}

uint32_t USART1_ReadLine(uint8_t *str)
{
  if (!USART1_CanReadLine())
    return 0;
  uint32_t i;
  for (i = 0; i < USART1_bufferPos; i++)
  {
    *(str + i) = USART1_buffer[i];
    if (i > 0 && *(str + i - 1) == '\r' && *(str + i) == '\n')
    {
      i++;
      break;
    }
  }
  __USART1_Shift(i);
  return i;
}

uint32_t USART1_ReadUntil(uint8_t *str, uint16_t endChar)
{
  if (!USART1_CanReadUntil(endChar))
    return 0;
  uint32_t i;
  for (i = 0; i < USART1_bufferPos; i++)
  {
    *(str + i) = USART1_buffer[i];
    if (*(str + i) == endChar)
    {
      i++;
      break;
    }
  }
  __USART1_Shift(i);
  return i;
}

uint32_t USART1_ReadAll(uint8_t *str)
{
  uint32_t i;
  for (i = 0; i < USART1_bufferPos; i++)
  {
    *(str + i) = USART1_buffer[i];
  }
  __USART1_Shift(i);
  return i;
}

void __USART1_Shift(uint32_t len)
{
  for (uint32_t i = 0; i < USART1_MAX_LEN - len; i++)
  {
    USART1_buffer[i] = USART1_buffer[i + len];
  }
  USART1_bufferPos -= len;
}

void __USART1_DumpAll(void)
{
  if (USART1_MAX_LEN > 100)
    return;
  USART1_WriteChar(USART1_bufferPos);
  for (uint32_t i = 0; i < USART1_MAX_LEN; i++)
  {
    USART1_WriteChar(USART1_buffer[i]);
  }
}

void USART1_ClearBuffer(void)
{
  USART1_bufferPos = 0;
}

void My_USART1_IRQHandler(USART_TypeDef *source)
{

  if (!__HAL_UART_GET_FLAG(USART1_dummyHandle, UART_FLAG_RXNE))
    return;
#if defined(STM32H750xx) || defined(STM32L431xx)|| defined(STM32G4)
  USART1_buffer[USART1_bufferPos++] = USART1->RDR;
#endif
#if defined(STM32F407xx) || defined(STM32F103xB)
  USART1_buffer[USART1_bufferPos++] = USART1->DR;
#endif
  if (USART1_bufferPos == USART1_MAX_LEN)
  {
    USART1_bufferPos = 0;
    USART1_bufferOverflowFlag = 1;
  }
}
