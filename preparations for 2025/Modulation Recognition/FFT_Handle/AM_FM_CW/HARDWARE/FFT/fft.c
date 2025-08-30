#include "FFT.h"
#include "window.h"

#define  FFT_LENGTH		 1024 				//FFT长度，默认是1024点FFT
#define  filter_cnt		 5 				//FFT长度，默认是1024点FFT	
uint32_t FRE;//采样频率
uint32_t Fn;//信号频率
uint16_t cnt;
uint16_t vmax;
uint16_t vmin;
uint16_t amp;

uint16_t ADC_GetValue	[FFT_LENGTH] = {0};
uint16_t line_idex [FFT_LENGTH]   = {0};
uint32_t filter_values_32[filter_cnt]={0};
uint16_t filter_values_16[filter_cnt]={0};
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
	HAL_TIM_Base_Start(&htim3);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_GetValue, FFT_LENGTH); 
}

void Get_amp(uint16_t *amp,uint16_t *vmax,uint16_t *vmin)
{
	uint16_t i;
	(*vmax)=0,(*vmin)=4096;
	for(i=0;i<FFT_LENGTH-1;i++)
	{
		if(ADC_GetValue[i]>(*vmax))
			(*vmax)=ADC_GetValue[i];
		if(ADC_GetValue[i]<(*vmin))
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
		if(ADC_GetValue[i]>(*vmax))
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
		if(ADC_GetValue[i]<(*vmin))
			(*vmin)=ADC_GetValue[i];
	}
	OLED_ShowInt(60,3,(*vmin),2);
}

uint32_t filtered_average_u32(uint32_t* values) 
{
	 uint16_t i;
    uint32_t max_val = values[0];
    uint32_t min_val = values[0];
    uint32_t sum = values[0];
    
    // 记录最大值和最小值的首次出现位置
    uint32_t max_index = 0;
    uint32_t min_index = 0;
    
    // 第一轮遍历：找出最大值和最小值
    for (i = 1; i < filter_cnt; i++) 
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
    return (uint32_t)((double)sum/(double)(filter_cnt - 2));
}

uint16_t filtered_average_u16(uint16_t* values) 
{
	 uint16_t i;
    uint16_t max_val = values[0];
    uint16_t min_val = values[0];
    uint16_t sum = values[0];
    
    // 记录最大值和最小值的首次出现位置
    uint16_t max_index = 0;
    uint16_t min_index = 0;
    
    // 第一轮遍历：找出最大值和最小值
    for (i = 1; i < filter_cnt; i++) 
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
    return (uint16_t)((double)sum/(double)(filter_cnt - 2));
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
	OLED_ShowFloat(60,5,max,2);
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
	
	arm_cfft_radix4_f32(&scfft,fft_inputbuf);		
	arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);		//把运算结果复数求模得幅值 	

	//fft_window(fft_outputbuf);//加窗
	fft_spectrogram(fft_outputbuf,&cnt,&Fn);
	for (i=0;i<filter_cnt;i++)
	{
		Get_amp(&amp,&vmax,&vmin);
		filter_values_16[i]=vmax;
	}
	filtered_average_u16(filter_values_16);
	
	Get_amp(&amp,&vmax,&vmin);

	arm_max_f32(fft_outputbuf+1,FFT_LENGTH/2 - 1,&max,&max_idex);
	
	if(max < 20.0f)
		max = 10000000000;

}

void fft_window(float *fft_outputbuf)
{
	for(int i=0;i<FFT_LENGTH;i++)
	{
		fft_outputbuf[i] = Kaiser_1024[i] * fft_outputbuf[i]; // 加窗
	}
}

   
/**
 * 生成 Blackman 窗
 * @param window 输出数组（float 类型）
 * @param N      窗口长度（如 FFT 点数）
 */
void generate_blackman_window(float *window, int N) {
    for (int n = 0; n < N; n++) {
        float a = 2.0f * PI * n / (N - 1);
        window[n] = 0.42f - 0.5f * cosf(a) + 0.08f * cosf(2.0f * a);
    }
}
/**
 * @brief 生成平顶窗函数
 *
 * 根据给定的窗口大小N，生成一个长度为N的平顶窗函数，并将其存储在window数组中。
 *
 * @param window 存储生成的平顶窗函数的数组
 * @param N 平顶窗函数的长度
 */
void generate_flattop_window(float *window, int N) {
    const float a0 = 1.0f;
    const float a1 = 1.93f;
    const float a2 = 1.29f;
    const float a3 = 0.388f;
    const float a4 = 0.028f;

    for (int n = 0; n < N; n++) {
        float angle = 2.0f * PI * n / (N - 1);
        window[n] = a0
                  - a1 * cosf(angle)
                  + a2 * cosf(2 * angle)
                  - a3 * cosf(3 * angle)
                  + a4 * cosf(4 * angle);
    }
}

/**
 * 生成汉宁窗（Hanning Window）
 * @param window  输出窗口数组
 * @param N       窗口长度（一般为 FFT 长度）
 */
void generate_hanning_window(float *window, int N) {
    for (int n = 0; n < N; n++) {
        window[n] = 0.5f * (1.0f - cosf(2.0f * PI * n / (N - 1)));
    }
}

#include <math.h>

/**
 * 生成 Hamming 窗
 * @param window 输出数组（float 类型）
 * @param N      窗口长度（如 FFT 点数）
 */
void generate_hamming_window(float *window, int N) {
    for (int n = 0; n < N; n++) {
        window[n] = 0.54f - 0.46f * cosf(2.0f * PI * n / (N - 1));
    }
}

/**
  * @brief  执行二次拉格朗日插值
  * @param  input: 指向原始输入数据数组的指针
  * @param  output: 指向存储插值结果的输出数组指针
  * @param  in_size: 输入数组的点数
  * @param  out_size: 期望的输出数组点数
  * @retval 无
  * @note   使用三点拉格朗日插值公式：
  *         P(x) = y0*(x-x1)(x-x2)/((x0-x1)(x0-x2)) + 
  *                y1*(x-x0)(x-x2)/((x1-x0)(x1-x2)) +
  *                y2*(x-x0)(x-x1)/((x2-x0)(x2-x1))
  */
 void lagrange_interpolate(uint32_t *input, float *output, int in_size, int out_size)
{
    float scale = (float)(in_size - 1) / (out_size - 1);

    for (int i = 0; i < out_size; i++) {
        float x = i * scale;

        int x0 = (int)x;
        int x1 = x0 + 1;
        int x2 = x0 + 2;

        // 边界保护，确保不越界
        if (x2 >= in_size) {
            // 如果越界，退化为最近点复制
            output[i] = (float)input[in_size - 1];
            continue;
        }

        // 获取 y 值
        float y0 = (float)input[x0];
        float y1 = (float)input[x1];
        float y2 = (float)input[x2];

        float denom0 = (x0 - x1) * (x0 - x2);
        float denom1 = (x1 - x0) * (x1 - x2);
        float denom2 = (x2 - x0) * (x2 - x1);

        // 检查除数是否为0，避免除零
        if (fabs(denom0) < 1e-6 || fabs(denom1) < 1e-6 || fabs(denom2) < 1e-6) {
            // 降阶处理：线性插值或直接取 y1
            output[i] = y1;
        } else {
            float L0 = ((x - x1)*(x - x2)) / denom0;
            float L1 = ((x - x0)*(x - x2)) / denom1;
            float L2 = ((x - x0)*(x - x1)) / denom2;
            output[i] = y0 * L0 + y1 * L1 + y2 * L2;
        }
    }
}

