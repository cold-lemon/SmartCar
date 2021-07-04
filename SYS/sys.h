#ifndef _SYS_H
#define _SYS_H

#include "stm32f10x.h"	//核心头文件
#include "stdio.h"
#include "delay.h"	//延时模块
#include "usart.h"	//串口模块
#include "motor.h"	//电机模块
#include "infrared.h"	//红外模块
#include "led.h"	//led提示灯
#include "speedCount.h"	//速度计算模块
#include "tracking.h"	//循迹模块
#include "ultrasonic.h"	//超声波测距模块
#include "bluetooth.h"	//蓝牙模块
#include "lcd1602.h"	//lcd1602显示屏

#define MGET_BIT(x,bit) ((x & (1 << bit)) >> bit)	//获取位bit的值		bit=[7:0]
#define MSET_BIT(x, bit) (x |= (1 << bit))  /* 置位位bit */
#define MCLEAR_BIT(x, bit) (x &= ~(1 << bit)) /* 清零位bit */
#define MBIT_M_TO_N(x, m, n) ((unsigned int)(x << (31-(n))) >> ((31 - (n)) + (m)))		//获取第[n:m]位的值

#define MAX2(a,b) ((a) > (b) ? (a) : (b))
#define MAX3(a,b,c) (a>b?(a>c?a:c):(b>c?b:c))

#define MIN2(a,b) ((a) < (b) ? (a) : (b))
#define MIN3(a,b,c) (a<b?(a<c?a:c):(b<c?b:c))

#define MID(a, b, c) (a < b ? (b < c ? b : a < c ? c : a) : (b > c ? b : a > c ? c : a))

#define MODE0 0		//人工操作
#define MODE1 1		//智能系统操作
#define MODE2 2		//系统辅助人工操作
#define MODE3 3		//人工辅助系统操作

extern u8 mode;	//模式选择

extern char speed_count;

extern u8 leftSpeed;	//测速值
extern u8 rightSpeed;

extern volatile u32 distanceCM;	//获取距离
extern u32 allDistanceCM[5];	//0：0度角、1：90度角、2：180度角、3：左半边测距、4右半边测距
extern u8 direction;

extern u8 trackStatus;		//循迹状态

extern unsigned char IRdata[4];	//接收到的数据
extern u8 IRflag;	//接收数据标志位	0：无新数据，1：有新数据

extern char ctrlCommand;	//控制指令

extern u8 servoFlag;

extern u8 pwm_1ms;
extern u8 tick_5ms;
extern u8 tick_200ms;
extern u8 mode2_redTime;
extern u8 wirelessFlag;	//无线通信标志
extern u8 redAvoidCount,distanceCount;
extern u8 super_power;
extern int powerCount;
extern u8 redAvoidflag,trackingflag,ultrasonicflag;

void mode0(void);
void mode1(void);
void mode2(void);
void mode3(void);

//位带操作
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
//IO口地址映射
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

//IO口操作,只对单一的IO口
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

void GPIO_clock_init(void);	//IO口时钟使能

#endif
