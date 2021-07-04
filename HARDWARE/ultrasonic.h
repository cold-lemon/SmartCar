#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#include "sys.h"

#define GET_D_0 getWdistance(25)		//0�Ƚ�
#define GET_D_90 getWdistance(15)	//90�Ƚ�
#define GET_D_180 getWdistance(5)	//180�Ƚ�

void ultrasonicInit(void);	//��ʼ��
void ult_tim4_init(u16 arr,u16 psc);	//���������ʱ���ƶ�ʱ����ʼ��
void getDistance(void);	//��ȡ��ǰ����
void getWdistance(int n);	//��ȡ��ͬ�������

#endif
