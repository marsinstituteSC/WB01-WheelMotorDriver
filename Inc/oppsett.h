#ifndef OPPSETT_H_
#define OPPSETT_H_

#include "stm32f4xx_hal.h"
#include "motorDriver.h"
#include "CAN_messages.h"

// Oppsett moduler og CAN-mask/filter
//----------------------------------------------------//

#define canModulMB1							// Choose Mikrobus socket 1 or 2 with "#define canModulMB1"/"#define canModulMB2"
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




#endif /* OPPSETT_H_ */
