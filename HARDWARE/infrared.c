#include "infrared.h"

unsigned char IRdata[4];	//接收到的数据
u8 IRflag=0;	//接收数据标志位	0：无新数据，1：有新数据

void IR_init()	//红外避障、红外控制初始化
{
    GPIO_InitTypeDef GPIO_InitTypeStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //红外控制	PD10
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_10;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOD,&GPIO_InitTypeStructure);

    //左红外避障	PG2
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_2;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOG,&GPIO_InitTypeStructure);

    //右红外避障 PC7
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_7;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitTypeStructure);

    //初始化红外控制口的外部中断——外部中断线 10
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource10);	//GPIO与中断线映射起来

    EXTI_InitStructure.EXTI_Line=EXTI_Line10;
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//触发中断
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;	//下降沿触发
    EXTI_Init(&EXTI_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组2，2位抢占优先级，2位响应优先级
    NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_InitStructure);
}

char avoid_status(void)	//获取红外避障的状态
{
    char left=0,right=0;	//暂存状态
    char count;	//采集次数

    if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)==BARRIER_Y)	//读取左红外避障
    {
        count=2;
        while(--count)	//滤波
        {
            if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)==BARRIER_N)
                break;
            delay_ms(1);
        }
        if(count==0)
            left=1;
    }

    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==BARRIER_Y)	//读取右红外避障
    {
        count=2;
        while(--count)	//滤波
        {
            if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==BARRIER_N)
                break;
            delay_ms(1);
        }
        if(count==0)
            right=2;
    }
    return left+right;
}

void red_check(int i)		//避障后检查
{
    while(i--) {
        delay_ms(1);
        if(avoid_status() != AVOID_NONE) {
			redAvoidCount++;
            carCtr(SPEED_STOP,SPEED_STOP);
            return;
        }
    }
}

void red_avoid_run(void)	//红外避障走法
{
    char status;
    status = avoid_status();
    switch(status)
    {
    case AVOID_LEFT:		//左边有障碍		右转
        carCtr(SPEED_BACK,SPEED_BACK);
        delay_ms(500);
        carCtr(SPEED_THREE,SPEED_ONE);
        red_check(300);
        break;

    case AVOID_RIGHT:		//右边有障碍		左转
        carCtr(SPEED_BACK,SPEED_BACK);
        delay_ms(500);
        carCtr(SPEED_ONE,SPEED_THREE);
        red_check(300);
        break;

    case AVOID_BOTH:		//左右有障碍		右转
        carCtr(SPEED_BACK,SPEED_BACK);
        delay_ms(500);
        carCtr(SPEED_THREE,SPEED_ONE);
        red_check(300);
        break;

    case AVOID_NONE:		//无障碍		直行
        carCtr(getLeftFlag(),getRightFlag());
        break;

    default:
        break;
    }
}

void red_scan(void)
{
    u16 IRtime;
    delay_ms(3);
	
	if(IR_VALUE==1){
		return;
	}
	
	mode2_redTime=100;
	
    if(IR_VALUE==0) {
        while(IR_VALUE==0) {	//9ms 引导码,低电平
            delay_us(100);

        }
        if(IR_VALUE==1) {

            while(IR_VALUE==1) {	//4.5ms 高电平
                delay_us(100);

            }
        }
        //搜集数据，四组数据，一组八位
        for(int i=0; i<4; i++) {
            for(int j=0; j<8; j++) {
                //一个逻辑 1 传输需要 2.25ms（560us脉冲+1680us 低电平）——接收到 560us 的低电平和 1680us 的高电平
                //一个逻辑 0 的传输需要 1.125ms（560us 脉冲+560us 低电平）——接收到 560us 的低电平和 560us 的高电平

                while(IR_VALUE==0) {	//前面 560us 的低电平
                    delay_us(10);
                }

                IRtime=0;
                while(IR_VALUE==1) {	//统计高电平时间
                    delay_us(100);
                    IRtime++;

                    if(IRtime>20) {
                        return;
                    }
                }
                IRdata[i]>>=1;
                if(IRtime>=8) {
                    IRdata[i] |= 0x80;
                }
                IRtime=0;
            }
        }

//		if(IRdata[2] != ~IRdata[3]){
//			return;
//		}
        printf("%x\n",IRdata[2]);
        switch(IRdata[2])
        {
        case 0x18:
            ctrlCommand=COMM_UP;
            break;	//前进
        case 0x52:
            ctrlCommand=COMM_DOWN;
            break;	//后退
        case 0x08:
            ctrlCommand=COMM_LEFT;
            break;	//左转
        case 0x5A:
            ctrlCommand=COMM_RIGHT;
            break;	//右转
        case 0x1C:
            ctrlCommand=COMM_STOP;
            break;	//停止
        case 0x15:
            ctrlCommand=SPEED_UP;
            break;	//加速
        case 0x07:
            ctrlCommand=SPEED_DOWN;
            break;	//减速
        case 0x16:
            ctrlCommand=COMM_TRACK;
            break;	//循迹开关
        case 0x09:
            ctrlCommand=SWITCH_MODE;
            break;	//模式选择
        case 0x19:
            ctrlCommand=COMM_REDAVOID;
            break;	//红外避障开关
        case 0x0d:
            ctrlCommand=COMM_ULTRASONIC;
            break;	//超声波开关
		case 0x0C:
            ctrlCommand=TURN_LEFT;
            break;	//开启左转指示
		case 0x5E:
            ctrlCommand=TURN_RIGHT;
            break;	//开启右转指示
		case 0x42:
            ctrlCommand=COMM_LED;
            break;	//指示灯
		case 0x4A:
            ctrlCommand=COMM_BEEP;
            break;	//警报铃
		case 0x44:
            ctrlCommand=SERVO_0;
            break;	//舵机 0 度
		case 0x46:
            ctrlCommand=SERVO_90;
            break;	//舵机 90 度
		case 0x43:
            ctrlCommand=SERVO_180;
            break;	//舵机 180 度
		case 0x47:
            ctrlCommand=SERVO_ADD;
            break;	//舵机加度数，顺时针旋转
		case 0x45:
            ctrlCommand=SERVO_SUB;
            break;	//舵机减度数，逆时针旋转
		case 0x40:
			ctrlCommand=SUPER_PRIV;
			break;	//超级权限
        default:
			ctrlCommand=COMM_NULL;
            break;	//空指令
        }
        IRflag=1;
    }
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line10)!=RESET) {
        EXTI_ClearITPendingBit(EXTI_Line10);
		if(wirelessFlag){
			red_scan();
		}
    }
}
