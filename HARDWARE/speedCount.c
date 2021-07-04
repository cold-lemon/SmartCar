#include "speedCount.h"

//����ֱ��66mm��������̳���Ϊ20�������ܳ� 207mm = 20.7cm
//��������жϸߵ͵�ƽ�仯����������Ҳ����˵����תһ�ܼ�������Ϊ40
//һ�������仯��ʾ�����ܹ��ľ���Ϊ 20.7/40 = 0.5175cm

u8 leftSpeed=0;
u8 rightSpeed=0;
static u8 leftSpeedTmp=0;
static u8 rightSpeedTmp=0;
static u8 left_compare=0;
static u8 right_compare=0;
static u8 timeCount=0;

void speedCountInit(void)	//GPIO��ʼ��
{
    GPIO_InitTypeDef GPIO_InitTypeStructure;

    //���ٶȼ��� PA12
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_12;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitTypeStructure);

    //���ٶȼ��� PA11
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_11;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitTypeStructure);

    speed_tim3_init(49,7199);		//5ms �ж�һ��
}

void speed_tim3_init(u16 arr,u16 psc)	//�ٶȼ���ʱ�ӳ�ʼ��
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM3,ENABLE);
}

void speedTest(void)	//�ٶȲ���
{
    timeCount++;
    if(LEFT_SPEEP_STATUS != left_compare) {
        leftSpeedTmp++;
        left_compare=LEFT_SPEEP_STATUS;
    }

    if(RIGHT_SPEEP_STATUS != right_compare) {
        rightSpeedTmp++;
        right_compare=RIGHT_SPEEP_STATUS;
    }

    if(timeCount == 100) {	//500ms��ȡһ������
        timeCount=0;
        //��ȡһ���ڵ���ֵ
        leftSpeed=leftSpeedTmp*2;
        rightSpeed=rightSpeedTmp*2;
        leftSpeedTmp=0;
        rightSpeedTmp=0;

        leftSpeed=(u8)(0.5175*(double)leftSpeed+0.5);	//�������룬��λ cm/s
        rightSpeed=(u8)(0.5175*(double)rightSpeed+0.5);
    }
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET) {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
        speedTest();
    }
}
