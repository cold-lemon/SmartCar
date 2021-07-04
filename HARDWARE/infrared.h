#ifndef _INFRARED_H
#define _INFRARED_H

#include "sys.h"

#define AVOID_LEFT 1	//左边检测到障碍
#define AVOID_RIGHT 2	//右边检测到障碍
#define AVOID_BOTH 3	//两边都检测到障碍
#define AVOID_NONE 0	//两边都没有检测到障碍

#define BARRIER_Y 0	//有障碍物
#define BARRIER_N 1	//无障碍物

//指令定义
#define COMM_STOP  'S'//停止	0x1C
#define COMM_UP    'A'//前进	0x18
#define COMM_DOWN  'B'//后退	0x52
#define COMM_LEFT  'C'//左转	0x08
#define COMM_RIGHT 'D'//右转	0x5A
#define SPEED_UP 'E'	//加速	0x15
#define SPEED_DOWN 'F'	//减速	0x07
#define COMM_TRACK 'G'	//循迹开关		0x16
#define SWITCH_MODE 'H'	//模式选择		0x09
#define COMM_REDAVOID 'I'		//红外避障开关		0x19
#define COMM_ULTRASONIC 'J'		//超声波开关		0x0d
#define TURN_LEFT 'K'		//开启左转指示		0x0C
#define TURN_RIGHT 'L'		//开启右转指示		0x5E
#define COMM_LED 'M'	//指示灯		0x42
#define COMM_BEEP 'N'	//警报铃		0x4A
#define SERVO_0 'O'		//舵机 0 度		0x44
#define SERVO_90 'P'	//舵机 90 度		0x46
#define SERVO_180 'Q'	//舵机 180 度	0x43
#define SERVO_ADD 'R'	//舵机加度数，顺时针旋转		0x47
#define SERVO_SUB 'T'	//舵机减度数，逆时针旋转		0x45
#define SUPER_PRIV 'U'	//超级权限		0x40
#define COMM_NULL 'Z'		//空指令

#define IR_VALUE GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)	//读取红外控制的数据

void IR_init(void);	//红外避障、红外控制初始化
void IR_tim3_init(u16 arr,u16 psc);	//红外控制延时控制定时器初始化
char avoid_status(void);	//获取红外避障的状态
void red_avoid_run(void);	//红外避障走法
void red_check(int i);		//避障后检查

#endif
