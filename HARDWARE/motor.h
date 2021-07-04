#ifndef _MOTOR_H
#define _MOTOR_H

#include "sys.h"

#define L293D_EN GPIO_SetBits(GPIOD,GPIO_Pin_4);GPIO_SetBits(GPIOD,GPIO_Pin_2)	//оƬʹ��
#define L293D_DISEN GPIO_ResetBits(GPIOD,GPIO_Pin_4);GPIO_ResetBits(GPIOD,GPIO_Pin_2)	//оƬʧ��
#define LEFT_UP GPIO_SetBits(GPIOC,GPIO_Pin_11);GPIO_ResetBits(GPIOD,GPIO_Pin_0)	//����ǰ��
#define RIGHT_UP GPIO_SetBits(GPIOD,GPIO_Pin_6);GPIO_ResetBits(GPIOG,GPIO_Pin_9)	//����ǰ��
#define LEFT_DOWN GPIO_ResetBits(GPIOC,GPIO_Pin_11);GPIO_SetBits(GPIOD,GPIO_Pin_0)	//���ֺ���
#define RIGHT_DOWN GPIO_ResetBits(GPIOD,GPIO_Pin_6);GPIO_SetBits(GPIOG,GPIO_Pin_9)	//���ֺ���
#define LEFT_STOP GPIO_ResetBits(GPIOC,GPIO_Pin_11);GPIO_ResetBits(GPIOD,GPIO_Pin_0)	//����ֹͣ
#define RIGHT_STOP GPIO_ResetBits(GPIOD,GPIO_Pin_6);GPIO_ResetBits(GPIOG,GPIO_Pin_9)	//����ֹͣ

#define SERVOSET GPIO_SetBits(GPIOA,GPIO_Pin_0)	//�����λ
#define SERVORESET GPIO_ResetBits(GPIOA,GPIO_Pin_0)	//�������
#define SERVO0 servoAngleCtr(25)
#define SERVO90 servoAngleCtr(15)
#define SERVO180 servoAngleCtr(5)
#define SERVOADD servoAngleADD()
#define SERVOSUB servoAngleSUB()

#define SPEED_ONE 20	//һ���ٶ�
#define SPEED_TWO 35	//�����ٶ�
#define SPEED_THREE 50	//�����ٶ�
#define SPEED_STOP 0	//ֹͣ��
#define SPEED_BACK -30	//������

void motor_init(void);	//��ʼ��
void motor_tim2_init(u16 arr,u16 psc);	//���PWMʱ�ӿ��Ƴ�ʼ��
void speedCtr(int left,int right);		//�ٶȿ���
void carCtr(int left,int right);	//С������ܿ���
void carLeft(int left);	//С����������
void carRight(int right);	//С���ҵ������
int getLeftFlag(void);	//��ȡ����ֵ
int getRightFlag(void);	//��ȡ�ҵ��ֵ
int getAngle(void);	//��ȡ����Ƕ�ֵ
void servoAngleCtr(int servoAngle);	//����Ƕȿ���
void servoAngleADD(void);	//˳ʱ��
void servoAngleSUB(void);	//��ʱ��
void carMove(void);	//С���˶�����
void carMoveCtr(int left,int right);	//С���˶�����,���ٶȿ���

#endif
