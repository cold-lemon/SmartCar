#include "lcd1602.h"	//lcd1602显示屏

unsigned char const welcomeTable[]="Welcome to use!";

unsigned char const distanceTable[]="Distance: ";

unsigned char const modeTable[]="MODE:";

unsigned char const speedTable1[]="FL:    cm/s    ";
unsigned char const speedTable2[]="FR:    cm/s    ";

unsigned char const trackTable1[]="Tracking state";
unsigned char const trackTable2[]="on the road";
unsigned char const trackTable3[]="road left";
unsigned char const trackTable4[]="road right";
unsigned char const trackTable5[]="out of road";

void lcdInit(void)	//初始化
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //PC13	数据/命令选择	1/0
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    //PC14	读/写选择	1/0
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    //PC15	使能信号		高电平读数据，下降沿写
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    //PF0-7	数据传输位
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOF,&GPIO_InitStructure);

    //初始化显示
    delay_ms(100);
    lcdWriteCmd(0x38);  //设置16*2显示，8位数据接口
    lcdWriteCmd(0x0c); //开显示，显示光标且闪烁
    lcdWriteCmd(0x06);//写一个指针自动加一
    lcdWriteCmd(0x01);//清屏
    delay_ms(100);//延时一段时间时间，等待LCD1602稳定

    //lcdDefault();
}

void lcdClear()		//清屏
{
    lcdWriteCmd(0x01);//清屏
    delay_ms(100);//延时一段时间时间，等待LCD1602稳定
}

void lcdWriteCmd(u8 cmd)	//写命令
{
    delay_us(20);
    LCDDATEWRITE(cmd);
    LCDRS_RESET;
    LCDRW_RESET;
    LCDEN_RESET;
    delay_us(10);
    LCDEN_SET;
    delay_us(10);
    LCDEN_RESET;
    delay_us(10);
}

void lcdWriteData(u8 date)	//读命令
{
    delay_us(20);
    LCDDATEWRITE(date);
    LCDRS_SET;
    LCDRW_RESET;
    LCDEN_RESET;
    delay_us(10);
    LCDEN_SET;
    delay_us(10);
    LCDEN_RESET;
    delay_us(10);
}

void lcdDefault()		//默认显示
{
    int index=0;
    lcdWriteCmd(0x80);//设置第一行 数据地址指针
    for(index=0; index<15; index++)
    {
        lcdWriteData(welcomeTable[index]);  //写入数据
    }
}

void lcdWriteDistance(u32 distance)		//LCD显示距离
{
    int index=0;
    char data_buf[4];

    lcdWriteCmd(0x80);//设置第一行 数据地址指针
    for(index=0; index<10; index++)
    {
        lcdWriteData(distanceTable[index]);  //写入数据
    }
    lcdWriteCmd(0xc0);//设置第二行 数据地址指针
	for(index=0;index<5;index++)
	{
		lcdWriteData(modeTable[index]);  //写入数据
	}

    data_buf[3] = 0;
    sprintf(data_buf,"%03d",(int)distance);
    lcdWriteCmd(0x80+10);//设置第一行 数据地址指针
    for(index=0; index<3; index++)
    {
        lcdWriteData(data_buf[index]);  //写入数据
    }
	
	sprintf(data_buf,"%03d",(int)mode+1);
	lcdWriteCmd(0xc0+6);//设置第二行 数据地址指针
    for(index=0; index<3; index++)
    {
        lcdWriteData(data_buf[index]);  //写入数据
    }
}

void lcdWriteSpeed(u8 ls,u8 rs)		//LCD显示速度
{
    int index=0;
    char data_buf[4];

    lcdWriteCmd(0x80);//设置第一行 数据地址指针
    for(index=0; index<11; index++)
    {
        lcdWriteData(speedTable1[index]);  //写入数据
    }

    lcdWriteCmd(0xc0);//设置第二行 数据地址指针
    for(index=0; index<11; index++)
    {
        lcdWriteData(speedTable2[index]);  //写入数据
    }

    data_buf[3] = 0;
    sprintf(data_buf,"%03d",(int)ls);
    lcdWriteCmd(0x80+3);//设置第一行 数据地址指针
    for(index=0; index<3; index++)
    {
        lcdWriteData(data_buf[index]);  //写入数据
    }

    sprintf(data_buf,"%03d",(int)rs);
    lcdWriteCmd(0xc0+3);//设置第一行 数据地址指针
    for(index=0; index<3; index++)
    {
        lcdWriteData(data_buf[index]);  //写入数据
    }
}

void lcdWriteTrackMsg(u8 trackS)		//循迹信息
{
    int index=0;

    lcdWriteCmd(0x80);//设置第一行 数据地址指针
    for(index=0; index<14; index++)
    {
        lcdWriteData(trackTable1[index]);  //写入数据
    }

    lcdWriteCmd(0xc0);//设置第二行 数据地址指针
    switch(trackStatus)
    {
    case ONLINE:
        for(index=0; index<11; index++)
        {
            lcdWriteData(trackTable2[index]);  //写入数据
        }
        break;
    case ONLEFT:
        for(index=0; index<9; index++)
        {
            lcdWriteData(trackTable3[index]);  //写入数据
        }
        break;
    case ONRIGHT:
        for(index=0; index<10; index++)
        {
            lcdWriteData(trackTable4[index]);  //写入数据
        }
        break;
    default:
		for(index=0; index<11; index++)
        {
            lcdWriteData(trackTable5[index]);  //写入数据
        }
        break;
    }
}
