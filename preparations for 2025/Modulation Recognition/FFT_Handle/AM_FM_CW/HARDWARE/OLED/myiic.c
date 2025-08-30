#include "myiic.h"
#include "delay.h"
	
//IIC��ʼ��
void IIC_Init(void)
{
//    GPIO_InitTypeDef GPIO_Initure;
//    
//    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��
//    
//    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
//    GPIO_Initure.Pull=GPIO_PULLUP;          //����
//    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
  
    IIC_SDA_1;
    IIC_SCL_1;  
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     
	IIC_SDA_1;	  	  
	IIC_SCL_1;
	delay_us(4);
 	IIC_SDA_0;
	delay_us(4);
	IIC_SCL_0;
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL_0;
	IIC_SDA_0;
 	delay_us(4);
	IIC_SCL_1; 
	IIC_SDA_1;
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();       
	IIC_SDA_1;
  delay_us(1);	   
	IIC_SCL_1;
  delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_0; 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA_0;
	delay_us(2);
	IIC_SCL_1;
	delay_us(2);
	IIC_SCL_0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA_1;
	delay_us(2);
	IIC_SCL_1;
	delay_us(2);
	IIC_SCL_0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
  uint8_t t;   
  SDA_OUT(); 	    
  IIC_SCL_0;
  for(t=0;t<8;t++)
  {              
    if (txd & 0x80)
      IIC_SDA_1;
    else
      IIC_SDA_0;
    txd<<=1; 	  
    delay_us(2);   
    IIC_SCL_1;
    delay_us(2); 
    IIC_SCL_0;	
    delay_us(2);
  }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
  for(i=0;i<8;i++ )
	{
    IIC_SCL_0; 
    delay_us(2);
    IIC_SCL_1;
    receive<<=1;
    if(READ_SDA)receive++;   
		delay_us(1); 
   }					 
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack(); 
    return receive;
}


