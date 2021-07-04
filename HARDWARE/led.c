#include "led.h"

void led_init(void)	//��ʼ��
{
    GPIO_InitTypeDef GPIO_InitTypeStructure;

    //PE12   LED
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_12;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOE,&GPIO_InitTypeStructure);

    //PE14   ������
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_14;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOE,&GPIO_InitTypeStructure);
}
/*
*��ʾ��
*������ʾ��һ����һ�Σ�0��1000��1000��
*������
*����
*������ʾ����0��700��700��
*�м���ʾ����0��400��400��
*�߼���ʾ����0��100��100��
*/
void led_MODE(u8 n,u16 x,u16 y)
{
    switch(n)
    {
    case 0:		//����ģʽ
        GPIO_SetBits(GPIOE,GPIO_Pin_12);
        delay_ms(x);
        GPIO_ResetBits(GPIOE,GPIO_Pin_12);
        delay_ms(y);
        break;
    case 1:		//΢��ģʽ
        GPIO_SetBits(GPIOE,GPIO_Pin_12);
        delay_us(x);
        GPIO_ResetBits(GPIOE,GPIO_Pin_12);
        delay_us(y);
        break;
    default:
        break;
    }
}
/*
*Ԥ������
*������ʾ����0��500��500��
*����Ԥ������0��150��150��
*/
void beep_MODE(u8 n,u16 x,u16 y)
{
    switch(n)
    {
    case 0:		//����ģʽ
        GPIO_SetBits(GPIOE,GPIO_Pin_14);
        delay_ms(x);
        GPIO_ResetBits(GPIOE,GPIO_Pin_14);
        delay_ms(y);
        break;
    case 1:		//΢��ģʽ
        GPIO_SetBits(GPIOE,GPIO_Pin_14);
        delay_us(x);
        GPIO_ResetBits(GPIOE,GPIO_Pin_14);
        delay_us(y);
        break;
    default:
        break;
    }
}
