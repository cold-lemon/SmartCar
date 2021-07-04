#include "tracking.h"

u8 trackStatus=0;

void tracking_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //PG4		PG8		PG6
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOG,&GPIO_InitStructure);
}

void trackingSearch(void)		//»ñÈ¡Ñ­¼£×´Ì¬
{
    if(TRACKING_LEFT==BLACK && TRACKING_MIDDLE==BLACK && TRACKING_RIGHT==BLACK){
        trackStatus = ONLINE;
    }else if(TRACKING_LEFT!=BLACK && TRACKING_MIDDLE==BLACK && TRACKING_RIGHT!=BLACK){
		trackStatus = ONLINE;
	}else if(TRACKING_LEFT==BLACK || (TRACKING_LEFT==BLACK && TRACKING_MIDDLE==BLACK)) {
        trackStatus = ONRIGHT;
	}else if(TRACKING_RIGHT==BLACK || (TRACKING_RIGHT==BLACK && TRACKING_MIDDLE==BLACK)){
        trackStatus = ONLEFT;
	}else{
		trackStatus = 0;
	}
}
