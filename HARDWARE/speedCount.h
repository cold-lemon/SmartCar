#ifndef _SPEEDCOUNT_H
#define _SPEEDCOUNT_H

#include "sys.h"

#define LEFT_SPEEP_STATUS GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)	//���ٶ�IO��״̬��ȡ
#define RIGHT_SPEEP_STATUS GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)	//���ٶ�IO��״̬��ȡ

void speedCountInit(void);	//GPIO��ʼ��
void speedTest(void);	//�ٶȲ���
void speed_tim3_init(u16 arr,u16 psc);	//�ٶȼ���ʱ�ӳ�ʼ��

#endif
