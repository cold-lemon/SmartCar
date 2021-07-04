#include "bluetooth.h"

u16 BTdate;

void usart3_init(u32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//GPIOA时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//串口时钟使能

    USART_DeInit(USART3);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//输出
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;	//PB10
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);		//引脚初始化

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//输入
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;	//PB11
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);	//引脚初始化

    USART_InitStructure.USART_BaudRate=BaudRate;	//波特率
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	//发送接收一起初始化
    USART_InitStructure.USART_Parity=USART_Parity_No;	//奇偶校验，无
    USART_InitStructure.USART_StopBits=USART_StopBits_1;	//停止位、1个
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;	//字长、8个
    USART_Init(USART3,&USART_InitStructure);	//初始化串口

    USART_Cmd(USART3,ENABLE);	//使能串口

    NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//响应优先级
    NVIC_Init(&NVIC_InitStructure);	//设置中断

    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);	//开启接收中断

}

void USART3_IRQHandler(void)	// USART3 中断函数
{
    //u8 res;
    if(USART_GetITStatus(USART3,USART_IT_RXNE)==1)
    {
        BTdate=USART_ReceiveData(USART3);
        //USART_SendData(USART3,res);
    }
}
