#include "motorDriver.h"
#include "main.h"
#include "TIM_oppsett.h"
#include "math.h"

extern int32_t fartkonst;
//extern uint16_t fart;



void PWM_Set_Frekvens(uint16_t tempfart){

	bool retning = (tempfart>>15) & 0x01;
	tempfart &= 0x7FFF;

	MOTOR_DIR_TOGGLE(retning);

	if(tempfart >= 1){

		tempfart = 10500000000 / (tempfart*5826); //*128
		if (tempfart <=55){tempfart = 55;}
		if (tempfart >= 65000){tempfart = 65000;}

		if((fartkonst<=500)||(fartkonst>=1500)){
			tempfart = tempfart;
		}else if ((fartkonst<=-1500)||(fartkonst>=-500)){
			tempfart = tempfart;
		}else
		{
			if(fartkonst<=0){
				tempfart = (uint16_t) ((tempfart*(-fartkonst))/1000);
			}else{
				tempfart = (uint16_t) ((tempfart*(fartkonst))/1000);
			}
		}
//		fart = tempfart;
		PWM_TIMER->ARR = tempfart;
		PWM_TIMER->CCR1 = (tempfart/2);
		PWM_TIMER->CR1 = 0x81;
	}else{
		PWM_TIMER->ARR = 10000;
		PWM_TIMER->CCR1 = 10000;
		PWM_TIMER->CR1 = 0x81;
	}
}

void MOTOR_DIR_TOGGLE(uint16_t dir){
	if(HAL_GPIO_ReadPin(DRIVE_DIR_GPIO_Port,DRIVE_DIR_Pin)!=dir){
		HAL_TIM_PWM_Stop(&htim,TIM_CHANNEL_1);
		MOTOR_DELAY();
		HAL_GPIO_TogglePin(DRIVE_DIR_GPIO_Port,DRIVE_DIR_Pin);
		MOTOR_DELAY();
		HAL_TIM_PWM_Start(&htim,TIM_CHANNEL_1);
	}
}
void MOTOR_DELAY(void){
	for(int i=0;i<=300;i++){
		asm("");
	}
}

