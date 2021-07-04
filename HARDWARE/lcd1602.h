#ifndef _LCD1602_H
#define _LCD1602_H

#include "sys.h"

#define LCDRS_SET GPIO_SetBits(GPIOC,GPIO_Pin_13)	//��������
#define LCDRS_RESET GPIO_ResetBits(GPIOC,GPIO_Pin_13)	//ָ������
#define LCDRW_SET GPIO_SetBits(GPIOC,GPIO_Pin_14)	//������
#define LCDRW_RESET GPIO_ResetBits(GPIOC,GPIO_Pin_14)	//д����
#define LCDEN_SET GPIO_SetBits(GPIOC,GPIO_Pin_15)	
#define LCDEN_RESET GPIO_ResetBits(GPIOC,GPIO_Pin_15)

//#define LCDDATEWRITE(x) GPIO_Write(GPIOF,x);	//д���ݵ�����λ
#define LCDDATEWRITE(x)   GPIOF->BSRR = 0x00ff0000;GPIOF->BSRR = (x)

void lcdInit(void);	//��ʼ��
void lcdWriteCmd(u8 cmd);	//д����
void lcdWriteData(u8 date);	//������

void lcdDefault(void);		//Ĭ����ʾ
void lcdClear(void);		//����
void lcdWriteDistance(u32 distance);		//LCD��ʾ����
void lcdWriteSpeed(u8 ls,u8 rs);		//LCD��ʾ�ٶ�
void lcdWriteTrackMsg(u8 trackS);		//ѭ����Ϣ

#endif
