#include "DAC8830.h"
uint16_t send_data=0;

/**
	初始化DAC8830的接口
 */
void dac8830_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = SPI2_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI2_CS_GPIO_Port, &GPIO_InitStruct);
	
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
	发送数据给dac8830
 */
void dac8830_send_byte(float data)
{
	send_data=(uint16_t)(data*26214.0f);
  	if (HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_READY)
	{
		HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi2,(uint8_t*)&send_data,1,1000);
		HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_SET);
	
	} 
}





