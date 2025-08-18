/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-17 11:01:25
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-08-17 20:14:24
 */
#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "BSP.h"
#include "semphr.h"

/* **************任务句柄***************  */
TaskHandle_t App_Task_Create_Handle;
TaskHandle_t Key_Task_Handle;
TaskHandle_t USART_Task_Handle;


/* ***************内核对象句柄************** */
SemaphoreHandle_t BinarySem_Handle;


/* **************任务函数****************** */
static void App_Task_Create(void);
static void Key_Task(void);
static void USART_Task(void);



int main(void)
{
	BaseType_t xReturn=pdFAIL;
	
	BSP_Init();
	
	xReturn=xTaskCreate((TaskFunction_t) App_Task_Create,"App_Task_Create",512,NULL,1,&App_Task_Create_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n app_Task is created \r\n");
		vTaskStartScheduler();
	}
	else
	{
		while(1);
	}
}

static void App_Task_Create(void)
{
	BaseType_t xReturn=pdFAIL;
	taskENTER_CRITICAL();
	BinarySem_Handle=xSemaphoreCreateBinary();
	if(BinarySem_Handle!=NULL)
	{
		printf("\r\n binary_sem is created \r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t) Key_Task,"Key_Task",128,NULL,2,&Key_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n key_task is created \r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t) USART_Task,"USART_Task",256,NULL,3,&USART_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n usart_task is created \r\n");
	}
	vTaskDelete(App_Task_Create_Handle);
	taskEXIT_CRITICAL();
}

static void Key_Task(void)
{
	BaseType_t xReturn=pdFAIL;
	while (1)
	{
		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
		{
			xReturn=xSemaphoreGive(BinarySem_Handle);
			if(xReturn==pdPASS)
			{
				printf("\r\n 信号量已经释放 \r\n");
			}
		}
		vTaskDelay(200);
	}
	
}

static void USART_Task(void)
{
	BaseType_t xReturn=pdFAIL;
	while (1)
	{
		xReturn=xSemaphoreTake(BinarySem_Handle,portMAX_DELAY);
		if(xReturn==pdPASS)
		{
			printf("\r\n 信号量已捕捉 \r\n");
		}
	}
	
}
