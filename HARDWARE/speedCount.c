#include "speedCount.h"

//轮子直径66mm，光电码盘齿数为20，轮子周长 207mm = 20.7cm
//程序采用判断高低电平变化次数计数，也就是说轮子转一周计数次数为40
//一个计数变化表示轮子跑过的距离为 20.7/40 = 0.5175cm

u8 leftSpeed=0;
u8 rightSpeed=0;
static u8 leftSpeedTmp=0;
static u8 rightSpeedTmp=0;
static u8 left_compare=0;
static u8 right_compare=0;
static u8 timeCount=0;

void speedCountInit(void)	//GPIO初始化
{
    GPIO_InitTypeDef GPIO_InitTypeStructure;

    //右速度计算 PA12
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_12;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitTypeStructure);

    //左速度计算 PA11
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_11;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitTypeStructure);

    speed_tim3_init(49,7199);		//5ms 中断一次
}

void speed_tim3_init(u16 arr,u16 psc)	//速度计算时钟初始化
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

void speedTest(void)	//速度测量
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

    if(timeCount == 100) {	//500ms获取一次数据
        timeCount=0;
        //获取一秒内的数值
        leftSpeed=leftSpeedTmp*2;
        rightSpeed=rightSpeedTmp*2;
        leftSpeedTmp=0;
        rightSpeedTmp=0;

        leftSpeed=(u8)(0.5175*(double)leftSpeed+0.5);	//四舍五入，单位 cm/s
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
