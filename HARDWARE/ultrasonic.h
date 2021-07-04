#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#include "sys.h"

#define GET_D_0 getWdistance(25)		//0度角
#define GET_D_90 getWdistance(15)	//90度角
#define GET_D_180 getWdistance(5)	//180度角

void ultrasonicInit(void);	//初始化
void ult_tim4_init(u16 arr,u16 psc);	//红外控制延时控制定时器初始化
void getDistance(void);	//获取当前距离
void getWdistance(int n);	//获取不同方向距离

#endif
