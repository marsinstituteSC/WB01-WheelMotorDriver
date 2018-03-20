
#ifndef TIM_OPPSETT_H_
#define TIM_OPPSETT_H_
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"

#ifdef adapterModulMB2
extern TIM_HandleTypeDef htim4;
#define PWM_TIM &htim4
#define PWM_TIM_REG TIM4
#endif

#ifdef adapterModulMB1
extern TIM_HandleTypeDef htim1;
#define PWM_TIM &htim1
#define PWM_TIM_REG TIM1
#endif


extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;

extern void _Error_Handler(char *, int);

void MX_TIM1_Init(void);
void MX_TIM4_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);



#ifdef __cplusplus
}
#endif


#endif /* TIM_OPPSETT_H_ */
