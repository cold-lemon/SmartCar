#include "motor.h"

//�������
static int left_flag=0;
static int right_flag=0;
char speed_count=0;

//�������
static int servo_count=0;
static int servo_angle=15;	//5-25ȡֵ��Χ

void motor_init(void)	//�����ʼ��
{
    GPIO_InitTypeDef GPIO_InitTypeStructure;

    //����ǰ��	PC11
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_11;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOC,&GPIO_InitTypeStructure);

    //��������	PD0
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOD,&GPIO_InitTypeStructure);

    //�ҵ��ǰ��	PD6
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOD,&GPIO_InitTypeStructure);

    //�ҵ������	PG9
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_9;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOG,&GPIO_InitTypeStructure);

    //����ʹ�� PD4
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_4;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOD,&GPIO_InitTypeStructure);

    //�ҵ��ʹ��	PD2
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_2;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOD,&GPIO_InitTypeStructure);

    //������� PA0
    /*
    *����Ŀ���һ����Ҫһ�� 20ms ���ҵ�ʱ�����壬������ĸߵ�ƽ����һ��Ϊ 0.5ms~2.5ms ��Χ�ڵĽǶȿ������岿��
    *0.5ms------------0 �ȣ�
    *1.0ms------------45 �ȣ�
    *1.5ms------------90 �ȣ�
    *2.0ms-----------135 �ȣ�
    *2.5ms-----------180 �ȣ�
    */
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitTypeStructure);

    L293D_EN;	//���ʹ��
    motor_tim2_init(99,71);	//��ʱ������ 10KHz 0.1ms
}

void motor_tim2_init(u16 arr,u16 psc)	//PWMʱ�ӿ��Ƴ�ʼ��
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

void carMove(void)	//С���˶�����
{
    //��
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

    //��
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

void carMoveCtr(int left,int right)	//С���˶�����,���ٶȿ���
{
    left_flag=left;
    right_flag=right;
    //��
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

    //��
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

void speedCtr(int left,int right)		//�ٶȿ���
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

void carCtr(int left,int right)	//С������ܿ��ƣ��൱�ڽӿ�����
{
    left_flag=left;
    right_flag=right;
}

void carLeft(int left)	//С����������
{
    left_flag=left;
}

void carRight(int right)	//С���ҵ������
{
    right_flag=right;
}

int getLeftFlag(void)	//��ȡ����ֵ
{
    return left_flag;
}

int getRightFlag(void)	//��ȡ�ҵ��ֵ
{
    return right_flag;
}

int getAngle()	//��ȡ����Ƕ�ֵ
{
	return servo_angle;
}

void servoAngleCtr(int servoAngle)	//����Ƕȿ���
{
    servoFlag=15;
    servo_angle=servoAngle;
}

void servoAngleADD()	//˳ʱ��
{
	int n;
	n=getAngle();
	if(n>5 && n<25){
		servoAngleCtr(n-1);
	}
}

void servoAngleSUB()	//��ʱ��
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
			}	//������������Ȩ�޵ĵ���ʱ
			
            if(speed_count >= 50) {
                speed_count=0;
            }
            carMove();	//�˶�����
        }

        //carMove();	//�˶�����

        //�������
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
