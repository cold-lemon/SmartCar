#include "usart.h"

void usart1_init(u32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//GPIOAʱ��ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//����ʱ��ʹ��

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//���
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;	//PA9
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);		//���ų�ʼ��

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//����
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;	//PA10
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);	//���ų�ʼ��

    USART_InitStructure.USART_BaudRate=BaudRate;	//������
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	//���ͽ���һ���ʼ��
    USART_InitStructure.USART_Parity=USART_Parity_No;	//��żУ�飬��
    USART_InitStructure.USART_StopBits=USART_StopBits_1;	//ֹͣλ��1��
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;	//�ֳ���8��
    USART_Init(USART1,&USART_InitStructure);	//��ʼ������

    USART_Cmd(USART1,ENABLE);	//ʹ�ܴ���

    NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//��Ӧ���ȼ�
    NVIC_Init(&NVIC_InitStructure);	//�����ж�

    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//���������ж�

}

int fputc(int ch, FILE *fp)
{
    if (ch == '\n')
        fputc('\r', fp);

    USART1->DR = ch;
    while ((USART1->SR & USART_SR_TXE) == 0);//�ȴ��������
    return ch;
}

void USART1_IRQHandler(void)	// USART1 �жϺ���
{
    u8 res;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)==1)
    {
        res=USART_ReceiveData(USART1);
        USART_SendData(USART1,res);
    }
}
