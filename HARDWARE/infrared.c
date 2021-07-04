#include "infrared.h"

unsigned char IRdata[4];	//���յ�������
u8 IRflag=0;	//�������ݱ�־λ	0���������ݣ�1����������

void IR_init()	//������ϡ�������Ƴ�ʼ��
{
    GPIO_InitTypeDef GPIO_InitTypeStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //�������	PD10
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_10;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOD,&GPIO_InitTypeStructure);

    //��������	PG2
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_2;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOG,&GPIO_InitTypeStructure);

    //�Һ������ PC7
    GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitTypeStructure.GPIO_Pin=GPIO_Pin_7;
    GPIO_InitTypeStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitTypeStructure);

    //��ʼ��������ƿڵ��ⲿ�жϡ����ⲿ�ж��� 10
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource10);	//GPIO���ж���ӳ������

    EXTI_InitStructure.EXTI_Line=EXTI_Line10;
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//�����ж�
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;	//�½��ش���
    EXTI_Init(&EXTI_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϷ���2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_InitStructure);
}

char avoid_status(void)	//��ȡ������ϵ�״̬
{
    char left=0,right=0;	//�ݴ�״̬
    char count;	//�ɼ�����

    if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)==BARRIER_Y)	//��ȡ��������
    {
        count=2;
        while(--count)	//�˲�
        {
            if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)==BARRIER_N)
                break;
            delay_ms(1);
        }
        if(count==0)
            left=1;
    }

    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==BARRIER_Y)	//��ȡ�Һ������
    {
        count=2;
        while(--count)	//�˲�
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

void red_check(int i)		//���Ϻ���
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

void red_avoid_run(void)	//��������߷�
{
    char status;
    status = avoid_status();
    switch(status)
    {
    case AVOID_LEFT:		//������ϰ�		��ת
        carCtr(SPEED_BACK,SPEED_BACK);
        delay_ms(500);
        carCtr(SPEED_THREE,SPEED_ONE);
        red_check(300);
        break;

    case AVOID_RIGHT:		//�ұ����ϰ�		��ת
        carCtr(SPEED_BACK,SPEED_BACK);
        delay_ms(500);
        carCtr(SPEED_ONE,SPEED_THREE);
        red_check(300);
        break;

    case AVOID_BOTH:		//�������ϰ�		��ת
        carCtr(SPEED_BACK,SPEED_BACK);
        delay_ms(500);
        carCtr(SPEED_THREE,SPEED_ONE);
        red_check(300);
        break;

    case AVOID_NONE:		//���ϰ�		ֱ��
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
        while(IR_VALUE==0) {	//9ms ������,�͵�ƽ
            delay_us(100);

        }
        if(IR_VALUE==1) {

            while(IR_VALUE==1) {	//4.5ms �ߵ�ƽ
                delay_us(100);

            }
        }
        //�Ѽ����ݣ��������ݣ�һ���λ
        for(int i=0; i<4; i++) {
            for(int j=0; j<8; j++) {
                //һ���߼� 1 ������Ҫ 2.25ms��560us����+1680us �͵�ƽ���������յ� 560us �ĵ͵�ƽ�� 1680us �ĸߵ�ƽ
                //һ���߼� 0 �Ĵ�����Ҫ 1.125ms��560us ����+560us �͵�ƽ���������յ� 560us �ĵ͵�ƽ�� 560us �ĸߵ�ƽ

                while(IR_VALUE==0) {	//ǰ�� 560us �ĵ͵�ƽ
                    delay_us(10);
                }

                IRtime=0;
                while(IR_VALUE==1) {	//ͳ�Ƹߵ�ƽʱ��
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
            break;	//ǰ��
        case 0x52:
            ctrlCommand=COMM_DOWN;
            break;	//����
        case 0x08:
            ctrlCommand=COMM_LEFT;
            break;	//��ת
        case 0x5A:
            ctrlCommand=COMM_RIGHT;
            break;	//��ת
        case 0x1C:
            ctrlCommand=COMM_STOP;
            break;	//ֹͣ
        case 0x15:
            ctrlCommand=SPEED_UP;
            break;	//����
        case 0x07:
            ctrlCommand=SPEED_DOWN;
            break;	//����
        case 0x16:
            ctrlCommand=COMM_TRACK;
            break;	//ѭ������
        case 0x09:
            ctrlCommand=SWITCH_MODE;
            break;	//ģʽѡ��
        case 0x19:
            ctrlCommand=COMM_REDAVOID;
            break;	//������Ͽ���
        case 0x0d:
            ctrlCommand=COMM_ULTRASONIC;
            break;	//����������
		case 0x0C:
            ctrlCommand=TURN_LEFT;
            break;	//������תָʾ
		case 0x5E:
            ctrlCommand=TURN_RIGHT;
            break;	//������תָʾ
		case 0x42:
            ctrlCommand=COMM_LED;
            break;	//ָʾ��
		case 0x4A:
            ctrlCommand=COMM_BEEP;
            break;	//������
		case 0x44:
            ctrlCommand=SERVO_0;
            break;	//��� 0 ��
		case 0x46:
            ctrlCommand=SERVO_90;
            break;	//��� 90 ��
		case 0x43:
            ctrlCommand=SERVO_180;
            break;	//��� 180 ��
		case 0x47:
            ctrlCommand=SERVO_ADD;
            break;	//����Ӷ�����˳ʱ����ת
		case 0x45:
            ctrlCommand=SERVO_SUB;
            break;	//�������������ʱ����ת
		case 0x40:
			ctrlCommand=SUPER_PRIV;
			break;	//����Ȩ��
        default:
			ctrlCommand=COMM_NULL;
            break;	//��ָ��
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
