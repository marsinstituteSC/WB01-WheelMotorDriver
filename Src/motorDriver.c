#include "motorDriver.h"
#include "main.h"

#define MOTOR_FRAM()   HAL_GPIO_WritePin(DRIVE_DIR_GPIO_Port, DRIVE_DIR_Pin, GPIO_PIN_SET)
#define MOTOR_BAK()   HAL_GPIO_WritePin(DRIVE_DIR_GPIO_Port, DRIVE_DIR_Pin, GPIO_PIN_RESET)
#define MOTOR_ENABLE()   {	HAL_GPIO_WritePin(DRIVE_EN_GPIO_Port, DRIVE_EN_Pin, GPIO_PIN_SET); \
							HAL_Delay(10000); \
							HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);\
							HAL_Delay(5000);}
#define MOTOR_DISABLE()   {	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1); \
							HAL_Delay(3000);\
							HAL_GPIO_WritePin(DRIVE_EN_GPIO_Port, DRIVE_EN_Pin, GPIO_PIN_RESET); \
							HAL_Delay(10000);}
