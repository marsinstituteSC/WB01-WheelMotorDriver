/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

// With some modification these files/functions can be used to control a motordriver
//#include "motorDriver.h"			// Functions for driving motor with PWM-signal.
//#include "TIM_oppsett.c" 			// Setup of timer for PWM-signal.


/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId Task1Handle;
osThreadId Task2Handle;
osThreadId CANReceiveTaskHandle;
SemaphoreHandle_t ISRCANSemaHandle;

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartTask1(void const * argument);
void StartTask2(void const * argument);
void StartCANReceiveTask(void const * argument);

void MX_FREERTOS_Init(void);

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {


  /* Create the threads */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityRealtime, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of AliveCounterTask */
  osThreadDef(Task1, StartTask1, osPriorityRealtime, 0, 128);
  Task1Handle = osThreadCreate(osThread(Task1), NULL);

  /* definition and creation of MotorControlTask */
  osThreadDef(Task2, StartTask2, osPriorityAboveNormal, 0, 128);
  Task2Handle = osThreadCreate(osThread(Task2), NULL);

  /* definition and creation of CANReceiveTask */
  osThreadDef(CANReceiveTask, StartCANReceiveTask, osPriorityRealtime, 0, 128);
  CANReceiveTaskHandle = osThreadCreate(osThread(CANReceiveTask), NULL);



  /* Creation of queues */


  /* Creation of Mutexes and Semaphores */
  ISRCANSemaHandle = xSemaphoreCreateBinary();

}


/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

	bool firstrun = true;

  for(;;)

  {	  if(firstrun){ // Run on startup

	  // Enable interrupt on CAN_INT-pin and motorfault-pin
	  // Interrupt before the kernel has started stops the system,
	  // therefore we set enable it in a task
	  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 1);
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	  // Clear interruption flag in MCP2515 in case message was received before kernel start
	  MCP2515_WriteByte(MCP2515_CANINTF,0x00);
	  // Deletes this task as it is just used for startup.
	  vTaskDelete(defaultTaskHandle);
  	  }
  }
}

/* StartAliveCounterTask function */
void StartTask1(void const * argument)
{
	/* Infinite loop */
  for(;;)
  {

  }
}

/* StartMotorControlTask function */
void StartTask2(void const * argument)
{


  for(;;)
  {


  }
}


/* StartCANReceiveTask function */
void StartCANReceiveTask(void const * argument)
{
	uCAN_MSG tempRxMessage;

  for(;;)
  {

	  if(xSemaphoreTake(ISRCANSemaHandle,osWaitForever)){		// Wait for interrupt signal from CAN-module

		  if(CANSPI_Receive(&tempRxMessage)){					// Receive the message from CAN-module, and switch-case on the ID
			  switch (tempRxMessage.frame.id) {
			  	  case 0x000:


			  	  case 0x001:



			  	  default:
					break;
			}

		}
	  }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
