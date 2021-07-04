#include "ultrasonic.h"

volatile u8 ultrasonicStatus=0;	//状态标志：0，空闲、1，发送触发信号、2，等待回响信号
volatile u32 distanceCM=0;		//实时距离
u32 allDistanceCM[5];		//0：0度角、1：90度角、2：180度角、3：左半边测距、4右半边测距
u8 direction=1;
static u32 distanceTmp=0;

void ultrasonicInit(void)
{
    GPIO_InitTypeDef GPIO_InitTypeStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //PB14——触发信号输入
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_14;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitTypeStructure);

    //PD8——回响信号输出
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_8;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOD,&GPIO_InitTypeStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);	//GPIO与中断线映射起来

    EXTI_InitStructure.EXTI_Line=EXTI_Line14;
    EXTI_InitStructure.EXTI_LineCmd=DISABLE;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_ResetBits(GPIOD,GPIO_Pin_8);
    ult_tim4_init(65535,71);
}

void ult_tim4_init(u16 arr,u16 psc)	//红外控制延时控制定时器初始化
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//时钟使能

    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;	//预分频
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=arr;	//重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;	//分频因子
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
}

void getDistance(void)	//获取距离
{
    GPIO_SetBits(GPIOD,GPIO_Pin_8);
    ultrasonicStatus=1;
    TIM_SetCounter(TIM4, 0);
    TIM_Cmd(TIM4, ENABLE);//开启定时器
    while(TIM_GetCounter(TIM4) < 11);	//触发信号为10us的高电平
    ultrasonicStatus=2;
    GPIO_ResetBits(GPIOD,GPIO_Pin_8);
    TIM_SetCounter(TIM4, 0);
    //模块内部循环发出 8 个 40KHz 脉冲，再到回响信号
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0) {	//等待回响信号的起始位置
        if(TIM_GetCounter(TIM4) > 60000) {	//超时
            ultrasonicStatus=0;
            distanceCM=0;
            TIM_Cmd(TIM4, DISABLE);
            return;	//失败返回
        }
    }
    TIM_SetCounter(TIM4, 0);
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==1) {	//计算
        if(TIM_GetCounter(TIM4) > 60000) {	//超时
            ultrasonicStatus=0;
            distanceCM=0;
            TIM_Cmd(TIM4, DISABLE);
            return;	//失败返回
        }
    }
    distanceTmp=TIM_GetCounter(TIM4);
    TIM_Cmd(TIM4, DISABLE);
    distanceCM=(u32)(((long)distanceTmp*34)/2000);	//单位：cm
    ultrasonicStatus=0;
//	return distanceCM;
}

void getWdistance(int n)
{
	servoAngleCtr(n);
		delay_ms(500);
		getDistance();
		if(n==5){
			allDistanceCM[0] = distanceCM;
			direction=0;
		}else if(n==15){
			allDistanceCM[1] = distanceCM;
			direction=1;
		}else if(n==25){
			allDistanceCM[2] = distanceCM;
			direction=2;
		}else if(n>5 && n<15){
			allDistanceCM[4] = distanceCM;
			direction=3;
		}else if(n>15 && n<25){
			allDistanceCM[3] = distanceCM;
			direction=4;
		}else{
			
		}
	
}
