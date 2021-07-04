#include "stm32f10x.h"
#include "sys.h"

char ctrlCommand=SWITCH_MODE;
u8 mode=3;	//模式选择
u8 redAvoidflag,trackingflag,ultrasonicflag,speedCountflag,turnLeft,turnRight;		//功能激活参数
u8 pwm_1ms=0;
u8 tick_5ms=0;
u8 tick_200ms=0;
u8 tick_2s=0;
u8 servoFlag=15;
int funN=0;
int fundisN=0;
u8 mode1_power;
u8 mode2_redTime;
int mode2_SLT,mode2_SRT;
u8 mode2_carStop;
u8 wirelessFlag;
u8 redAvoidCount=0;
u8 distanceCount=0;
u8 trackingCount=0;
u8 super_power=0;
u8 super_power_B=0;
int powerCount=0;
char Rstatus=0;	//红外结果参数

int main(void)
{
    GPIO_clock_init();	//IO口时钟使能
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组2，2位抢占优先级，2位响应优先级
    delay_init();	//延时函数初始化
    led_init();	//指示灯测试
    lcdInit();	//lcd初始化

    IR_init();	//红外避障、红外控制初始化
    tracking_init();		//三路循迹初始化
    speedCountInit();		//速度传感器初始化
    ultrasonicInit();	//超声波模块初始化

    usart1_init(115200);	//usart1初始化、波特率115200
//    usart3_init(9600);	//蓝牙串口
    motor_init();	//电机、舵机模块初始化

    while(1)
    {

        if(ctrlCommand==SWITCH_MODE) {
            IRflag=0;
            ctrlCommand=COMM_NULL;
            carCtr(SPEED_STOP,SPEED_STOP);
            mode++;
            mode %= 4;
            switch(mode)		//参数初始化
            {
            case MODE0:
                redAvoidflag=1;
                trackingflag=0;
                ultrasonicflag=1;
                speedCountflag=1;
                wirelessFlag=1;
                redAvoidCount=1;
                distanceCount=1;
                break;
            case MODE1:
                redAvoidflag=1;
                trackingflag=0;
                ultrasonicflag=1;
                speedCountflag=1;
                mode1_power=1;
                wirelessFlag=1;
                redAvoidCount=0;
                break;
            case MODE2:
                redAvoidflag=1;
                trackingflag=0;
                ultrasonicflag=1;
                speedCountflag=1;
                mode2_redTime=0;
                wirelessFlag=1;
                redAvoidCount=0;
                trackingCount=0;
                distanceCount=0;
                mode2_carStop=0;
                mode2_SLT=0;
                mode2_SRT=0;
                turnLeft=0;
                turnRight=0;
                break;
            case MODE3:
                redAvoidflag=1;
                trackingflag=0;
                ultrasonicflag=1;
                speedCountflag=1;
                mode1_power=1;
                wirelessFlag=1;
                super_power=0;
                super_power_B=0;
                distanceCount=1;
                break;
            default:
                break;
            }
        }

        switch(mode)
        {
        case MODE0:
            mode0();
            break;
        case MODE1:
            mode1();
            break;
        case MODE2:
            mode2();
            break;
        case MODE3:
            mode3();
            break;
        default:
            break;
        }
    }
}

void mode0()
{
    if(tick_5ms >= 5) {
        tick_5ms=0;

        funN %= 2;
        if(funN==0 && redAvoidflag) {
            Rstatus=avoid_status();
            if(Rstatus && redAvoidCount) {		//报警三次
                redAvoidCount--;
                beep_MODE(0,150,150);
            }
            if(!Rstatus) {		//脱离后重置报警次数
                redAvoidCount=3;
            }
        }
        if(funN==1 && ultrasonicflag) {
            getDistance();
            if(distanceCM <= 30 && distanceCount) {
                distanceCount--;
                led_MODE(0,300,300);
            }
            if(distanceCM >= 30) {
                distanceCount=3;
            }
        }
        funN++;

        tick_200ms++;
        if(tick_200ms >= 40) {		//200ms刷新一次信息
            tick_200ms=0;

            tick_2s++;
            if(tick_2s >= 10) {		//2s切换一次显示的信息
                tick_2s=0;
                fundisN++;
                if(trackingflag) {
                    fundisN %= 3;
                } else {
                    fundisN %= 2;
                }
                lcdClear();		//清屏
            }

            if(fundisN==0 && ultrasonicflag) {
                lcdWriteDistance(distanceCM);
            }
            if(fundisN==1 && speedCountflag) {
                lcdWriteSpeed(leftSpeed,rightSpeed);
            }
            if(fundisN==2 && trackingflag) {
                lcdWriteTrackMsg(trackStatus);
            }
        }

        if(IRflag==1) {
            IRflag=0;

            switch(ctrlCommand)
            {
            case COMM_UP:
                if(getLeftFlag() > 0 || getRightFlag() > 0) {
                    if(getLeftFlag() >= getRightFlag()) {
                        carRight(getLeftFlag());
                    } else {
                        carLeft(getRightFlag());
                    }
                } else {
                    carCtr(SPEED_ONE,SPEED_ONE);
                }
                break;

            case COMM_DOWN:
                if(getLeftFlag() < 0 || getRightFlag() < 0) {
                    if(getLeftFlag() <= getRightFlag()) {
                        carRight(getLeftFlag());
                    } else {
                        carLeft(getRightFlag());
                    }
                } else {
                    carCtr(SPEED_BACK,SPEED_BACK);
                }
                break;

            case COMM_LEFT:
                if((getLeftFlag()-10) < 0) {
                    carLeft(0);
                } else {
                    carLeft(getLeftFlag()-10);
                }
                carCtr(getLeftFlag(),getRightFlag());
                break;

            case COMM_RIGHT:
                if((getRightFlag()-10) < 0) {
                    carRight(0);
                } else {
                    carRight(getRightFlag()-10);
                }
                carCtr(getLeftFlag(),getRightFlag());
                break;

            case COMM_STOP:
                carCtr(SPEED_STOP,SPEED_STOP);
                break;

            case SPEED_UP:
                speedCtr(getLeftFlag()+5,getRightFlag()+5);
                break;

            case SPEED_DOWN:
                speedCtr(getLeftFlag()-5,getRightFlag()-5);
                break;

            case COMM_TRACK:
                if(trackingflag) {
                    trackingflag=0;
                } else {
                    trackingflag=1;
                }
                break;

            case COMM_REDAVOID:
                if(redAvoidflag) {
                    redAvoidflag=0;
                } else {
                    redAvoidflag=1;
                }
                break;

            case COMM_ULTRASONIC:
                if(ultrasonicflag) {
                    ultrasonicflag=0;
                } else {
                    ultrasonicflag=1;
                }
                break;

            case COMM_LED:
                led_MODE(0,250,250);
                break;

            case COMM_BEEP:
                beep_MODE(0,100,100);
                break;

            case SERVO_0:
                SERVO0;
                break;

            case SERVO_90:
                SERVO90;
                break;

            case SERVO_180:
                SERVO180;
                break;

            case SERVO_ADD:
                SERVOADD;
                break;

            case SERVO_SUB:
                SERVOSUB;
                break;

            default:
                break;
            }
        }
    }
    if(trackingflag) {		//循迹
        trackingSearch();

        if(trackStatus==ONLINE) {

        } else if(trackStatus==ONLEFT) {

        } else if(trackStatus==ONRIGHT) {

        } else {

        }
    }

}

void mode1()
{
    if(IRflag==1) {
        IRflag=0;
        switch(ctrlCommand)
        {
        case COMM_STOP:
            if(mode1_power) {
                mode1_power=0;
                if(Rstatus) {
                    carCtr(SPEED_BACK,SPEED_BACK);
                    delay_ms(1000);
                }

                carCtr(SPEED_ONE,SPEED_ONE);

            } else {
                carCtr(SPEED_STOP,SPEED_STOP);
                mode1_power=1;
            }
            break;

        case COMM_TRACK:
            if(trackingflag) {
                trackingflag=0;
            } else {
                trackingflag=1;
            }
            break;

        case COMM_REDAVOID:
            if(redAvoidflag) {
                redAvoidflag=0;
            } else {
                redAvoidflag=1;
            }
            break;

        case COMM_ULTRASONIC:
            if(ultrasonicflag) {
                ultrasonicflag=0;
            } else {
                ultrasonicflag=1;
            }
            break;

        case COMM_LED:
            led_MODE(0,250,250);
            break;

        case COMM_BEEP:
            beep_MODE(0,100,100);
            break;

        case SERVO_0:
            SERVO0;
            break;

        case SERVO_90:
            SERVO90;
            break;

        case SERVO_180:
            SERVO180;
            break;

        case SERVO_ADD:
            SERVOADD;
            break;

        case SERVO_SUB:
            SERVOSUB;
            break;

        default:
            break;
        }
    }

    if(tick_5ms >= 5) {
        tick_5ms=0;

        funN %= 2;
        if(funN==0 && redAvoidflag && !trackingflag && !mode1_power) {
            red_avoid_run();
            //printf("%d\n",redAvoidCount);
            if(redAvoidCount>=3) {
                carCtr(SPEED_BACK,SPEED_BACK);
                delay_ms(1000);
                carCtr(SPEED_STOP,SPEED_STOP);

                GET_D_0;
                GET_D_180;
                GET_D_90;

                if(allDistanceCM[0]>allDistanceCM[2]) {
                    carCtr(SPEED_STOP,SPEED_THREE);
                } else {
                    carCtr(SPEED_THREE,SPEED_STOP);
                }
                while(avoid_status());
                carCtr(SPEED_ONE,SPEED_ONE);
                redAvoidCount=0;
            }
        }
        if(funN==1 && ultrasonicflag && avoid_status()==0 && !mode1_power) {
            getDistance();
            if(distanceCM <= 5 && distanceCM > 0) {
                carCtr(SPEED_STOP,SPEED_STOP);
                beep_MODE(0,150,150);
            }
            else if(distanceCM > 20 && distanceCM <= 50) {
                carCtr(SPEED_ONE,SPEED_ONE);
            } else if(distanceCM >50 && distanceCM <=150) {
                carCtr(SPEED_TWO,SPEED_TWO);
            } else {
                carCtr(SPEED_THREE,SPEED_THREE);
            }
        }
        funN++;

        tick_200ms++;
        if(tick_200ms >= 40) {
            tick_200ms=0;

            tick_2s++;
            if(tick_2s >= 10) {
                tick_2s=0;
                fundisN++;
                if(trackingflag) {
                    fundisN %= 3;
                } else {
                    fundisN %= 2;
                }
                lcdClear();
            }

            if(fundisN==0 && ultrasonicflag) {
                lcdWriteDistance(distanceCM);
            }
            if(fundisN==1 && speedCountflag) {
                lcdWriteSpeed(leftSpeed,rightSpeed);
            }
            if(fundisN==2 && trackingflag) {
                lcdWriteTrackMsg(trackStatus);
            }
        }
    }

    if(trackingflag && !mode1_power) {		//循迹
        trackingSearch();
        switch(trackStatus)
        {
        case ONLINE:	//路径上
            carCtr(SPEED_TWO,SPEED_TWO);
            break;
        case ONRIGHT:	//路径的右边，往左转回到路径上
            carCtr(SPEED_ONE,SPEED_TWO);
            break;
        case ONLEFT:	//路径的左边，往右转回到路径上
            carCtr(SPEED_TWO,SPEED_ONE);
            break;
        default:
            break;
        }
    }
}

void mode2()
{
    if(tick_5ms >= 5) {
        tick_5ms=0;

        funN %= 2;
        if(funN==0 && redAvoidflag) {
            Rstatus=avoid_status();
            if(Rstatus) {
                if(!redAvoidCount) {
                    mode2_SLT=getLeftFlag();
                    mode2_SRT=getRightFlag();
                    redAvoidCount=1;
                }
                wirelessFlag=0;

                switch(Rstatus)
                {
                case AVOID_LEFT:		//左边有障碍

                    carCtr(SPEED_BACK,SPEED_BACK);
                    delay_ms(500);
                    carCtr(SPEED_THREE,SPEED_ONE);
                    red_check(300);

                    break;

                case AVOID_RIGHT:		//右边有障碍

                    carCtr(SPEED_BACK,SPEED_BACK);
                    delay_ms(500);
                    carCtr(SPEED_ONE,SPEED_THREE);
                    red_check(300);

                    break;

                case AVOID_BOTH:		//左右有障碍

                    carCtr(SPEED_BACK,SPEED_BACK);
                    delay_ms(500);
                    carCtr(SPEED_THREE,SPEED_ONE);
                    red_check(300);

                    break;
                default:
                    break;
                }
            } else {
                wirelessFlag=1;
                redAvoidCount=0;
				mode2_carStop=1;
				
                //carCtr(SPEED_STOP,SPEED_STOP);
            }
        }

        if(funN==1 && ultrasonicflag) {
            getDistance();
            if(distanceCount && mode2_carStop) {
                if(distanceCM>=10 && distanceCM<100) {
                    if((mode2_SLT-5)>10) {
                        mode2_SLT-=5;
                    }
                    if((mode2_SRT-5)>10) {
                        mode2_SRT-=5;
                    }
                } else if(distanceCM>=100 && distanceCM<200) {
                    if((mode2_SLT+5)<35) {
                        mode2_SLT+=5;
                    }
                    if((mode2_SRT+5)<35) {
                        mode2_SRT+=5;
                    }
                } else {
                    if((mode2_SLT+10)<50) {
                        mode2_SLT+=5;
                    }
                    if((mode2_SRT+10)<50) {
                        mode2_SRT+=5;
                    }
                }
            }
        }
        funN++;

        tick_200ms++;
        if(tick_200ms >= 40) {
            tick_200ms=0;

            tick_2s++;
            if(tick_2s >= 10) {
                tick_2s=0;
                fundisN++;
//                if(trackingflag) {
//                    fundisN %= 3;
//                } else {
                fundisN %= 2;
//                }
                lcdClear();
            }

            if(fundisN==0 && ultrasonicflag) {
                lcdWriteDistance(distanceCM);
            }
            if(fundisN==1 && speedCountflag) {
                lcdWriteSpeed(leftSpeed,rightSpeed);
            }
//            if(fundisN==2 && trackingflag) {
//                lcdWriteTrackMsg(trackStatus);
//            }
        }

        mode2_redTime--;//500ms 无接收到行动指令就停车
        if(mode2_redTime == 0) {
            mode2_carStop=0;
            mode2_redTime = 1;
			if(mode2_carStop){
				mode2_SLT=getLeftFlag();
				mode2_SRT=getRightFlag();
				carCtr(SPEED_STOP,SPEED_STOP);
				//printf("%d\t%d\n",mode2_SLT,mode2_SRT);
			}
        }

        if(IRflag==1) {
            IRflag=0;
			//printf("进入控制\n");
            switch(ctrlCommand)
            {
            case COMM_UP:
				//printf("前进\n");
                if(mode2_SLT > 0 || mode2_SRT > 0) {
                    if(mode2_SLT >= mode2_SRT) {
                        mode2_SRT = mode2_SLT;
                        carCtr(mode2_SLT,mode2_SRT);
                    } else {
                        mode2_SLT = mode2_SRT;
                        carCtr(mode2_SLT,mode2_SRT);
                    }
                } else {
					//printf("初速度\n");
                    carCtr(SPEED_ONE,SPEED_ONE);
                }
                mode2_carStop=1;
                break;

            case COMM_DOWN:
                if(mode2_SLT < 0 || mode2_SRT < 0) {
                    if(mode2_SLT <= mode2_SRT) {
                        mode2_SRT = mode2_SLT;
                        carCtr(mode2_SLT,mode2_SRT);
                    } else {
                        mode2_SLT = mode2_SRT;
                        carCtr(mode2_SLT,mode2_SRT);
                    }
                } else {
                    carCtr(SPEED_BACK,SPEED_BACK);
                }
                mode2_carStop=1;
                break;

            case COMM_LEFT:
                if((mode2_SLT-10) < 0) {
                    mode2_SLT = 0;
                    carCtr(mode2_SLT,mode2_SRT);
                } else {
                    mode2_SLT -= 10;
                    carCtr(mode2_SLT,mode2_SRT);
                }
                mode2_carStop=1;
                break;

            case COMM_RIGHT:
                if(mode2_SRT-10 < 0) {
                    mode2_SRT=0;
                    carCtr(mode2_SLT,mode2_SRT);
                } else {
                    mode2_SRT -= 10;
                    carCtr(mode2_SLT,mode2_SRT);
                }
                mode2_carStop=1;
                break;

            case COMM_STOP:
                carCtr(SPEED_STOP,SPEED_STOP);
                mode2_carStop=1;
                break;

            case SPEED_UP:
                speedCtr(mode2_SLT+5,mode2_SRT+5);
                mode2_carStop=1;
                break;

            case SPEED_DOWN:
                speedCtr(mode2_SLT-5,mode2_SRT-5);
                mode2_carStop=1;
                break;

            case COMM_TRACK:
                if(trackingflag) {
                    trackingflag=0;
                } else {
                    trackingflag=1;
                }
                break;

            case COMM_REDAVOID:
                if(redAvoidflag) {
                    redAvoidflag=0;
                } else {
                    redAvoidflag=1;
                }
                break;

            case COMM_ULTRASONIC:
                if(distanceCount) {
                    distanceCount=0;
                } else {
                    distanceCount=1;
                }
                break;

            case TURN_LEFT:
                if(turnLeft) {
                    turnLeft=0;
                } else {
                    turnLeft=1;
                }
                break;

            case TURN_RIGHT:
                if(turnRight) {
                    turnRight=0;
                } else {
                    turnRight=1;
                }
                break;

            case COMM_LED:
                led_MODE(0,250,250);
                break;

            case COMM_BEEP:
                beep_MODE(0,100,100);
                break;

            case SERVO_0:
                SERVO0;
                break;

            case SERVO_90:
                SERVO90;
                break;

            case SERVO_180:
                SERVO180;
                break;

            case SERVO_ADD:
                SERVOADD;
                break;

            case SERVO_SUB:
                SERVOSUB;
                break;

            default:
                break;
            }
        }
    }

    if(trackingflag) {		//循迹
        //trackingSearch();      
            if((TRACKING_LEFT==BLACK && (TRACKING_RIGHT!=BLACK && TRACKING_MIDDLE!=BLACK)) || ((TRACKING_LEFT==BLACK && TRACKING_MIDDLE==BLACK) && TRACKING_RIGHT!=BLACK)) {	//左边压线
                if(!trackingCount && !(turnLeft || turnRight)) {
                mode2_SLT=getLeftFlag();
                mode2_SRT=getRightFlag();
                trackingCount=1;
				}
				wirelessFlag=0;
				if(mode2_SRT-5 < 0) {

                    carCtr(mode2_SLT,0);
                } else {

                    carCtr(mode2_SLT,(mode2_SRT-5));
                }
            }
//			else{
//				wirelessFlag=1;
//				trackingCount=0;
//			}
			
            else if((TRACKING_RIGHT==BLACK && (TRACKING_LEFT!=BLACK && TRACKING_MIDDLE!=BLACK)) || ((TRACKING_RIGHT==BLACK && TRACKING_MIDDLE==BLACK) && TRACKING_LEFT!=BLACK)) {		//右边压线
                if(!trackingCount && !(turnLeft || turnRight)) {
                mode2_SLT=getLeftFlag();
                mode2_SRT=getRightFlag();
                trackingCount=1;
				}
				wirelessFlag=0;
				
				if((mode2_SLT-5) < 0) {

                    carCtr(0,mode2_SRT);
                } else {

                    carCtr((mode2_SLT-5),mode2_SRT);
                }
            }
			
			else{
				wirelessFlag=1;
				trackingCount=0;
			}
    }
}

void mode3()
{
    if(IRflag==1 && !(super_power || super_power_B)) {
        IRflag=0;
        switch(ctrlCommand)
        {
        case COMM_STOP:
            if(mode1_power) {
                mode1_power=0;
                if(Rstatus) {
                    carCtr(SPEED_BACK,SPEED_BACK);
                    delay_ms(1000);
                }

                carCtr(SPEED_ONE,SPEED_ONE);

            } else {
                carCtr(SPEED_STOP,SPEED_STOP);
                mode1_power=1;
            }
            break;

        case COMM_TRACK:
            if(trackingflag) {
                trackingflag=0;
            } else {
                trackingflag=1;
            }
            break;

        case COMM_REDAVOID:
            if(redAvoidflag) {
                redAvoidflag=0;
            } else {
                redAvoidflag=1;
            }
            break;

        case COMM_ULTRASONIC:
            if(ultrasonicflag) {
                distanceCount=0;
            } else {
                distanceCount=1;
            }
            break;

        case COMM_LED:
            led_MODE(0,250,250);
            break;

        case COMM_BEEP:
            beep_MODE(0,100,100);
            break;

        case SUPER_PRIV:
            if(super_power_B) {
                super_power_B=0;
                redAvoidflag=1;
                distanceCount=1;
            } else {
                super_power=1;
                powerCount=10000;
                redAvoidflag=0;
                trackingflag=0;
                distanceCount=0;
            }
            break;

        case SERVO_0:
            SERVO0;
            break;

        case SERVO_90:
            SERVO90;
            break;

        case SERVO_180:
            SERVO180;
            break;

        case SERVO_ADD:
            SERVOADD;
            break;

        case SERVO_SUB:
            SERVOSUB;
            break;

        default:
            break;
        }
    }

    if(IRflag==1 && (super_power || super_power_B)) {
        IRflag=0;
        //printf("super power\n");
        switch(ctrlCommand)
        {
        case COMM_UP:
            if(getLeftFlag() > 0 || getRightFlag() > 0) {
                if(getLeftFlag() >= getRightFlag()) {
                    carRight(getLeftFlag());
                } else {
                    carLeft(getRightFlag());
                }
            } else {
                carCtr(SPEED_ONE,SPEED_ONE);
            }
            break;

        case COMM_DOWN:
            if(getLeftFlag() < 0 || getRightFlag() < 0) {
                if(getLeftFlag() <= getRightFlag()) {
                    carRight(getLeftFlag());
                } else {
                    carLeft(getRightFlag());
                }
            } else {
                carCtr(SPEED_BACK,SPEED_BACK);
            }
            break;

        case COMM_LEFT:
            if((getLeftFlag()-10) < 0) {
                carLeft(0);
            } else {
                carLeft(getLeftFlag()-10);
            }
            carCtr(getLeftFlag(),getRightFlag());
            break;

        case COMM_RIGHT:
            if((getRightFlag()-10) < 0) {
                carRight(0);
            } else {
                carRight(getRightFlag()-10);
            }
            carCtr(getLeftFlag(),getRightFlag());
            break;

        case COMM_STOP:
            if(mode1_power) {
                mode1_power=0;
            } else {
                mode1_power=1;
            }
            carCtr(SPEED_STOP,SPEED_STOP);
            break;

        case SPEED_UP:
            speedCtr(getLeftFlag()+5,getRightFlag()+5);
            break;

        case SPEED_DOWN:
            speedCtr(getLeftFlag()-5,getRightFlag()-5);
            break;

        default:
            break;
        }
    }

    if(tick_5ms >= 5) {
        tick_5ms=0;

        funN %= 2;
        if(funN==0 && redAvoidflag && !trackingflag && !mode1_power) {
            red_avoid_run();
            //printf("%d\n",redAvoidCount);
            if(redAvoidCount>=3) {
                carCtr(SPEED_BACK,SPEED_BACK);
                delay_ms(1000);
                carCtr(SPEED_STOP,SPEED_STOP);
                super_power_B=1;

                redAvoidflag=0;
                distanceCount=0;
            }
        }

        if(!avoid_status() && super_power_B) {
            super_power_B=0;
            redAvoidflag=1;
            distanceCount=1;
            carCtr(SPEED_ONE,SPEED_ONE);
        }

        if(funN==1 && ultrasonicflag && avoid_status()==0 && !mode1_power) {
            getDistance();
            if(distanceCount) {
                if(distanceCM <= 5) {
                    carCtr(SPEED_STOP,SPEED_STOP);
                    beep_MODE(0,150,150);
                }
                else if(distanceCM > 5 && distanceCM <= 50) {
                    carCtr(SPEED_ONE,SPEED_ONE);
                } else if(distanceCM >50 && distanceCM <=150) {
                    carCtr(SPEED_TWO,SPEED_TWO);
                } else {
                    carCtr(SPEED_THREE,SPEED_THREE);
                }
            }
        }
        funN++;

        tick_200ms++;
        if(tick_200ms >= 40) {
            tick_200ms=0;

            tick_2s++;
            if(tick_2s >= 10) {
                tick_2s=0;
                fundisN++;
                if(trackingflag) {
                    fundisN %= 3;
                } else {
                    fundisN %= 2;
                }
                lcdClear();
            }

            if(fundisN==0 && ultrasonicflag) {
                lcdWriteDistance(distanceCM);
            }
            if(fundisN==1 && speedCountflag) {
                lcdWriteSpeed(leftSpeed,rightSpeed);
            }
            if(fundisN==2 && trackingflag) {
                lcdWriteTrackMsg(trackStatus);
            }
        }
    }

    if(trackingflag && !mode1_power) {		//循迹
        trackingSearch();
        switch(trackStatus)
        {
        case ONLINE:	//路径上
            carCtr(SPEED_TWO,SPEED_TWO);
            break;
        case ONRIGHT:	//路径的右边，往左转回到路径上
            carCtr(SPEED_ONE,SPEED_TWO);
            break;
        case ONLEFT:	//路径的左边，往右转回到路径上
            carCtr(SPEED_TWO,SPEED_ONE);
            break;
        default:
            break;
        }
    }
}
