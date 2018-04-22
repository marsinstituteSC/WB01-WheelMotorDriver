/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "cmsis_os.h"
#include "CANSPI.h"
#include "math.h"
#include "stdlib.h"
#include "MCP2515.h"

/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId SettFartTaskHandle;
osThreadId AckermannTaskHandle;
osThreadId CANbehandlingTaskHandle;
osThreadId MotorFaultTaskHandle;
QueueHandle_t MeldingQueueHandle;
QueueHandle_t FartQueueHandle;
QueueHandle_t AckerQueueHandle;
SemaphoreHandle_t ISRSemaHandleCAN;
SemaphoreHandle_t ISRSemaHandleFault;
SemaphoreHandle_t AckerProtHandle;

int32_t fartkonst;

//osMessageQId FartQueueHandle;
//osMessageQId RadiusQueueHandle;
//osMessageQId MeldingQueueHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartMotorTask(void const * argument);
void StartMotorFaultTask(void const * argument);
void StartAckermannTask(void const * argument);
void StartCANTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {


  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityRealtime, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of SettFartTask */
  osThreadDef(SettFartTask, StartMotorTask, osPriorityAboveNormal, 0, 128);
  SettFartTaskHandle = osThreadCreate(osThread(SettFartTask), NULL);

  osThreadDef(MotorFaultTask, StartMotorFaultTask, osPriorityNormal, 0, 128);
   MotorFaultTaskHandle = osThreadCreate(osThread(MotorFaultTask), NULL);

  /* definition and creation of AckermannTask */
  osThreadDef(AckermannTask, StartAckermannTask, osPriorityAboveNormal, 0, 128);
  AckermannTaskHandle = osThreadCreate(osThread(AckermannTask), NULL);

  /* definition and creation of CANbehandling */
  osThreadDef(CANbehandlingTask, StartCANTask, osPriorityRealtime, 0, 128);
  CANbehandlingTaskHandle = osThreadCreate(osThread(CANbehandlingTask), NULL);



  /* USER CODE BEGIN RTOS_QUEUES */
  FartQueueHandle = xQueueCreate(16,sizeof(uint16_t));
  AckerQueueHandle = xQueueCreate(16,sizeof(uint32_t));
  ISRSemaHandleCAN = xSemaphoreCreateBinary();
  ISRSemaHandleFault = xSemaphoreCreateBinary();
  AckerProtHandle = xSemaphoreCreateMutex();

  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}


/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
//	  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 10, 0);
//	  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 1);
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	  MCP2515_WriteByte(MCP2515_CANINTF,0x00);
	  vTaskDelete(defaultTaskHandle);

  }
  /* USER CODE END StartDefaultTask */
}

/* StartTask02 function */
void StartMotorTask(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */

	uint16_t fart = 0;
  for(;;)
  {
	if(xQueueReceive(FartQueueHandle,&fart,osWaitForever)){
		PWM_Set_Frekvens(fart);
	}
  }
  /* USER CODE END StartTask02 */
}


void StartMotorFaultTask(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */

  for(;;)
  {
//	  if(xSemaphoreTake(ISRSemaHandleFault,osWaitForever)){
//		  PWM_Set_Frekvens(0);
//		  MOTOR_STATE(0);
//		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
//		  vTaskDelay(300);
//		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
//		  MOTOR_STATE(1);
////		  uCAN_MSG tempTxMessage;
////		  tempTxMessage.frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
////		  tempTxMessage.frame.id = 0x222;
////		  tempTxMessage.frame.dlc = 1;
////		  tempTxMessage.frame.data0 = 0x01;
////		  CANSPI_Transmit(&tempTxMessage);


//	  }
  }
  /* USER CODE END StartTask02 */
}

/* StartTask03 function */
void StartAckermannTask(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
	int32_t radius;
	uint32_t bredde = 500;
	uint32_t bredde2 = 15625;
	uint32_t lengde = 250000;
  /* Infinite loop */
  for(;;)

  {

	  if(xQueueReceive(AckerQueueHandle,&radius,osWaitForever)){

		  if(xSemaphoreTake(AckerProtHandle,osWaitForever)){
			  if(radius==0x80000000){
				  fartkonst = 1;
			  } else if (( radius==1)||(radius==-1)){
				  fartkonst = 1; //fiks
			  }else {
				  fartkonst = (sqrt((float)((radius*radius)-radius*bredde+bredde2+lengde))/radius)*1000;
			  }
			  if(fartkonst<=0){fartkonst = -fartkonst;}
			  xSemaphoreGive(AckerProtHandle);
		  }
	  }
  }
  /* USER CODE END StartTask03 */
}

/* StartTask04 function */

void StartCANTask(void const * argument)
{
	uCAN_MSG tempRxMessage;
	uint16_t fart;
	int32_t radius;
  for(;;)
  {

	  if(xSemaphoreTake(ISRSemaHandleCAN,osWaitForever)){

		  if(CANSPI_Receive(&tempRxMessage)){
			  switch (tempRxMessage.frame.id) {
				case 0x100:
					if (((tempRxMessage.frame.data0<<8)+tempRxMessage.frame.data1)==0x8000){
						fart = 0xFFFF;
					} else {
						fart = ((tempRxMessage.frame.data0&0x80)<<8);
						fart += (abs((int16_t)((tempRxMessage.frame.data0<<8)+tempRxMessage.frame.data1)));
					}

					  xQueueSend(FartQueueHandle,&fart,0);
					  radius = (tempRxMessage.frame.data2<<24)+(tempRxMessage.frame.data3<<16)+(tempRxMessage.frame.data4<<8)+tempRxMessage.frame.data5;
				  	  xQueueSend(AckerQueueHandle,&radius,0);
				default:
					break;
			}

			}

	  }
  }
  /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Application */

//
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
