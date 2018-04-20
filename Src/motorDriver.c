#include "motorDriver.h"
#include "main.h"
#include "TIM_oppsett.h"
#include "math.h"
#include <stdlib.h>
#include "CANSPI.h"
extern int32_t fartkonst;
bool retning;
//extern uint16_t fart;



void PWM_Set_Frekvens(uint16_t tempfart){


	if(HAL_GPIO_ReadPin(DRIVE_EN_GPIO_Port,DRIVE_EN_Pin)){

		bool retning = (tempfart>>15) & 0x01;
		tempfart &= 0x7FFF;

		MOTOR_DIR_TOGGLE(retning);

//		uCAN_MSG txmessage;
//		txmessage.frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
//		txmessage.frame.dlc = 8;
//		txmessage.frame.data0 = tempfart >> 8 ;
//		txmessage.frame.data1 = tempfart;
//		CANSPI_Transmit(&txmessage);

		if(tempfart >= 1){

			tempfart = 10500000000 / (tempfart*5826); //*128

			if (tempfart <=55){tempfart = 55;}
//			if (tempfart <=55){tempfart = 55;}
			if (tempfart >= 65000){tempfart = 65000;}

//			if((fartkonst<=500)||(fartkonst>=1500)){
//				tempfart = tempfart;
//			}else if ((fartkonst<=-1500)||(fartkonst>=-500)){
//				tempfart = tempfart;
//			}else
//			{
//				if(fartkonst<=0){
//					tempfart = (uint16_t) ((tempfart*(-fartkonst))/1000);
//				}else{
//					tempfart = (uint16_t) ((tempfart*(fartkonst))/1000);
//				}

	//		fart = tempfart;
			PWM_TIMER->ARR = tempfart;
			PWM_TIMER->CCR1 = (tempfart/2);
			PWM_TIMER->CR1 = 0x81;
		}else{
			PWM_TIMER->ARR = 10;
			PWM_TIMER->CCR1 = 10;
			PWM_TIMER->CR1 = 0x81;
		}
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

void MOTOR_STATE(uint8_t state){
	if(HAL_GPIO_ReadPin(DRIVE_EN_GPIO_Port,DRIVE_EN_Pin)!=state){     			// Hvis ny tilstand er ulik nåværende tilstand
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

