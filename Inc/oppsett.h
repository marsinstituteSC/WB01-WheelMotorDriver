#ifndef OPPSETT_H_
#define OPPSETT_H_

#include "stm32f4xx_hal.h"
#include "motorDriver.h"
#include "CAN_messages.h"

// Setup modules and CAN-mask/filter
//----------------------------------------------------//

#define canModulMB1			// Should not be touched
#define adapterModulMB2		// Should not be touched
#define MOTOR_FL								// Choose motor: "MOTORxx", xx = FL / FR / ML / MR / BL / BR
											// In forward direction:F/M/B = front, middle, back, L/R = left/right

//----- CAN mask & filter oppsett -----//
#define mask0 MASK_ONES
#define filter0 GLOB_DRIVE
#define filter1 WDRW_FF_FAULT

#define mask1 MASK_ONES
#define filter2 WDRW_FF_STAT
//#define filter3 WROT_FL_ANGLE  // Set in wheel-specific settings below
#define filter4 FLTR_NULL
#define filter5 FLTR_NULL


//----------------------------------------------------//


// Implement differences in Ackermann-calculations and WDRW_FF_STAT message bits.
#ifdef MOTOR_FL
		#define EN_MOTOR 0x01
		#define ACK_LEN	540
		#define ACK_WIDTH 330
		#define ACK_WIDTH2
		#define filter3 WROT_FL_ANGL
#endif
#ifdef MOTOR_FR
		#define EN_MOTOR 0x02
		#define ACK_LEN
		#define ACK_WIDTH
		#define ACK_WIDTH2
		#define filter3 WROT_FR_ANGL
#endif
#ifdef MOTOR_ML
		#define EN_MOTOR 0x04
		#define ACK_LEN
		#define ACK_WIDTH
		#define ACK_WIDTH2
		#define filter3 WROT_ML_ANGL
#endif
#ifdef MOTOR_MR
		#define EN_MOTOR 0x08
		#define ACK_LEN
		#define ACK_WIDTH
		#define ACK_WIDTH2
		#define filter3 WROT_MR_ANGL
#endif
#ifdef MOTOR_BL
		#define EN_MOTOR 0x10
		#define ACK_LEN
		#define ACK_WIDTH
		#define ACK_WIDTH2
		#define filter3 WROT_BL_ANGL
#endif
#ifdef MOTOR_BR
		#define EN_MOTOR 0x20
		#define ACK_LEN
		#define ACK_WIDTH
		#define ACK_WIDTH2
		#define filter3 WROT_BR_ANGL
#endif



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
