#include "bluetooth.h"

u16 BTdate;

void usart3_init(u32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//GPIOAʱ��ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//����ʱ��ʹ��

    USART_DeInit(USART3);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//���
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;	//PB10
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);		//���ų�ʼ��

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//����
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;	//PB11
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);	//���ų�ʼ��

    USART_InitStructure.USART_BaudRate=BaudRate;	//������
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	//���ͽ���һ���ʼ��
    USART_InitStructure.USART_Parity=USART_Parity_No;	//��żУ�飬��
    USART_InitStructure.USART_StopBits=USART_StopBits_1;	//ֹͣλ��1��
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;	//�ֳ���8��
    USART_Init(USART3,&USART_InitStructure);	//��ʼ������

    USART_Cmd(USART3,ENABLE);	//ʹ�ܴ���

    NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//��Ӧ���ȼ�
    NVIC_Init(&NVIC_InitStructure);	//�����ж�

    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);	//���������ж�

}

void USART3_IRQHandler(void)	// USART3 �жϺ���
{
    //u8 res;
    if(USART_GetITStatus(USART3,USART_IT_RXNE)==1)
    {
        BTdate=USART_ReceiveData(USART3);
        //USART_SendData(USART3,res);
    }
}
