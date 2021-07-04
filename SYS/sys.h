#ifndef _SYS_H
#define _SYS_H

#include "stm32f10x.h"	//����ͷ�ļ�
#include "stdio.h"
#include "delay.h"	//��ʱģ��
#include "usart.h"	//����ģ��
#include "motor.h"	//���ģ��
#include "infrared.h"	//����ģ��
#include "led.h"	//led��ʾ��
#include "speedCount.h"	//�ٶȼ���ģ��
#include "tracking.h"	//ѭ��ģ��
#include "ultrasonic.h"	//���������ģ��
#include "bluetooth.h"	//����ģ��
#include "lcd1602.h"	//lcd1602��ʾ��

#define MGET_BIT(x,bit) ((x & (1 << bit)) >> bit)	//��ȡλbit��ֵ		bit=[7:0]
#define MSET_BIT(x, bit) (x |= (1 << bit))  /* ��λλbit */
#define MCLEAR_BIT(x, bit) (x &= ~(1 << bit)) /* ����λbit */
#define MBIT_M_TO_N(x, m, n) ((unsigned int)(x << (31-(n))) >> ((31 - (n)) + (m)))		//��ȡ��[n:m]λ��ֵ

#define MAX2(a,b) ((a) > (b) ? (a) : (b))
#define MAX3(a,b,c) (a>b?(a>c?a:c):(b>c?b:c))

#define MIN2(a,b) ((a) < (b) ? (a) : (b))
#define MIN3(a,b,c) (a<b?(a<c?a:c):(b<c?b:c))

#define MID(a, b, c) (a < b ? (b < c ? b : a < c ? c : a) : (b > c ? b : a > c ? c : a))

#define MODE0 0		//�˹�����
#define MODE1 1		//����ϵͳ����
#define MODE2 2		//ϵͳ�����˹�����
#define MODE3 3		//�˹�����ϵͳ����

extern u8 mode;	//ģʽѡ��

extern char speed_count;

extern u8 leftSpeed;	//����ֵ
extern u8 rightSpeed;

extern volatile u32 distanceCM;	//��ȡ����
extern u32 allDistanceCM[5];	//0��0�Ƚǡ�1��90�Ƚǡ�2��180�Ƚǡ�3�����߲�ࡢ4�Ұ�߲��
extern u8 direction;

extern u8 trackStatus;		//ѭ��״̬

extern unsigned char IRdata[4];	//���յ�������
extern u8 IRflag;	//�������ݱ�־λ	0���������ݣ�1����������

extern char ctrlCommand;	//����ָ��

extern u8 servoFlag;

extern u8 pwm_1ms;
extern u8 tick_5ms;
extern u8 tick_200ms;
extern u8 mode2_redTime;
extern u8 wirelessFlag;	//����ͨ�ű�־
extern u8 redAvoidCount,distanceCount;
extern u8 super_power;
extern int powerCount;
extern u8 redAvoidflag,trackingflag,ultrasonicflag;

void mode0(void);
void mode1(void);
void mode2(void);
void mode3(void);

//λ������
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

//IO�ڲ���,ֻ�Ե�һ��IO��
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

void GPIO_clock_init(void);	//IO��ʱ��ʹ��

#endif
