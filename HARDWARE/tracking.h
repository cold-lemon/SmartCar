#ifndef _TRACKING_H
#define _TRACKING_H

#include "sys.h"

#define TRACKING_LEFT GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6)		//��
#define TRACKING_MIDDLE GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_8)		//��
#define TRACKING_RIGHT GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4)		//��

#define BLACK 1		//��⵽����
#define WHITE 0		//��ⲻ������

#define ONLINE 1
#define ONLEFT 2
#define ONRIGHT 3

void tracking_init(void);		//��·ѭ����ʼ��
void trackingSearch(void);		//��ȡѭ��״̬

#endif
