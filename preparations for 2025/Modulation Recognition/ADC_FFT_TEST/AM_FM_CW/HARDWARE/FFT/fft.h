#ifndef _FFT_H
#define _FFT_H

#include "main.h"
#include "delay.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "arm_math.h"
#include "myusart1.h"
#include "OLED_I2C.h"

void 	ADC1_Get(uint32_t FRE);//开始采样
void 	Fre_Control(uint32_t Fre);//采样频率
void 	Get_amp(uint16_t *amp,uint16_t *vmax,uint16_t *vmin);//峰峰值最大值最小值
void 	Get_vmax(uint16_t *vmax);//最大值
void 	Get_vmin(uint16_t *vmin);//最小值
void 	fft_spectrogram_judge(void);//频谱分析

#endif
