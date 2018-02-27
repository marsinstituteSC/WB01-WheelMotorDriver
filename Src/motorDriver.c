#include "motorDriver.h"
#include "main.h"
#include <math.h>

void PWM_Set_Frekvens(uint16_t tempfart){

//	bool retning = (tempfart>>15) & 0x01;
//	tempfart &= 0xEF;

//	if (retning==1) {
//		MOTOR_FRAM();
//	}
//	else{
//		MOTOR_BAK();
//	}

	if(tempfart >= 1){

//		tempfart = (65000000-1000*3215*exp(tempfart2/10922))/1000;
		tempfart = 10500000000 / (tempfart*5826*128);
		if (tempfart <=54){tempfart = 54;}
		if (tempfart >= 65000){tempfart = 65000;}

		TIM4->ARR = tempfart;
		TIM4->CCR1 = (tempfart/2);
		TIM4->CR1 = 0x81;
//		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	}else{
//		HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
//		MOTOR_DISABLE()
		TIM4->ARR = 10000;
		TIM4->CCR1 = 10000;
		TIM4->CR1 = 0x81;
	}
}
