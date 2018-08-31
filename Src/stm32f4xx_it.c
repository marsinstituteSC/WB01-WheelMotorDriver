/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************

  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "CANSPI.h"
#include "FreeRTOS.h"
#include "queue.h"


/* External variables --------------------------------------------------------*/

extern TIM_HandleTypeDef htim2;
extern SemaphoreHandle_t ISRCANSemaHandle;
extern SemaphoreHandle_t ISRFaultSemaHandle;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  osSystickHandler();
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles TIM2 global interrupt.
*/
void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim2);
}

/**
* @brief This function handles EXTI line[15:10] interrupts.
*/
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_15);

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR(ISRCANSemaHandle,&xHigherPriorityTaskWoken);

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);

  	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_15);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR(ISRFaultSemaHandle,&xHigherPriorityTaskWoken);

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);

  	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
