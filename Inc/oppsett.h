#ifndef OPPSETT_H_
#define OPPSETT_H_

#include "stm32f4xx_hal.h"
#include "motorDriver.h"
#include "CAN_messages.h"

// Oppsett moduler og CAN-mask/filter
//----------------------------------------------------//

#define canModulMB1
#define adapterModulMB2
#define MOTORVF								// Velg motor: "MOTORxx", xx = VF / HF / VM / HM / VB / HB
											// I fartsretning: V/H = venstre/høyre, F/M/B = framme, midt, bak

//----- CAN mask & filter oppsett -----//
#define mask0 MASK_ONES
#define filter0 GLOB_DRIVE
#define filter1 WDRW_FF_FAULT

#define mask1 MASK_ONES
#define filter2 WDRW_FF_STAT
#define filter3 FLTR_NULL
#define filter4 FLTR_NULL
#define filter5 FLTR_NULL


//----------------------------------------------------//


// Implement differences in Ackermann-calculations and WDRW_FF_STAT message bits.
#ifdef MOTORVF
		#define EN_MOTOR 0x01
#endif
//#ifdef MOTORHF
//		#define EN_MOTOR 0x02
//#endif
//#ifdef MOTORVM
//		#define EN_MOTOR 0x04
//#endif
//#ifdef MOTORHM
//		#define EN_MOTOR 0x08
//#endif
//#ifdef MOTORVB
//		#define EN_MOTOR 0x10
//#endif
//#ifdef MOTORHB
//		#define EN_MOTOR 0x20
//#endif



#ifdef canModulMB1

// CAN-module GPIO-pin definitions for Mikrobus socket 1
#define CAN_CS_Pin GPIO_PIN_8
#define CAN_CS_GPIO_Port GPIOE
#define CAN_INT_Pin GPIO_PIN_10
#define CAN_INT_GPIO_Port GPIOE
#define CAN_INT_EXTI_IRQn EXTI15_10_IRQn
#define CAN_SPI SPI3

#endif

#ifdef canModulMB2

// CAN-module GPIO-pin definitions for Mikrobus socket 2
#define CAN_CS_Pin GPIO_PIN_11
#define CAN_CS_GPIO_Port GPIOE
#define CAN_INT_Pin GPIO_PIN_14
#define CAN_INT_GPIO_Port GPIOE
#define CAN_INT_EXTI_IRQn EXTI15_10_IRQn
#define CAN_SPI SPI2

#endif



#ifdef adapterModulMB1

// Adapter-module/motordriver GPIO-pin definitions for Mikrobus socket 1
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

// Adapter-module/motordriver GPIO-pin definitions for Mikrobus socket 1
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
