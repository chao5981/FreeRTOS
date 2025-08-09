
#include "stm32f10x.h"   
#include "FreeRTOS.h"
#include "task.h"
#include "BSP.h"

/*********����������********* */
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED1_Task_Handle;
static TaskHandle_t LED2_task_Handle;
static TaskHandle_t USART_Task_Handle;
static TaskHandle_t Task_Monitor_Handle;

/*************���񴴽�����************** */
static void AppTaskCreate(void);
static void LED1_Task(void);
static void LED2_Task(void);
static void USART_Task(void);
static void TaskMonitor(void);



int main(void)
{
	BaseType_t xRetrun=pdPASS;
	BSP_Init();
	printf("\r\n����һ�������������ʵ��\r\n");
	xRetrun=xTaskCreate((TaskFunction_t)AppTaskCreate,"APP_Task",128,NULL,1,&AppTaskCreate_Handle);
	if(xRetrun==pdPASS)
	{
		vTaskStartScheduler();
	}
	else
	{
		printf("\r\n��������ʧ��\r\n");
		while (1)
		{
		}
		
	}
}

/**
 * @brief ���񴴽�����
 * @param ��
 * @retval  ��
 */
static void AppTaskCreate(void)
{
	BaseType_t xReturn=pdPASS;

	// �����ٽ��
	taskENTER_CRITICAL();

	xReturn=xTaskCreate((TaskFunction_t)LED1_Task,"LED1_Task",30,NULL,2,&LED1_Task_Handle);

	if(xReturn==pdPASS)
	{
		printf("\r\nLED1_Task successfully create\r\n");
	}

	xReturn=xTaskCreate((TaskFunction_t)LED2_Task,"LED2_Task",30,NULL,2,&LED2_task_Handle);
	if (xReturn==pdPASS)
	{
		printf("\r\nLED2_Task successfully create\r\n");
	}
	
	xReturn=xTaskCreate((TaskFunction_t)USART_Task,"USART_Task",60,NULL,2,&USART_Task_Handle);

	if(xReturn==pdPASS)
	{
		printf("\r\nUSART_Task successfully create\r\n");
	}

	xReturn=xTaskCreate((TaskFunction_t)TaskMonitor,"TaskMonitor",128,NULL,3,&Task_Monitor_Handle);
	
	if(xReturn==pdPASS)
	{
		printf("\r\nTask Monitor successfully create\r\n");
	}

	// ɾ��appTaskCreate����
	 vTaskDelete(AppTaskCreate_Handle); 

	// �˳��ٽ��
	taskEXIT_CRITICAL();
}


/**
 * @brief USART���ڴ�ӡ����
 * @param ��
 * @retval ��
 */
static void USART_Task(void)
{
	while (1)
	{
		printf("\r\nUASRT_Task successful used\r\n");
		vTaskDelay(600);
	}
	
}


/**
 * @brief LED1����˸����
 * @param  ��
 * @retval  ��
 */
static void LED1_Task(void)
{
	while(1)
	{
		LED1_ON;
		vTaskDelay(500);
		LED1_OFF;
		vTaskDelay(500);
	}
}


/**
 * @brief LED2��������˸����
 * @param ��
 * @retval ��
 */
static void LED2_Task(void)
{
	while(1)
	{
		LED2_ON;
		vTaskDelay(500);
		LED2_OFF;
		vTaskDelay(500);
	}
}


/**
 * @brief ����������ջ��С
 * @param ��
 * @retval ��
 */
static void TaskMonitor(void)
{
	UBaseType_t LED1_Mark;
	UBaseType_t LED2_Mark;
	UBaseType_t USART_Mark;
	while(1)
	{
		LED1_Mark=uxTaskGetStackHighWaterMark(LED1_Task_Handle);
		LED2_Mark=uxTaskGetStackHighWaterMark(LED2_task_Handle);
		USART_Mark=uxTaskGetStackHighWaterMark(USART_Task_Handle);

		printf("\r\nLED1 Task Stack: Used = %u bytes, Free = %u bytes\r\n", 
               (30 - LED1_Mark) * 4, LED1_Mark * 4);
    printf("\r\nLED2 Task Stack: Used = %u bytes, Free = %u bytes\r\n",
               (30 - LED2_Mark) * 4, LED2_Mark * 4);
		printf("\r\nUSART Task Stack: Used = %u byte,free = %u bytes\r\n",
				(60-USART_Mark)*4,USART_Mark*4);
		
		vTaskDelay(2000);
	}
	
}

