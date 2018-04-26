#ifndef OPPSETT_H_
#define OPPSETT_H_

// Oppsett moduler og CAN-mask/filter
//----------------------------------------------------//

#include "stm32f4xx_hal.h"
#include "motorDriver.h"
#include "CAN_messages.h"

#define canModulMB1
#define adapterModulMB2
#define motorVF

//----- CAN mask & filter oppsett -----//
#define mask0 MASK_ONES
#define filter0 GLOB_DRIVE
#define filter1 FLTR_NULL

#define mask1 MASK_ONES
#define filter2 0x255
#define filter3 FLTR_NULL
#define filter4 FLTR_NULL
#define filter5 FLTR_NULL



//----------------------------------------------------//









#ifdef canModulMB1

#define CAN_CS_Pin GPIO_PIN_8
#define CAN_CS_GPIO_Port GPIOE
#define CAN_INT_Pin GPIO_PIN_10
#define CAN_INT_GPIO_Port GPIOE
#define CAN_INT_EXTI_IRQn EXTI15_10_IRQn
#define CAN_SPI SPI3

#endif

#ifdef canModulMB2

#define CAN_CS_Pin GPIO_PIN_11
#define CAN_CS_GPIO_Port GPIOE
#define CAN_INT_Pin GPIO_PIN_14
#define CAN_INT_GPIO_Port GPIOE
#define CAN_INT_EXTI_IRQn EXTI15_10_IRQn
#define CAN_SPI SPI2

#endif



#ifdef adapterModulMB1

#define DRIVE_EN_Pin GPIO_PIN_2
#define DRIVE_EN_GPIO_Port GPIOA
#define DRIVE_DIR_Pin GPIO_PIN_10
#define DRIVE_DIR_GPIO_Port GPIOE
#define DRIVE_PWM_Pin GPIO_PIN_9
#define DRIVE_PWM_GPIO_Port GPIOE
#define DRIVE_FAULT_GPIO_Port GPIOD
#define DRIVE_FAULT_Pin GPIO_PIN_6
#define PWM_TIMER TIM1

#endif

#ifdef adapterModulMB2

#define DRIVE_EN_Pin GPIO_PIN_11
#define DRIVE_EN_GPIO_Port GPIOE
#define DRIVE_DIR_Pin GPIO_PIN_14
#define DRIVE_DIR_GPIO_Port GPIOE
#define DRIVE_PWM_Pin GPIO_PIN_12
#define DRIVE_PWM_GPIO_Port GPIOD
#define DRIVE_FAULT_GPIO_Port GPIOD
#define DRIVE_FAULT_Pin GPIO_PIN_8
#define PWM_TIMER TIM4

#endif


#endif /* OPPSETT_H_ */
