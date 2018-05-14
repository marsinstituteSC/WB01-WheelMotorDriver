///**
//  ******************************************************************************
//  * File Name          : TIM_oppsett.c
//  * Description        : This file provides code for the configuration
//  *                      of timers
//  ******************************************************************************
//*/
///* Includes ------------------------------------------------------------------*/
//#include "TIM_oppsett.h"
//
//
//TIM_HandleTypeDef htim;
//
///* TIM1 init function */
//void TIMER_Init(void)
//{
//  TIM_MasterConfigTypeDef sMasterConfig;
//  TIM_OC_InitTypeDef sConfigOC;
//  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
//
//  htim.Instance = PWM_TIMER;
//  htim.Init.Prescaler = 7;
//  htim.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim.Init.Period = 0;
//  htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//
//  if (htim.Instance == TIM1){
//	htim.Init.RepetitionCounter = 0;
//  }
//
//  if (HAL_TIM_PWM_Init(&htim) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }
//
//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  if (HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }
//
//  sConfigOC.OCMode = TIM_OCMODE_PWM1;
//  sConfigOC.Pulse = 0;
//  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//
//  if (htim.Instance == TIM1)
//  {
//	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
//	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
//	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
//  }
//
//  if (HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }
//
//  if (htim.Instance == TIM1)
//  {
//	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
//	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
//	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
//	sBreakDeadTimeConfig.DeadTime = 0;
//	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
//	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
//	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
//	if (HAL_TIMEx_ConfigBreakDeadTime(&htim, &sBreakDeadTimeConfig) != HAL_OK)
//	{
//	  _Error_Handler(__FILE__, __LINE__);
//	}
//  }
//
//
//  HAL_TIM_MspPostInit(&htim);
//  HAL_TIM_Base_Start(&htim);
//  HAL_TIM_PWM_Start(&htim,TIM_CHANNEL_1);
//
//}
//
//void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
//{
//
//  if(tim_pwmHandle->Instance==TIM1)
//  {
//    /* TIM1 clock enable */
//    __HAL_RCC_TIM1_CLK_ENABLE();
//  }
//  else if(tim_pwmHandle->Instance==TIM4)
//  {
//    /* TIM4 clock enable */
//    __HAL_RCC_TIM4_CLK_ENABLE();
//  }
//}
//void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
//{
//
//  GPIO_InitTypeDef GPIO_InitStruct;
//  if(timHandle->Instance==TIM1)
//  {
//    /**TIM1 GPIO Configuration
//    PE9     ------> TIM1_CH1
//    */
//    GPIO_InitStruct.Pin = DRIVE_PWM_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
//    HAL_GPIO_Init(DRIVE_PWM_GPIO_Port, &GPIO_InitStruct);
//
//  }
//  else if(timHandle->Instance==TIM4)
//  {
//
//    /**TIM4 GPIO Configuration
//    PD12     ------> TIM4_CH1
//    */
//    GPIO_InitStruct.Pin = DRIVE_PWM_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
//    HAL_GPIO_Init(DRIVE_PWM_GPIO_Port, &GPIO_InitStruct);
//
//  }
//
//}
//
//void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
//{
//
//  if(tim_pwmHandle->Instance==TIM1)
//  {
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM1_CLK_DISABLE();
//  }
//  else if(tim_pwmHandle->Instance==TIM4)
//  {
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM4_CLK_DISABLE();
//  }
//}
//
///**
//  * @}
//  */
//
///**
//  * @}
//  */
//
///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
