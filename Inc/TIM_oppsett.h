
#ifndef TIM_OPPSETT_H_
#define TIM_OPPSETT_H_
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"


extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;

extern void _Error_Handler(char *, int);

void TIM_Init(void);
void MX_TIM1_Init(TIM_HandleTypeDef htim1);
void MX_TIM4_Init(TIM_HandleTypeDef htim4);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);



#ifdef __cplusplus
}
#endif


#endif /* TIM_OPPSETT_H_ */
