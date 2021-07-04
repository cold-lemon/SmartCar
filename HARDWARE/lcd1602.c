#include "lcd1602.h"	//lcd1602��ʾ��

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

void lcdInit(void)	//��ʼ��
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //PC13	����/����ѡ��	1/0
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    //PC14	��/дѡ��	1/0
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    //PC15	ʹ���ź�		�ߵ�ƽ�����ݣ��½���д
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    //PF0-7	���ݴ���λ
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOF,&GPIO_InitStructure);

    //��ʼ����ʾ
    delay_ms(100);
    lcdWriteCmd(0x38);  //����16*2��ʾ��8λ���ݽӿ�
    lcdWriteCmd(0x0c); //����ʾ����ʾ�������˸
    lcdWriteCmd(0x06);//дһ��ָ���Զ���һ
    lcdWriteCmd(0x01);//����
    delay_ms(100);//��ʱһ��ʱ��ʱ�䣬�ȴ�LCD1602�ȶ�

    //lcdDefault();
}

void lcdClear()		//����
{
    lcdWriteCmd(0x01);//����
    delay_ms(100);//��ʱһ��ʱ��ʱ�䣬�ȴ�LCD1602�ȶ�
}

void lcdWriteCmd(u8 cmd)	//д����
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

void lcdWriteData(u8 date)	//������
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

void lcdDefault()		//Ĭ����ʾ
{
    int index=0;
    lcdWriteCmd(0x80);//���õ�һ�� ���ݵ�ַָ��
    for(index=0; index<15; index++)
    {
        lcdWriteData(welcomeTable[index]);  //д������
    }
}

void lcdWriteDistance(u32 distance)		//LCD��ʾ����
{
    int index=0;
    char data_buf[4];

    lcdWriteCmd(0x80);//���õ�һ�� ���ݵ�ַָ��
    for(index=0; index<10; index++)
    {
        lcdWriteData(distanceTable[index]);  //д������
    }
    lcdWriteCmd(0xc0);//���õڶ��� ���ݵ�ַָ��
	for(index=0;index<5;index++)
	{
		lcdWriteData(modeTable[index]);  //д������
	}

    data_buf[3] = 0;
    sprintf(data_buf,"%03d",(int)distance);
    lcdWriteCmd(0x80+10);//���õ�һ�� ���ݵ�ַָ��
    for(index=0; index<3; index++)
    {
        lcdWriteData(data_buf[index]);  //д������
    }
	
	sprintf(data_buf,"%03d",(int)mode+1);
	lcdWriteCmd(0xc0+6);//���õڶ��� ���ݵ�ַָ��
    for(index=0; index<3; index++)
    {
        lcdWriteData(data_buf[index]);  //д������
    }
}

void lcdWriteSpeed(u8 ls,u8 rs)		//LCD��ʾ�ٶ�
{
    int index=0;
    char data_buf[4];

    lcdWriteCmd(0x80);//���õ�һ�� ���ݵ�ַָ��
    for(index=0; index<11; index++)
    {
        lcdWriteData(speedTable1[index]);  //д������
    }

    lcdWriteCmd(0xc0);//���õڶ��� ���ݵ�ַָ��
    for(index=0; index<11; index++)
    {
        lcdWriteData(speedTable2[index]);  //д������
    }

    data_buf[3] = 0;
    sprintf(data_buf,"%03d",(int)ls);
    lcdWriteCmd(0x80+3);//���õ�һ�� ���ݵ�ַָ��
    for(index=0; index<3; index++)
    {
        lcdWriteData(data_buf[index]);  //д������
    }

    sprintf(data_buf,"%03d",(int)rs);
    lcdWriteCmd(0xc0+3);//���õ�һ�� ���ݵ�ַָ��
    for(index=0; index<3; index++)
    {
        lcdWriteData(data_buf[index]);  //д������
    }
}

void lcdWriteTrackMsg(u8 trackS)		//ѭ����Ϣ
{
    int index=0;

    lcdWriteCmd(0x80);//���õ�һ�� ���ݵ�ַָ��
    for(index=0; index<14; index++)
    {
        lcdWriteData(trackTable1[index]);  //д������
    }

    lcdWriteCmd(0xc0);//���õڶ��� ���ݵ�ַָ��
    switch(trackStatus)
    {
    case ONLINE:
        for(index=0; index<11; index++)
        {
            lcdWriteData(trackTable2[index]);  //д������
        }
        break;
    case ONLEFT:
        for(index=0; index<9; index++)
        {
            lcdWriteData(trackTable3[index]);  //д������
        }
        break;
    case ONRIGHT:
        for(index=0; index<10; index++)
        {
            lcdWriteData(trackTable4[index]);  //д������
        }
        break;
    default:
		for(index=0; index<11; index++)
        {
            lcdWriteData(trackTable5[index]);  //д������
        }
        break;
    }
}
