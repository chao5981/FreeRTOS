/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-19 10:06:23
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-08-19 11:03:12
 */
#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "BSP.h"
#include "semphr.h"
#include "queue.h"

/* **************任务句柄************** */
TaskHandle_t App_TaskCreate_Handle;
TaskHandle_t High_Priority_Task_Handle;
TaskHandle_t Mid_Priority_Task_Handle;
TaskHandle_t Low_Priority_Task_Handle;


/* ***************内核句柄************** */
SemaphoreHandle_t MuxSem_Handle;



/* ***************任务函数******************* */
static void App_Task_Create(void);
static void High_Priority_Task(void);
static void Mid_Priority_Task(void);
static void Low_Priority_Task(void);




int main(void)
{
	BaseType_t xReturn=pdFAIL;
	BSP_Init();
	xReturn=xTaskCreate((TaskFunction_t)App_Task_Create,"App_Task_Create",512,NULL,1,&App_TaskCreate_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n app task is created \r\n");
		vTaskStartScheduler();
	}
	else
	{
		printf("\r\n error \r\n");
		while(1);
	}
}

static void App_Task_Create(void)
{
	BaseType_t xReturn=pdFAIL;
	taskENTER_CRITICAL();
	MuxSem_Handle=xSemaphoreCreateMutex();
	if(MuxSem_Handle!=NULL)
	{
		printf("\r\n muxsem is created \r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t)Low_Priority_Task,"Low_Priority_Task",128,NULL,2,&Low_Priority_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n low priority task is created \r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t)Mid_Priority_Task,"Mid_Priority_Task",128,NULL,3,&Mid_Priority_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n Mid priority task is created \r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t)High_Priority_Task,"High_Priority_Task",128,NULL,4,&High_Priority_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n high priority task is created \r\n");
	}
	vTaskDelete(App_TaskCreate_Handle);
	taskEXIT_CRITICAL();
}

static void Low_Priority_Task(void)
{
	BaseType_t xReturn=pdFAIL;
	while (1)
	{
		printf("\r\n low priority task is running \r\n");
		xReturn=xSemaphoreTake(MuxSem_Handle,portMAX_DELAY);
		if(xReturn==pdTRUE)
		{
			printf("\r\n low priority task get sem \r\n");
		}

		//这里为占用互斥量的代码，这里我们延时表示在此期间的阻塞态
		vTaskDelay(2000);
		printf("\r\n low priority task give sem \r\n");
		xReturn=xSemaphoreGive(MuxSem_Handle);
		vTaskDelay(1000);
	}
}


static void Mid_Priority_Task(void)
{
	while (1)
	{
		//这里看看在互斥量被低优先级占用时是否会出现优先级翻转的情况
		printf("\r\n mid priority task is running \r\n");
		vTaskDelay(1000);
	}
	
}

static void High_Priority_Task(void)
{
	BaseType_t xReturn=pdFAIL;
	while (1)
	{
		printf("\r\n high priority task is running \r\n");
		xReturn=xSemaphoreTake(MuxSem_Handle,portMAX_DELAY);
		if(xReturn==pdTRUE)
		{
			printf("\r\n high priority task get sem \r\n");
		}

		//这里为占用互斥量的代码，这里我们延时表示在此期间的阻塞态
		vTaskDelay(200);
		printf("\r\n high priority task give sem \r\n");
		xReturn=xSemaphoreGive(MuxSem_Handle);
		vTaskDelay(1000);
	}
	
}

