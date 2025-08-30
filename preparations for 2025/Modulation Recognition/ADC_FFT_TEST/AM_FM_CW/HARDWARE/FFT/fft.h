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

void 	ADC1_Get(uint32_t FRE);//��ʼ����
void 	Fre_Control(uint32_t Fre);//����Ƶ��
void 	Get_amp(uint16_t *amp,uint16_t *vmax,uint16_t *vmin);//���ֵ���ֵ��Сֵ
void 	Get_vmax(uint16_t *vmax);//���ֵ
void 	Get_vmin(uint16_t *vmin);//��Сֵ
void 	fft_spectrogram_judge(void);//Ƶ�׷���

#endif
