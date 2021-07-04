#ifndef _LCD1602_H
#define _LCD1602_H

#include "sys.h"

#define LCDRS_SET GPIO_SetBits(GPIOC,GPIO_Pin_13)	//数据输入
#define LCDRS_RESET GPIO_ResetBits(GPIOC,GPIO_Pin_13)	//指令输入
#define LCDRW_SET GPIO_SetBits(GPIOC,GPIO_Pin_14)	//读操作
#define LCDRW_RESET GPIO_ResetBits(GPIOC,GPIO_Pin_14)	//写操作
#define LCDEN_SET GPIO_SetBits(GPIOC,GPIO_Pin_15)	
#define LCDEN_RESET GPIO_ResetBits(GPIOC,GPIO_Pin_15)

//#define LCDDATEWRITE(x) GPIO_Write(GPIOF,x);	//写数据到数据位
#define LCDDATEWRITE(x)   GPIOF->BSRR = 0x00ff0000;GPIOF->BSRR = (x)

void lcdInit(void);	//初始化
void lcdWriteCmd(u8 cmd);	//写命令
void lcdWriteData(u8 date);	//读命令

void lcdDefault(void);		//默认显示
void lcdClear(void);		//清屏
void lcdWriteDistance(u32 distance);		//LCD显示距离
void lcdWriteSpeed(u8 ls,u8 rs);		//LCD显示速度
void lcdWriteTrackMsg(u8 trackS);		//循迹信息

#endif
