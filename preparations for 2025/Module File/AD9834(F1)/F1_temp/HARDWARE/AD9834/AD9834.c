#include "AD9834.h"
/*
��������AD9833_Write_Data

��  �ܣ���AD9834д��16λ����

��  ����data -- 16λ����

����ֵ����
*/
void AD9834_Write_Data(unsigned int data)
{
	unsigned char i =0;
	
	AD9834_SCLK_SET;
	AD9834_FSYNC_CLR;
	
	for(i= 0;i < 16;	i++)
	{
		if(data & 0x8000)
			AD9834_SDATA_SET;
		else
			AD9834_SDATA_CLR;
		
		AD9834_SCLK_CLR;
		data <<= 1;
		AD9834_SCLK_SET;
	}
	AD9834_SDATA_SET;
	AD9834_FSYNC_SET;
}

/*
��������AD9834_Init

��  �ܣ�AD9834��ʼ��

��  ������

����ֵ����
*/

void AD9834_Init(void)
{
	AD9834_FS_CLR;
	AD9833_PS_CLR;
	
	AD9834_Write_Data(0x2100);
	AD9834_Write_Data(0x2038);//AD9834_Write_Data(0x2038);
	//AD9834_Write_Data(0XC000);
//	AD9834_Write_Data(0x2100);
//AD9834_Write_Data(0X00C0);
}



/*
��������AD9834_Select_Wave

��  �ܣ�����ѡ��		����ΪʲôҪ��������RESET��λ

��  ����initdata -- ��������

����ֵ����
*/

void AD9834_Select_Wave(unsigned int initdata) 

{ 

    AD9834_FSYNC_SET; 

    AD9834_SCLK_SET; 

     

    AD9834_RESET_SET; 

    AD9834_RESET_SET; 

    AD9834_RESET_CLR;


    AD9834_Write_Data(initdata); 

} 

/*
��������AD9834_Set_Freq

��  �ܣ�Ƶ��д��

��  ����freq_number -- Ƶ�ʼĴ���ѡ��
				freq -- Ƶ��ֵ

����ֵ����
*/

void AD9834_Set_Freq(unsigned char freq_number, unsigned long freq) 
{
    unsigned long FREQREG = (unsigned long)(268435456.0/AD9834_MCLK*freq); 
    unsigned int FREQREG_LSB_14BIT = (unsigned int)FREQREG; 
    unsigned int FREQREG_MSB_14BIT = (unsigned int)(FREQREG>>14); 
	
    if(freq_number == FREQ_0) 
    { 
        FREQREG_LSB_14BIT &= ~(1U<<15); 
        FREQREG_LSB_14BIT |= 1<<14; 
        FREQREG_MSB_14BIT &= ~(1U<<15); 
        FREQREG_MSB_14BIT |= 1<<14; 
    }
    else
    {
        FREQREG_LSB_14BIT &= ~(1<<14); 
        FREQREG_LSB_14BIT |= 1U<<15; 
        FREQREG_MSB_14BIT &= ~(1<<14); 
        FREQREG_MSB_14BIT |= 1U<<15; 
    } 
    AD9834_Write_Data(FREQREG_LSB_14BIT); 
    AD9834_Write_Data(FREQREG_MSB_14BIT);      
} 
