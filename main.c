/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-12 11:16:28
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-08-13 10:33:09
 */
#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "BSP.h"

/* ***********创建任务句柄*********** */
TaskHandle_t App_Task_Create_Handle;
TaskHandle_t KEY_Scan_Task_Handle;
TaskHandle_t USART_Print_Task_Handle;

//TaskHandle_t LED1_Task_Handle;
extern TaskHandle_t LED1_Task_Handle;

/* *************创建任务函数*************** */
static void App_Task_Create(void);
static void LED1_Task(void);
static void Key_Scan_Task(void);
static void USART_Print_Task(void);


/* **********全局变量*********** */
extern uint8_t LED1_Flag;

int main(void)
{
	BaseType_t xReturn=pdPASS;
	BSP_Init();
	xReturn=xTaskCreate((TaskFunction_t)App_Task_Create,"App_Task_create",256,NULL,1,&App_Task_Create_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\napp_task is created\r\n");
		vTaskStartScheduler();
	}
	else
	{
		while(1);
	}
	
}

static void App_Task_Create(void)
{
	BaseType_t xReturn =pdPASS;

	taskENTER_CRITICAL();
	
	xReturn=xTaskCreate((TaskFunction_t) LED1_Task,"LED1_Task",128,NULL,1,&LED1_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("LED1_Task is created\r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t) Key_Scan_Task,"Key_Scan_Task",256,NULL,3,&KEY_Scan_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("Key_Saan_Task is created\r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t) USART_Print_Task,"USART_Print_Task",512,NULL,2,&USART_Print_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("UASRT_Task is created\r\n");
	}

	vTaskDelete(App_Task_Create_Handle);

	taskEXIT_CRITICAL();
}


static void LED1_Task(void)
{
	while (1)
	{
			LEDG(ON);
			vTaskDelay(500);
			LEDG(OFF);
			vTaskDelay(500);
	}
	
}


//static void Key_Scan_Task(void)
//{
//	while (1)
//	{
//		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
//		{
//			LED1_Flag=~LED1_Flag;
//			vTaskSuspend(LED1_Task_Handle);
//		}
//		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN)==KEY_ON)
//		{
//			LED1_Flag=~LED1_Flag;
//			vTaskResume(LED1_Task_Handle);
//		}
//		vTaskDelay(20);
//	}
//	
//}



static void Key_Scan_Task(void)
{
	while (1)
	{
		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN)==KEY_ON)
		{
			LED1_Flag=~LED1_Flag;
			vTaskSuspend(LED1_Task_Handle);
		}
		vTaskDelay(20);
	}
	
}

static void USART_Print_Task(void)
{
	//保存上次的时间
	static TickType_t PreviousWakeTime;

	//设置延时时间，将时间转化为节拍数
	const TickType_t TimeIncrement=pdMS_TO_TICKS(1000);

	PreviousWakeTime=xTaskGetTickCount();

	while (1)
	{
		if(LED1_Flag==0)
		{
			printf("\r\nLED1_Task was running\r\n");
		}
		else
		{
			printf("\r\nLED1_Task was hung\r\n");
		}
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);
	}
}


