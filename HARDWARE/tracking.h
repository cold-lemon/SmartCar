#ifndef _TRACKING_H
#define _TRACKING_H

#include "sys.h"

#define TRACKING_LEFT GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6)		//左
#define TRACKING_MIDDLE GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_8)		//中
#define TRACKING_RIGHT GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4)		//右

#define BLACK 1		//检测到黑线
#define WHITE 0		//检测不到黑线

#define ONLINE 1
#define ONLEFT 2
#define ONRIGHT 3

void tracking_init(void);		//三路循迹初始化
void trackingSearch(void);		//获取循迹状态

#endif
