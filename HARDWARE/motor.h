#ifndef _MOTOR_H
#define _MOTOR_H

#include "sys.h"

#define L293D_EN GPIO_SetBits(GPIOD,GPIO_Pin_4);GPIO_SetBits(GPIOD,GPIO_Pin_2)	//芯片使能
#define L293D_DISEN GPIO_ResetBits(GPIOD,GPIO_Pin_4);GPIO_ResetBits(GPIOD,GPIO_Pin_2)	//芯片失能
#define LEFT_UP GPIO_SetBits(GPIOC,GPIO_Pin_11);GPIO_ResetBits(GPIOD,GPIO_Pin_0)	//左轮前进
#define RIGHT_UP GPIO_SetBits(GPIOD,GPIO_Pin_6);GPIO_ResetBits(GPIOG,GPIO_Pin_9)	//右轮前进
#define LEFT_DOWN GPIO_ResetBits(GPIOC,GPIO_Pin_11);GPIO_SetBits(GPIOD,GPIO_Pin_0)	//左轮后退
#define RIGHT_DOWN GPIO_ResetBits(GPIOD,GPIO_Pin_6);GPIO_SetBits(GPIOG,GPIO_Pin_9)	//右轮后退
#define LEFT_STOP GPIO_ResetBits(GPIOC,GPIO_Pin_11);GPIO_ResetBits(GPIOD,GPIO_Pin_0)	//左轮停止
#define RIGHT_STOP GPIO_ResetBits(GPIOD,GPIO_Pin_6);GPIO_ResetBits(GPIOG,GPIO_Pin_9)	//右轮停止

#define SERVOSET GPIO_SetBits(GPIOA,GPIO_Pin_0)	//舵机置位
#define SERVORESET GPIO_ResetBits(GPIOA,GPIO_Pin_0)	//舵机清零
#define SERVO0 servoAngleCtr(25)
#define SERVO90 servoAngleCtr(15)
#define SERVO180 servoAngleCtr(5)
#define SERVOADD servoAngleADD()
#define SERVOSUB servoAngleSUB()

#define SPEED_ONE 20	//一档速度
#define SPEED_TWO 35	//二档速度
#define SPEED_THREE 50	//三档速度
#define SPEED_STOP 0	//停止档
#define SPEED_BACK -30	//倒车档

void motor_init(void);	//初始化
void motor_tim2_init(u16 arr,u16 psc);	//电机PWM时钟控制初始化
void speedCtr(int left,int right);		//速度控制
void carCtr(int left,int right);	//小车电机总控制
void carLeft(int left);	//小车左电机控制
void carRight(int right);	//小车右电机控制
int getLeftFlag(void);	//获取左电机值
int getRightFlag(void);	//获取右电机值
int getAngle(void);	//获取舵机角度值
void servoAngleCtr(int servoAngle);	//舵机角度控制
void servoAngleADD(void);	//顺时针
void servoAngleSUB(void);	//逆时针
void carMove(void);	//小车运动函数
void carMoveCtr(int left,int right);	//小车运动函数,带速度控制

#endif
