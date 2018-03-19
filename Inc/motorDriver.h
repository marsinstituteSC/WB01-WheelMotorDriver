
#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include "stm32f4xx_hal.h"
#include "main.h"



#define MOTOR_FRAM()   	{	HAL_TIM_PWM_Stop(PWM_TIM,TIM_CHANNEL_1);								\
							HAL_Delay(1);															\
							HAL_GPIO_WritePin(DRIVE_DIR_GPIO_Port, DRIVE_DIR_Pin, GPIO_PIN_SET);	\
							HAL_Delay(1);															\
							HAL_TIM_PWM_Start(PWM_TIM,TIM_CHANNEL_1);								}

#define MOTOR_BAK()   	{	HAL_TIM_PWM_Stop(PWM_TIM,TIM_CHANNEL_1);									\
							HAL_Delay(1);															\
							HAL_GPIO_WritePin(DRIVE_DIR_GPIO_Port, DRIVE_DIR_Pin, GPIO_PIN_RESET);	\
							HAL_Delay(1);															\
							HAL_TIM_PWM_Start(PWM_TIM,TIM_CHANNEL_1);								}

#define MOTOR_ENABLE()  {	HAL_GPIO_WritePin(DRIVE_EN_GPIO_Port, DRIVE_EN_Pin, GPIO_PIN_SET); 		\
							HAL_Delay(1); 															\
							HAL_TIM_PWM_Start(PWM_TIM,TIM_CHANNEL_1);								}

#define MOTOR_DISABLE()	{	HAL_TIM_PWM_Stop(PWM_TIM,TIM_CHANNEL_1); 								\
							HAL_Delay(1);															\
							HAL_GPIO_WritePin(DRIVE_EN_GPIO_Port, DRIVE_EN_Pin, GPIO_PIN_RESET);	}


void PWM_Set_Frekvens(uint16_t fart);


#endif /* MOTORDRIVER_H_ */
