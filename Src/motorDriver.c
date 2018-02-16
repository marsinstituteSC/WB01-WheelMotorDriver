#include "motorDriver.h"
#include "main.h"



void PWM_Set_Frekvens(uint16_t tempfart){

	bool retning = (tempfart>>15) & 0x01;
	tempfart &= 0xEF;

	if (retning==1) {
		MOTOR_FRAM();
	}
	else{
		MOTOR_BAK();
	}

	tempfart = 84000000 / tempfart * 6;


	TIM4->ARR = 420;
	TIM4->CCR1 = (420/2);
	TIM4->CR1 = 0x81;

}
