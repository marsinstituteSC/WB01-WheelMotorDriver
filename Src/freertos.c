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


/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId AliveCounterTaskHandle;
osThreadId MotorControlTaskHandle;
osThreadId AckermannTaskHandle;
osThreadId CANReceiveTaskHandle;
osThreadId MotorFaultTaskHandle;
QueueHandle_t VelocityQueueHandle;
QueueHandle_t AckerQueueHandle;
SemaphoreHandle_t ISRCANSemaHandle;
SemaphoreHandle_t ISRFaultSemaHandle;
SemaphoreHandle_t AckerProtHandle;
SemaphoreHandle_t globAliveCounterHandle;


bool isTaskSuspendedInAC;
bool isTaskSuspendedInMF;
bool isTaskSuspendedInCAN;
int32_t AckermannFactor;
uint16_t farttest;
uint16_t farttest2;
uint16_t test1;
uint16_t test2;
uint16_t test3;



/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartAliveCounterTask(void const * argument);
void StartMotorControlTask(void const * argument);
void StartMotorFaultTask(void const * argument);
void StartAckermannTask(void const * argument);
void StartCANReceiveTask(void const * argument);

void MX_FREERTOS_Init(void);

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {


  /* Create the threads */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityRealtime, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of AliveCounterTask */
  osThreadDef(AliveCounterTask, StartAliveCounterTask, osPriorityRealtime, 0, 128);
  AliveCounterTaskHandle = osThreadCreate(osThread(AliveCounterTask), NULL);

  /* definition and creation of MotorControlTask */
  osThreadDef(MotorControlTask, StartMotorControlTask, osPriorityAboveNormal, 0, 128);
  MotorControlTaskHandle = osThreadCreate(osThread(MotorControlTask), NULL);

  /* definition and creation of MotorFaultTask */
  osThreadDef(MotorFaultTask, StartMotorFaultTask, osPriorityNormal, 0, 128);
   MotorFaultTaskHandle = osThreadCreate(osThread(MotorFaultTask), NULL);

  /* definition and creation of AckermannTask */
  osThreadDef(AckermannTask, StartAckermannTask, osPriorityNormal, 0, 128);
  AckermannTaskHandle = osThreadCreate(osThread(AckermannTask), NULL);

  /* definition and creation of CANReceiveTask */
  osThreadDef(CANReceiveTask, StartCANReceiveTask, osPriorityRealtime, 0, 128);
  CANReceiveTaskHandle = osThreadCreate(osThread(CANReceiveTask), NULL);



  /* Creation of queues */
  VelocityQueueHandle = xQueueCreate(1,sizeof(uint16_t));
  AckerQueueHandle = xQueueCreate(1,sizeof(uint32_t));

  /* Creation of Mutexes and Semaphores */
  ISRCANSemaHandle = xSemaphoreCreateBinary();
  ISRFaultSemaHandle = xSemaphoreCreateBinary();
  globAliveCounterHandle = xSemaphoreCreateBinary();
  AckerProtHandle = xSemaphoreCreateMutex();

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
	  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 10, 0);
	  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 1);
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	  isTaskSuspendedInAC = false;

	  // Clear interruption flag in MCP2515 in case message was received before kernel start
	  MCP2515_WriteByte(MCP2515_CANINTF,0x00);
//	  vTaskDelete(defaultTaskHandle);
  }
  		  // Waits 2000ms for alive-counter signal from motherboard, suspends MotorControlTask if one is not received
  	  	  // Commented out because it is not implemented in the motherboard

//  	  if(xSemaphoreTake(globAliveCounterHandle,2000)){
//  		  if(isTaskSuspendedInAC){
//  			  if(!(isTaskSuspendedInCAN||isTaskSuspendedInMF)){
//  				vTaskResume(MotorControlTaskHandle);
//  			  }
//  			  isTaskSuspendedInAC = false;
//  		  }
//  	  }else{
//  		  if(isTaskSuspendedInAC==false){
//  			vTaskSuspend(MotorControlTaskHandle);
//  			isTaskSuspendedInAC = true;
//  		  }
//			MOTOR_PWM_SET(0,1000);
//  	  }
  }
}

/* StartAliveCounterTask function */
void StartAliveCounterTask(void const * argument)
{
	// Define Alive-counter CAN-message
	uCAN_MSG aliveTxMessage;
	aliveTxMessage.frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
	aliveTxMessage.frame.id = WDRW_FL_STAT;
	aliveTxMessage.frame.dlc = 2;
	aliveTxMessage.frame.data0 = 0x00;
	aliveTxMessage.frame.data1 = 0x00;
  for(;;)
  {
	  // Increment the counter in data1, set to 0 when overflow occurs
	  aliveTxMessage.frame.data1++;
	  if(aliveTxMessage.frame.data1>=256){
		  aliveTxMessage.frame.data1 = 0;
	  }

	  // If task is suspended by motorfaulttask, set bit in data0 to signal this to motherboard
	  if(isTaskSuspendedInMF){
		  aliveTxMessage.frame.data0 = 0x01;
	  } else {
		  aliveTxMessage.frame.data0 = 0x00;
	  }
	  CANSPI_Transmit(&aliveTxMessage);
	  vTaskDelay(1000); // Delay 1000ms
  }
}

/* StartMotorControlTask function */
void StartMotorControlTask(void const * argument)
{
	uint16_t AckermannFactor2 = 1000;
	uint16_t tempvelocity = 0;

	/* Infinite loop */
  for(;;)
  {

	if(xQueueReceive(VelocityQueueHandle,&tempvelocity,osWaitForever)){		// Wait for CANReceiveTask to put data on queue
		test1 = tempvelocity;
		if(xSemaphoreTake(AckerProtHandle,10)){						// Use mutex to protect data
			AckermannFactor2 = (uint16_t)AckermannFactor;
			xSemaphoreGive(AckerProtHandle);
		} else {
			AckermannFactor2 = 1000;
		}
		MOTOR_PWM_SET(tempvelocity,AckermannFactor2); 						// Send velocity and Ackermannfactor to method
	}
  }
}

/* StartMotorFaultTask function */
void StartMotorFaultTask(void const * argument)
{
  /* Infinite loop */
  for(;;)
  {

	  if(xSemaphoreTake(ISRFaultSemaHandle,osWaitForever)){ 			// Wait for interrupt signal

//		  HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_15);

		  vTaskSuspend(MotorControlTaskHandle);							// Suspend MotorControlTask
		  isTaskSuspendedInMF = true;									// Set indicator that the task is suspended

		  MOTOR_PWM_SET(0,1000);										// Shut off PWM-signal

		  uCAN_MSG faultTxMessage;
		  faultTxMessage.frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
		  faultTxMessage.frame.id = WDRW_FF_FAULT;
		  faultTxMessage.frame.dlc = 1;
		  faultTxMessage.frame.data0 = 0x03;
		  CANSPI_Transmit(&faultTxMessage);								// Transmit message to other nodes and PDB to shut off power

		  vTaskDelay(300);

		  faultTxMessage.frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
		  faultTxMessage.frame.id = WDRW_FF_FAULT;
		  faultTxMessage.frame.dlc = 1;
		  faultTxMessage.frame.data0 = 0x00;
		  CANSPI_Transmit(&faultTxMessage);								// Transmit message to other nodes and PDB to turn on power

		  vTaskDelay(100);


		  if(!(isTaskSuspendedInAC||isTaskSuspendedInCAN)){				// If MotorControlTask is not suspended by other tasks, resume it
			  vTaskResume(SettFartTaskHandle);
		  }
		  isTaskSuspendedInMF = false;
	  }
  }
}

/* StartAckermannTask function */
void StartAckermannTask(void const * argument)
{
	int32_t radius;
	uint32_t bredde = 500;
	uint32_t bredde2 = 15625;
	uint32_t lengde = 250000;

  /* Infinite loop */
  for(;;)

  {

	  if(xQueueReceive(AckerQueueHandle,&radius,osWaitForever)){

		  if(xSemaphoreTake(AckerProtHandle,osWaitForever)){		// Get the mutex for protection of data
			  if(radius==0x80000000){
				  AckermannFactor = 1000;
			  } else if (( radius==1)||(radius==-1)||(radius==0)){
				  AckermannFactor = 1000;							// Needs fixing
			  }else {
				  // Calculate ackermannfactor from radius
				  AckermannFactor = (sqrt((float)((radius*radius)-radius*bredde+bredde2+lengde))/radius)*1000;
			  }
			  if(AckermannFactor<=0){AckermannFactor = -AckermannFactor;} // Absolute value
			  xSemaphoreGive(AckerProtHandle);
		  }
	  }
  }
}

/* StartCANReceiveTask function */
void StartCANReceiveTask(void const * argument)
{
	uCAN_MSG tempRxMessage;
	uint16_t velocity;
	int32_t radius;

  for(;;)
  {

	  if(xSemaphoreTake(ISRCANSemaHandle,osWaitForever)){		// Wait for interrupt signal from CAN-module

		  if(CANSPI_Receive(&tempRxMessage)){					// Receive the message from CAN-module, and switch-case on the ID
			  switch (tempRxMessage.frame.id) {

				case GLOB_DRIVE:	// Extract velocity and radius from message, send to queues

					// The velocity received is in INT16, the if-else below converts it to MSB=direction and the remaining 15 = velocity
					if (((tempRxMessage.frame.data0<<8)+tempRxMessage.frame.data1)==0x8000){
						velocity = 0xFFFF;
					} else {
						velocity = ((tempRxMessage.frame.data0&0x80)<<8);
						velocity += (abs((int16_t)((tempRxMessage.frame.data0<<8)+tempRxMessage.frame.data1)));
					}

					xQueueOverwrite(VelocityQueueHandle,&velocity);

					radius = (tempRxMessage.frame.data2<<24)+(tempRxMessage.frame.data3<<16)+(tempRxMessage.frame.data4<<8)+tempRxMessage.frame.data5;
					xQueueOverwrite(AckerQueueHandle,&radius);

					radius = 0;

				case WROT_FL_ANGLE: // Implement extraction of angle from message, and send on queue


				case WDRW_FF_STAT: // Use data0 to enable/disable the ENA-pin to driver
					if(tempRxMessage.frame.data0&EN_MOTOR){
						MOTOR_STATE(1);
					}else if(!(tempRxMessage.frame.data0&EN_MOTOR)){
						MOTOR_STATE(0);
					}

				case WDRW_FF_FAULT: // Suspend Motorcontroltask and shut off PWM
					if(tempRxMessage.frame.data0&0x01){
						vTaskSuspend(MotorControlTaskHandle);
						isTaskSuspendedInCAN = true;
						MOTOR_PWM_SET(0,1000);
					} else {
						// Resume task if not suspended by other tasks
						if(!(isTaskSuspendedInAC||isTaskSuspendedInMF)){
							vTaskResume(MotorControlTaskHandle);
						}
						isTaskSuspendedInCAN = false;
					}

				case 0x105: // Send signal to defaultTask that motherboard alive counter has updated
					xSemaphoreGive(globAliveCounterHandle);

				default:
					break;
			}

			}
	  }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
