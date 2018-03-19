#ifndef OPPSETT_H_
#define OPPSETT_H_

#include "stm32f4xx_hal.h"

#define canModulMB2
#define adapterModulMB1


#define mask0 0x000
#define mask1 0x000

#define filter0 0x000
#define filter1 0x000


#define filter2 0x000
#define filter3 0x000
#define filter4 0x000
#define filter5 0x000


#ifdef canModulMB1

#define CAN_CS_Pin GPIO_PIN_8
#define CAN_CS_GPIO_Port GPIOE
#define CAN_INT_Pin GPIO_PIN_10
#define CAN_INT_GPIO_Port GPIOE
#define CAN_INT_EXTI_IRQn EXTI15_10_IRQn

#endif

#ifdef canModulMB2

#define CAN_CS_Pin GPIO_PIN_11
#define CAN_CS_GPIO_Port GPIOE
#define CAN_INT_Pin GPIO_PIN_14
#define CAN_INT_GPIO_Port GPIOE
#define CAN_INT_EXTI_IRQn EXTI15_10_IRQn

#endif



#ifdef adapterModulMB1

#define DRIVE_EN_Pin GPIO_PIN_2
#define DRIVE_EN_GPIO_Port GPIOA
#define DRIVE_DIR_Pin GPIO_PIN_10
#define DRIVE_DIR_GPIO_Port GPIOE
#define DRIVE_PWM_Pin GPIO_PIN_9
#define DRIVE_PWM_GPIO_Port GPIOE

#endif

#ifdef adapterModulMB2

#define DRIVE_EN_Pin GPIO_PIN_3
#define DRIVE_EN_GPIO_Port GPIOA
#define DRIVE_DIR_Pin GPIO_PIN_14
#define DRIVE_DIR_GPIO_Port GPIOE
#define DRIVE_PWM_Pin GPIO_PIN_12
#define DRIVE_PWM_GPIO_Port GPIOD

#endif


#endif /* OPPSETT_H_ */
