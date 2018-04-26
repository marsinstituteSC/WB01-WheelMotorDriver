#include "motorDriver.h"
#include "main.h"
#include "TIM_oppsett.h"
#include "math.h"
#include <stdlib.h>
#include "CANSPI.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "cmsis_os.h"
extern uint16_t farttest;
extern int32_t fartkonst;
extern SemaphoreHandle_t AckerProtHandle;
extern uint16_t farttest2;
bool retning;
//extern uint16_t fart;



void PWM_Set_Frekvens(uint16_t tempfart, uint16_t konst){





//	if(HAL_GPIO_ReadPin(DRIVE_EN_GPIO_Port,DRIVE_EN_Pin)){
		bool retning = (tempfart>>15) & 0x01;
		tempfart &= 0x7FFF;
		MOTOR_DIR_TOGGLE(retning);

		if(tempfart >= 1){


			tempfart = 10500000000 / (tempfart*5826); //*128

			if (tempfart <=55){tempfart = 55;}
			if (tempfart >= 65000){tempfart = 65000;}

			tempfart = ((tempfart*konst)/1000);

			farttest = tempfart;
			PWM_TIMER->ARR = tempfart;
			PWM_TIMER->CCR1 = (tempfart/2);
			PWM_TIMER->CR1 = 0x81;
			HAL_GPIO_WritePin(DRIVE_EN_GPIO_Port,DRIVE_EN_Pin,GPIO_PIN_SET);
		}else{
			PWM_TIMER->ARR = 10;
			PWM_TIMER->CCR1 = 10;
			PWM_TIMER->CR1 = 0x80;
		}
	}
//}

void MOTOR_DIR_TOGGLE(uint16_t dir){
	if(HAL_GPIO_ReadPin(DRIVE_DIR_GPIO_Port,DRIVE_DIR_Pin)!=dir){
		HAL_TIM_PWM_Stop(&htim,TIM_CHANNEL_1);
		MOTOR_DELAY();
		HAL_GPIO_TogglePin(DRIVE_DIR_GPIO_Port,DRIVE_DIR_Pin);
		MOTOR_DELAY();
		HAL_TIM_PWM_Start(&htim,TIM_CHANNEL_1);
	}
}

void MOTOR_STATE(uint8_t state){
	if(HAL_GPIO_ReadPin(DRIVE_EN_GPIO_Port,DRIVE_EN_Pin)!=state){     			// Hvis ny tilstand er ulik n�v�rende tilstand
		if(state){

			HAL_GPIO_WritePin(DRIVE_EN_GPIO_Port,DRIVE_EN_Pin,GPIO_PIN_SET);
			MOTOR_DELAY();
			HAL_TIM_PWM_Start(&htim,TIM_CHANNEL_1);

		} else if(!state){

			HAL_TIM_PWM_Stop(&htim,TIM_CHANNEL_1);
			PWM_TIMER->ARR = 10;
			PWM_TIMER->CCR1 = 10;
			MOTOR_DELAY();
			HAL_GPIO_WritePin(DRIVE_EN_GPIO_Port,DRIVE_EN_Pin,GPIO_PIN_RESET);

		}
	}
}
void MOTOR_DELAY(void){
	for(int i=0;i<=300;i++){
		asm("");
	}
}

