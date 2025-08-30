#include "FFT.h"

#define  FFT_LENGTH		 1024 				//FFT长度，默认是1024点FFT	
uint32_t FRE;//采样频率
uint32_t Fn;//信号频率
uint16_t cnt;
uint16_t vmax;
uint16_t vmin;
uint16_t amp;

uint16_t ADC_GetValue	[FFT_LENGTH] = {0};
uint16_t line_idex [FFT_LENGTH]   = {0};	
float fft_inputbuf [FFT_LENGTH*2] = {0};			//FFT输入数组
float fft_outputbuf[FFT_LENGTH]   = {0};			//FFT输出数组

arm_cfft_radix4_instance_f32 scfft;

void Fre_Control(uint32_t Fre)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

	uint32_t MaxData;
	uint16_t div=1;	
	while( (SystemCoreClock/Fre/div)>65535 )
	{
		div++;
	}
	MaxData =  SystemCoreClock/Fre/div - 1;	
	
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = div-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = MaxData;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

void ADC1_Get(uint32_t FRE)
{
	Fre_Control(FRE);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_GetValue, FFT_LENGTH); 
}

void Get_amp(uint16_t *amp,uint16_t *vmax,uint16_t *vmin)
{
	uint16_t i;
	(*vmax)=0,(*vmin)=4096;
	for(i=0;i<FFT_LENGTH-1;i++)
	{
		if(ADC_GetValue[i]>*vmax)
			(*vmax)=ADC_GetValue[i];
		if(ADC_GetValue[i]<*vmin)
			(*vmin)=ADC_GetValue[i];
	}
	(*amp)=(*vmax)-(*vmin);
	OLED_ShowInt(10,3,(*vmax),2);
	OLED_ShowInt(60,3,(*vmin),2);
}



void Get_vmax(uint16_t *vmax)
{
	uint16_t i;
	(*vmax)=0;
	for(i=0;i<FFT_LENGTH-1;i++)
	{
		if(ADC_GetValue[i]>*vmax)
			(*vmax)=ADC_GetValue[i];
	}
	OLED_ShowInt(10,3,(*vmax),2);
}
void Get_vmin(uint16_t *vmin)
{
	uint16_t i;
	(*vmin)=4096;
	for(i=0;i<FFT_LENGTH-1;i++)
	{
		if(ADC_GetValue[i]<*vmin)
			(*vmin)=ADC_GetValue[i];
	}
	OLED_ShowInt(30,3,(*vmin),2);
}

uint32_t filtered_average_u32(uint32_t* values, uint32_t count) 
{
	 uint16_t i;
    uint32_t max_val = values[0];
    uint32_t min_val = values[0];
    uint32_t sum = values[0];
    
    // 记录最大值和最小值的首次出现位置
    uint32_t max_index = 0;
    uint32_t min_index = 0;
    
    // 第一轮遍历：找出最大值和最小值
    for (i = 1; i < count; i++) 
	{
        sum += values[i];
        if (values[i] > max_val) 
		  {
            max_val = values[i];
            max_index = i;
        }
        else if (values[i] < min_val) 
		  {		
            min_val = values[i];
            min_index = i;
        }
    }
    if (max_index == min_index) 
	 {
        return values[0];  // 直接返回任意值
    }
    sum = sum - max_val - min_val;
    return (uint32_t)((double)sum/(double)(count - 2));
}

void fft_spectrogram(float* input,uint16_t *cnt,uint32_t *Fn)
{	
	float max;	
	
	uint16_t i=0;
	uint32_t max_idex = 0;
	(*Fn)=0;
	(*cnt)=0;

	arm_max_f32(input+1,FFT_LENGTH/2,&max,&max_idex);
	
	for(i=1;i<FFT_LENGTH/2;i++)
	{
		if(input[i] > 0.08f * max) //基准值选择
		{
			line_idex	[(*cnt)] = i;
			(*cnt) ++ ;
		}			
	}	
	(*Fn) = line_idex[0] * (FRE / FFT_LENGTH);

	//显示
	OLED_CLS();  
	OLED_ShowInt(10,0,(*cnt),2);	
	OLED_ShowInt(30,0,(*Fn),2);
}
	
void fft_spectrogram_judge(void)
{
	uint16_t i;	
	float max;
	uint32_t max_idex = 0;
	
	arm_cfft_radix4_instance_f32 scfft;
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);

	for(i=0;i<FFT_LENGTH;i++)//生成信号序列
	{
		fft_inputbuf[2*i] = (float)ADC_GetValue[ i ]*3.3f/4095.0f;	//实部为ADC采样值
		fft_inputbuf[2*i+1]=0;										//虚部全部为0
	}
	/*for(i=0;i<FFT_LENGTH;i++)
	{
		printf("%d\r\n",ADC_GetValue[ i ]);
	}	*/
	arm_cfft_radix4_f32(&scfft,fft_inputbuf);		
	arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);		//把运算结果复数求模得幅值 	
	
	fft_spectrogram(fft_outputbuf,&cnt,&Fn);
	Get_amp(&amp,&vmax,&vmin);

	arm_max_f32(fft_outputbuf+1,FFT_LENGTH/2 - 1,&max,&max_idex);
	
	if(max < 20.0f)
		max = 10000000000;
	for(i=1;i<FFT_LENGTH/2;i++)
	{
		printf("%f\r\n",fft_outputbuf[i]);
	}		
}
