#ifndef _SPEEDCOUNT_H
#define _SPEEDCOUNT_H

#include "sys.h"

#define LEFT_SPEEP_STATUS GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)	//左速度IO口状态获取
#define RIGHT_SPEEP_STATUS GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)	//右速度IO口状态获取

void speedCountInit(void);	//GPIO初始化
void speedTest(void);	//速度测量
void speed_tim3_init(u16 arr,u16 psc);	//速度计算时钟初始化

#endif
