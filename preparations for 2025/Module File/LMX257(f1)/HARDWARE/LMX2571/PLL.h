#ifndef _PLL_H_
#define _PLL_H_

#include "main.h"

#define u32_ unsigned long int
#define u8_ unsigned char
#define u16_ unsigned int
#define u64_ unsigned long long int


typedef struct PLL_Return {
	unsigned int			N_Divier;//N��Ƶ��λ��
	unsigned int			NUM_Divier;//С����Ƶ��numλ��
	unsigned int			DEN_Divier;//С����Ƶ��denλ��
	unsigned int			PRE_R_Divier;//R��Ƶ��Preλ��
	unsigned int			POST_R_Divier;//R��Ƶ��Postλ��
	unsigned int			CHDIV1;//�����Ƶ��1λ��
	unsigned int			CHDIV2;//�����Ƶ��2λ��
	unsigned int			Prescalar;
	unsigned int			MULT;
	unsigned long			nowfpd;

	
}_PLL_Return;

typedef struct PLL_state {
	unsigned long			clkin; //ʱ��Ƶ��
	unsigned long			fpd; //����Ƶ��

	unsigned int			N_Divier_BIT;//N��Ƶ��λ��
	unsigned int			NUM_Divier_BIT;//С����Ƶ��numλ��
	unsigned int			DEN_Divier_BIT;//С����Ƶ��denλ��
	unsigned int			PRE_R_Divier_BIT;//R��Ƶ��Preλ��
	unsigned int			POST_R_Divier_BIT;//R��Ƶ��Postλ��
	unsigned int			MULT_Divier_BIT;//R��Ƶ��Postλ��

	unsigned int			CHDIV1_BIT;//�����Ƶ��1λ��
	unsigned int			CHDIV2_BIT;//�����Ƶ��2λ��
	unsigned int			Prescalar_BIT;//Ԥ��Ƶ��λ��

	unsigned long			min_fpfd_freq; //��С����Ƶ��
	unsigned long			max_fpfd_freq; //������Ƶ��
	unsigned long long int		min_VCO_freq; //��С���Ƶ��
	unsigned long long int		max_VCO_freq; //������Ƶ��


	unsigned int			CHDIV1_MIN;//�����Ƶ��1���ֵ
	unsigned int			CHDIV1_MAX;//�����Ƶ��1���ֵ
	unsigned int			CHDIV2_MIN;//�����Ƶ��1���ֵ
	unsigned int			CHDIV2_MAX;//�����Ƶ��1���ֵ
	unsigned int			MULT_MAX;//���뱶Ƶ�����ֵ
	unsigned int			N_Divier_MAX;//N��Ƶ�����ֵ
	unsigned int			NUM_Divier_MAX;//С����Ƶ�����ֵ
	unsigned int			DEN_Divier_MAX;//С����Ƶ�����ֵ
	unsigned int			PRE_R_Divier_MAX;//С����Ƶ�����ֵ
	unsigned int			POST_R_Divier_MAX;//С����Ƶ�����ֵ

	_PLL_Return PLL;

}_PLL_state;

void PLL_Data_Init(_PLL_state val);
u8_ PLL_Calulate(u64_ fre);
_PLL_state  GetPLLNum(void);


#endif
