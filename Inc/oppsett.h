#ifndef OPPSETT_H_
#define OPPSETT_H_

#include "stm32f4xx_hal.h"

#define canModul microbus1


#define mask0 0x000
#define mask1 0x000

#define filter0 0x000
#define filter1 0x000


#define filter2 0x000
#define filter3 0x000
#define filter4 0x000
#define filter5 0x000

#define adapterModul microbus2

#ifdef canModul

#if canModul == microbus1

SPI_HandleTypeDef        hspi3;
#define SPI_CAN                 &hspi3
#define hSPICAN                 hspi3
#define MBSPI SPI3

#define CAN_CS_Pin GPIO_PIN_8
#define CAN_CS_GPIO_Port GPIOE
#define CAN_INT_Pin GPIO_PIN_10
#define CAN_INT_GPIO_Port GPIOE
#define CAN_INT_EXTI_IRQn EXTI15_10_IRQn

#endif

#if canModul == microbus2

#endif

#endif

#ifdef adapterModul

#if adapterModul == microbus1
#endif

#if adapterModul == microbus2
#endif

#endif


#endif /* OPPSETT_H_ */
