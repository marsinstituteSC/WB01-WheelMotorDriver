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
extern int32_t AckermannFactor;
extern SemaphoreHandle_t AckerProtHandle;
extern uint16_t farttest2;
extern uint16_t test3;
extern uint16_t test2;
bool retning;
//extern uint16_t fart;



void MOTOR_PWM_SET(uint16_t tempvelocity, uint16_t AckerFactor){


//	if(HAL_GPIO_ReadPin(DRIVE_EN_GPIO_Port,DRIVE_EN_Pin)){


		bool direction = (tempvelocity>>15) & 0x01;			// Get the direction and send it to MOTOR_DIR_TOGGLE
		tempvelocity &= 0x7FFF;								// Remove direction bit from velocity
		MOTOR_DIR_TOGGLE(direction);

		if(tempvelocity >= 1){

			tempvelocity = 10500000000 / (tempvelocity*2584);	// Convert from "frequency" to "period", and scale for max 0.5m/s speed
			test3 = tempvelocity;
			test2 = AckerFactor;
			if (tempvelocity <=124){tempvelocity = 124;}		// Check if over max-values, if yes, set to max values
			if (tempvelocity >= 65535){tempvelocity = 65535;}
			tempvelocity = tempvelocity*1000;
			tempvelocity = tempvelocity/AckerFactor;			// Calculates new velocity from the Ackermann steering geometry

			PWM_TIMER->ARR = tempvelocity;						// Put in Timer reload and capture compare registers
			PWM_TIMER->CCR1 = (tempvelocity/2);
			PWM_TIMER->CR1 = 0x81;								// Enable Auto-reload and PWM-output
		}else{
			PWM_TIMER->ARR = 10;			// Timer reload-value
			PWM_TIMER->CCR1 = 10;			// Capture compare value, sets the duty-cycle
			PWM_TIMER->CR1 = 0x80;			// Set duty-cycle to 100% and disable PWM-output
		}
	}
//}

void MOTOR_DIR_TOGGLE(uint16_t dir){
	if(HAL_GPIO_ReadPin(DRIVE_DIR_GPIO_Port,DRIVE_DIR_Pin)!=dir){	// Check current state of DIR-pin against new state, if !=, enter
		HAL_TIM_PWM_Stop(&htim,TIM_CHANNEL_1);						// Stop output, wait 5us, toggle DIR pin, wait 5us, start output
		MOTOR_DELAY();
		HAL_GPIO_TogglePin(DRIVE_DIR_GPIO_Port,DRIVE_DIR_Pin);
		MOTOR_DELAY();
		HAL_TIM_PWM_Start(&htim,TIM_CHANNEL_1);
	}
}

void MOTOR_STATE(uint8_t state){
	if(HAL_GPIO_ReadPin(DRIVE_EN_GPIO_Port,DRIVE_EN_Pin)!=state){   // Check current state of ENA-pin against new state, if !=, enter
		if(state){				// if new state is on

			HAL_GPIO_WritePin(DRIVE_EN_GPIO_Port,DRIVE_EN_Pin,GPIO_PIN_SET);	// Set ENA pin, delay, start output
			MOTOR_DELAY();
			HAL_TIM_PWM_Start(&htim,TIM_CHANNEL_1);

		} else if(!state){		// if new state is off

			HAL_TIM_PWM_Stop(&htim,TIM_CHANNEL_1);								// Stop output, duty cycle 100%, delay, reset ENA pin
			PWM_TIMER->ARR = 10;
			PWM_TIMER->CCR1 = 10;
			MOTOR_DELAY();
			HAL_GPIO_WritePin(DRIVE_EN_GPIO_Port,DRIVE_EN_Pin,GPIO_PIN_RESET);

		}
	}
}
void MOTOR_DELAY(void){
	for(int i=0;i<=300;i++){ // simple solution for 5us delay
		asm("");			 // Emtpy assembly method, prevents compiler from optimizing away the for-loop
	}
}

