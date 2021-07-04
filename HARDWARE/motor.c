#include "motor.h"

//电机控制
static int left_flag=0;
static int right_flag=0;
char speed_count=0;

//舵机控制
static int servo_count=0;
static int servo_angle=15;	//5-25取值范围

void motor_init(void)	//电机初始化
{
    GPIO_InitTypeDef GPIO_InitTypeStructure;

    //左电机前进	PC11
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_11;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOC,&GPIO_InitTypeStructure);

    //左电机后退	PD0
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOD,&GPIO_InitTypeStructure);

    //右电机前进	PD6
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOD,&GPIO_InitTypeStructure);

    //右电机后退	PG9
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_9;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOG,&GPIO_InitTypeStructure);

    //左电机使能 PD4
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_4;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOD,&GPIO_InitTypeStructure);

    //右电机使能	PD2
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_2;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOD,&GPIO_InitTypeStructure);

    //舵机控制 PA0
    /*
    *舵机的控制一般需要一个 20ms 左右的时基脉冲，该脉冲的高电平部分一般为 0.5ms~2.5ms 范围内的角度控制脉冲部分
    *0.5ms------------0 度；
    *1.0ms------------45 度；
    *1.5ms------------90 度；
    *2.0ms-----------135 度；
    *2.5ms-----------180 度；
    */
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitTypeStructure);

    L293D_EN;	//电机使能
    motor_tim2_init(99,71);	//定时器配置 10KHz 0.1ms
}

void motor_tim2_init(u16 arr,u16 psc)	//PWM时钟控制初始化
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM2,ENABLE);
}

void carMove(void)	//小车运动函数
{
    //左
    if(left_flag > 0) {
        if(speed_count < left_flag) {
            LEFT_UP;
        } else {
            LEFT_STOP;
        }
    } else if(left_flag<0) {
        if(speed_count < (-1)*left_flag) {
            LEFT_DOWN;
        } else {
            LEFT_STOP;
        }
    } else {
        LEFT_STOP;
    }

    //右
    if(right_flag > 0) {
        if(speed_count < right_flag) {
            RIGHT_UP;
        } else {
            RIGHT_STOP;
        }
    } else if(right_flag < 0) {
        if(speed_count < (-1)*right_flag) {
            RIGHT_DOWN;
        } else {
            RIGHT_STOP;
        }
    } else {
        RIGHT_STOP;
    }
}

void carMoveCtr(int left,int right)	//小车运动函数,带速度控制
{
    left_flag=left;
    right_flag=right;
    //左
    if(left_flag > 0) {
        if(speed_count < left_flag) {
            LEFT_UP;
        } else {
            LEFT_STOP;
        }
    } else if(left_flag<0) {
        if(speed_count < (-1)*left_flag) {
            LEFT_DOWN;
        } else {
            LEFT_STOP;
        }
    } else {
        LEFT_STOP;
    }

    //右
    if(right_flag > 0) {
        if(speed_count < right_flag) {
            RIGHT_UP;
        } else {
            RIGHT_STOP;
        }
    } else if(right_flag < 0) {
        if(speed_count < (-1)*right_flag) {
            RIGHT_DOWN;
        } else {
            RIGHT_STOP;
        }
    } else {
        RIGHT_STOP;
    }
}

void speedCtr(int left,int right)		//速度控制
{
    if(left >= 50) {
        left = 50;
    }
    if(right >= 50) {
        right = 50;
    }
    if(left <= -50) {
        left = -50;
    }
    if(right <= -50) {
        right = -50;
    }
    left_flag=left;
    right_flag=right;
}

void carCtr(int left,int right)	//小车电机总控制，相当于接口作用
{
    left_flag=left;
    right_flag=right;
}

void carLeft(int left)	//小车左电机控制
{
    left_flag=left;
}

void carRight(int right)	//小车右电机控制
{
    right_flag=right;
}

int getLeftFlag(void)	//获取左电机值
{
    return left_flag;
}

int getRightFlag(void)	//获取右电机值
{
    return right_flag;
}

int getAngle()	//获取舵机角度值
{
	return servo_angle;
}

void servoAngleCtr(int servoAngle)	//舵机角度控制
{
    servoFlag=15;
    servo_angle=servoAngle;
}

void servoAngleADD()	//顺时针
{
	int n;
	n=getAngle();
	if(n>5 && n<25){
		servoAngleCtr(n-1);
	}
}

void servoAngleSUB()	//逆时针
{
	int n;
	n=getAngle();
	if(n>5 && n<25){
		servoAngleCtr(n+1);
	}
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET) {
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        pwm_1ms++;

        if(pwm_1ms >= 10) {
            pwm_1ms=0;
            speed_count++;
            tick_5ms++;
			
			if(super_power){
				powerCount--;
				if(!powerCount){
					super_power=0;
					redAvoidflag=1;
					//trackingflag=0;
					distanceCount=1;
				}
			}	//主动开启超级权限的倒计时
			
            if(speed_count >= 50) {
                speed_count=0;
            }
            carMove();	//运动函数
        }

        //carMove();	//运动函数

        //舵机控制
        if(servoFlag) {
            servo_count++;
            if(servo_count <= servo_angle) {
                SERVOSET;
            } else {
                SERVORESET;
            }
            if(servo_count >= 200) {
                servo_count=0;
                servoFlag--;
            }
        }
    }
}
