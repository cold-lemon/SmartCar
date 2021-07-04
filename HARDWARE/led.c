#include "led.h"

void led_init(void)	//初始化
{
    GPIO_InitTypeDef GPIO_InitTypeStructure;

    //PE12   LED
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_12;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOE,&GPIO_InitTypeStructure);

    //PE14   蜂鸣器
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_14;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOE,&GPIO_InitTypeStructure);
}
/*
*警示灯
*正常显示：一秒闪一次（0，1000，1000）
*常亮：
*常灭：
*初级警示：（0，700，700）
*中级警示：（0，400，400）
*高级警示：（0，100，100）
*/
void led_MODE(u8 n,u16 x,u16 y)
{
    switch(n)
    {
    case 0:		//毫秒模式
        GPIO_SetBits(GPIOE,GPIO_Pin_12);
        delay_ms(x);
        GPIO_ResetBits(GPIOE,GPIO_Pin_12);
        delay_ms(y);
        break;
    case 1:		//微秒模式
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
*预警铃声
*警告提示：（0，500，500）
*紧急预警：（0，150，150）
*/
void beep_MODE(u8 n,u16 x,u16 y)
{
    switch(n)
    {
    case 0:		//毫秒模式
        GPIO_SetBits(GPIOE,GPIO_Pin_14);
        delay_ms(x);
        GPIO_ResetBits(GPIOE,GPIO_Pin_14);
        delay_ms(y);
        break;
    case 1:		//微秒模式
        GPIO_SetBits(GPIOE,GPIO_Pin_14);
        delay_us(x);
        GPIO_ResetBits(GPIOE,GPIO_Pin_14);
        delay_us(y);
        break;
    default:
        break;
    }
}
