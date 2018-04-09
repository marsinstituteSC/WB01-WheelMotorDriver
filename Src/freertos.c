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

/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId SettFartTaskHandle;
osThreadId AckermannTaskHandle;
osThreadId CANbehandlingHandle;
QueueHandle_t MeldingQueueHandle;
QueueHandle_t FartQueueHandle;
QueueHandle_t AckerQueueHandle;
SemaphoreHandle_t ISRSemaHandle;
uint16_t teller2 = 0;
extern uint16_t tellertest;
uint32_t fartkonst;

//osMessageQId FartQueueHandle;
//osMessageQId RadiusQueueHandle;
//osMessageQId MeldingQueueHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);
void StartTask04(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of SettFartTask */
  osThreadDef(SettFartTask, StartTask02, osPriorityNormal, 0, 128);
  SettFartTaskHandle = osThreadCreate(osThread(SettFartTask), NULL);

  /* definition and creation of AckermannTask */
  osThreadDef(AckermannTask, StartTask03, osPriorityNormal, 0, 128);
  AckermannTaskHandle = osThreadCreate(osThread(AckermannTask), NULL);

  /* definition and creation of CANbehandling */
  osThreadDef(CANbehandling, StartTask04, osPriorityRealtime, 0, 128);
  CANbehandlingHandle = osThreadCreate(osThread(CANbehandling), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of FartQueue */
//  osMessageQDef(FartQueue, 10, uint16_t);
//  FartQueueHandle = osMessageCreate(osMessageQ(FartQueue), NULL);
//
//  /* definition and creation of RadiusQueue */
//  osMessageQDef(RadiusQueue, 10, uint32_t);
//  RadiusQueueHandle = osMessageCreate(osMessageQ(RadiusQueue), NULL);
//
//  /* definition and creation of MeldingQueue */
//  osMessageQDef(MeldingQueue, 16, uCAN_MSG);
//  MeldingQueueHandle = osMessageCreate(osMessageQ(MeldingQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  MeldingQueueHandle = xQueueCreate(16,sizeof(uCAN_MSG));
  FartQueueHandle = xQueueCreate(16,sizeof(uint16_t));
  AckerQueueHandle = xQueueCreate(16,sizeof(uint32_t));
  ISRSemaHandle = xSemaphoreCreateBinary();

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
	  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	  vTaskDelete(defaultTaskHandle);

  }
  /* USER CODE END StartDefaultTask */
}

/* StartTask02 function */
void StartTask02(void const * argument)
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

/* StartTask03 function */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
	uint32_t radius;
	uint32_t radius2;
	uint32_t bredde = 500;
	uint32_t bredde2 = 15625;
	uint32_t lengde = 250000;
  /* Infinite loop */
  for(;;)

  {
//	  if(xQueueReceive(AckerQueueHandle,&radius,osWaitForever)){
//		  fartkonst = (uint32_t)(((sqrt((double)((radius*radius)-radius*bredde+bredde2+lengde)))/radius)*1000) ;
//	  }
  }
  /* USER CODE END StartTask03 */
}

/* StartTask04 function */
void StartTask04(void const * argument)
{
	uCAN_MSG tempRxMessage;
	uCAN_MSG temptxmessage;
	uint16_t fart;
	uint32_t radius;
  for(;;)
  {
	  if(xSemaphoreTake(ISRSemaHandle,osWaitForever)){
		  if(CANSPI_Receive(&tempRxMessage)){
			  switch (tempRxMessage.frame.id) {
				case 0x100:
					  fart = (tempRxMessage.frame.data0<<8)+tempRxMessage.frame.data1;
	//			  			  radius = (tempRxMessage.frame.data2<<24)+(tempRxMessage.frame.data3<<16)+(tempRxMessage.frame.data4<<8)+tempRxMessage.frame.data5;
					  xQueueSend(FartQueueHandle,&fart,0);
	//			  			  xQueueSend(AckerQueueHandle,&radius,0);
				default:
					break;
			}
//			  			  fart = (tempRxMessage.frame.data0<<8)+tempRxMessage.frame.data1;
////			  			  radius = (tempRxMessage.frame.data2<<24)+(tempRxMessage.frame.data3<<16)+(tempRxMessage.frame.data4<<8)+tempRxMessage.frame.data5;
//			  			  xQueueSend(FartQueueHandle,&fart,0);
////			  			  xQueueSend(AckerQueueHandle,&radius,0);

//				  switch (tempRxMessage.frame.id) {
//					case 0x100:
//						fart = (tempRxMessage.frame.data0<<8)+tempRxMessage.frame.data1;
//						xQueueSend(FartQueueHandle,&fart,0);
//					case 0x410:
//						radius = (tempRxMessage.frame.data0<<8)+tempRxMessage.frame.data1;
//						xQueueSend(AckerQueueHandle,&radius,0);
//					case 0x400:
//						break;
//					default:
//						break;
//				}
			}
	  }
  }
  /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Application */

//
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
