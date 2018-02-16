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

	tempfart = 8400000000 / (tempfart * 600);

	if (tempfart <=420){tempfart = 420;}
	if (tempfart >= 65535){tempfart = 65535;}

	TIM4->ARR = tempfart;
	TIM4->CCR1 = (tempfart/2);
	TIM4->CR1 = 0x81;
}
