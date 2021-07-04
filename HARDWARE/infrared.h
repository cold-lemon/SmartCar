#ifndef _INFRARED_H
#define _INFRARED_H

#include "sys.h"

#define AVOID_LEFT 1	//��߼�⵽�ϰ�
#define AVOID_RIGHT 2	//�ұ߼�⵽�ϰ�
#define AVOID_BOTH 3	//���߶���⵽�ϰ�
#define AVOID_NONE 0	//���߶�û�м�⵽�ϰ�

#define BARRIER_Y 0	//���ϰ���
#define BARRIER_N 1	//���ϰ���

//ָ���
#define COMM_STOP  'S'//ֹͣ	0x1C
#define COMM_UP    'A'//ǰ��	0x18
#define COMM_DOWN  'B'//����	0x52
#define COMM_LEFT  'C'//��ת	0x08
#define COMM_RIGHT 'D'//��ת	0x5A
#define SPEED_UP 'E'	//����	0x15
#define SPEED_DOWN 'F'	//����	0x07
#define COMM_TRACK 'G'	//ѭ������		0x16
#define SWITCH_MODE 'H'	//ģʽѡ��		0x09
#define COMM_REDAVOID 'I'		//������Ͽ���		0x19
#define COMM_ULTRASONIC 'J'		//����������		0x0d
#define TURN_LEFT 'K'		//������תָʾ		0x0C
#define TURN_RIGHT 'L'		//������תָʾ		0x5E
#define COMM_LED 'M'	//ָʾ��		0x42
#define COMM_BEEP 'N'	//������		0x4A
#define SERVO_0 'O'		//��� 0 ��		0x44
#define SERVO_90 'P'	//��� 90 ��		0x46
#define SERVO_180 'Q'	//��� 180 ��	0x43
#define SERVO_ADD 'R'	//����Ӷ�����˳ʱ����ת		0x47
#define SERVO_SUB 'T'	//�������������ʱ����ת		0x45
#define SUPER_PRIV 'U'	//����Ȩ��		0x40
#define COMM_NULL 'Z'		//��ָ��

#define IR_VALUE GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)	//��ȡ������Ƶ�����

void IR_init(void);	//������ϡ�������Ƴ�ʼ��
void IR_tim3_init(u16 arr,u16 psc);	//���������ʱ���ƶ�ʱ����ʼ��
char avoid_status(void);	//��ȡ������ϵ�״̬
void red_avoid_run(void);	//��������߷�
void red_check(int i);		//���Ϻ���

#endif
