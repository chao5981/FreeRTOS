/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-20 11:11:31
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-08-23 09:53:25
 */
#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "BSP.h"
#include "event_groups.h"


/* ***************任务句柄******************** */
TaskHandle_t App_Task_Create_Handle;
TaskHandle_t Key_Task_Handle;
TaskHandle_t UASRT_Task_Handle;


/* ******************内核句柄********************* */
EventGroupHandle_t Event_Handle;


/* ********************宏定义********************** */
#define Key1_Event    (0x01<<0)
#define Key2_Event    (0x01<<1)


/* **********************任务函数*********************** */
static void App_Task_Create(void);
static void Key_Task(void);
static void USART_Task(void);

int main(void)
{
	BaseType_t xReturn=pdFAIL;
	BSP_Init();
	xReturn=xTaskCreate((TaskFunction_t)App_Task_Create,"App_Task",512,NULL,1,&App_Task_Create_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n app task is created \r\n");
		vTaskStartScheduler();
	}
	else
	{
		printf("\r\n app task created error \r\n");
		while (1);
	}
}

static void App_Task_Create(void)
{
	BaseType_t xReturn=pdFAIL;
	taskENTER_CRITICAL();
	Event_Handle=xEventGroupCreate();
	if(Event_Handle!=NULL)
	{
		printf("\r\n event is created \r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t)Key_Task,"key_task",512,NULL,3,&Key_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n key_Task is created \r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t)USART_Task,"USART_task",256,NULL,2,&UASRT_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n uasrt_Task is created \r\n");
	}
	vTaskDelete(App_Task_Create_Handle);
	taskEXIT_CRITICAL();
}


static void Key_Task(void)
{
	while(1)
	{
		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
		{
			xEventGroupSetBits(Event_Handle,Key1_Event);
			printf("\r\n key1 is put \r\n");
		}
		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN)==KEY_ON)
		{
			xEventGroupSetBits(Event_Handle,Key2_Event);
			printf("\r\n key2 is put \r\n");
		}
		vTaskDelay(200);
	}
}

static void USART_Task(void)
{
	EventBits_t r_event;
	while(1)
	{
		r_event=xEventGroupWaitBits(Event_Handle,Key1_Event|Key2_Event,pdTRUE,pdTRUE,portMAX_DELAY);
		if(r_event&(Key1_Event|Key2_Event)==(Key1_Event|Key2_Event))
		{
			printf("\r\n key1 and key2 are put \r\n");
		}
	}
}
