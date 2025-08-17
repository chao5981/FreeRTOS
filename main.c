/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-15 09:27:59
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-08-15 11:25:37
 */
#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "BSP.h"

/* *************创建任务句柄************** */
TaskHandle_t App_Task_Create_Handle;
TaskHandle_t USART_Printf_Task_Handle;
TaskHandle_t Key_Scan_Task_Handle;
TaskHandle_t LED_Task_Handle;

/* **************内核对象句柄*************** */
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */

QueueHandle_t Test_Queue_Handle=NULL;


/* ************队列相关的宏定义************** */
#define Test_Queue_Length    4
#define Test_Queue_Size 	 sizeof(uint8_t)

/* **************任务创建函数***************** */
static void App_Task_Create(void);
static void USART_Printf_Task(void);
static void Key_Scan_Task(void);
static void LED_Task(void);

int main(void)
{
	BaseType_t xReturn=pdPASS;
	BSP_Init();
	xReturn=xTaskCreate((TaskFunction_t)App_Task_Create,"App_Task_Create",512,NULL,1,&App_Task_Create_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n App_Task_Create is Successfully created\r\n");
		vTaskStartScheduler();
	}
	else
	{
		printf("\r\n !!!App_Task_Create isn`t used!!!!\r\n");
		while(1);
	}
}


/**
 * @brief 创建任务函数
 * @param 无
 * @retval 无
 */
static void App_Task_Create(void)
{
	BaseType_t xReturn=pdPASS;

	taskENTER_CRITICAL();
	xReturn=xTaskCreate((TaskFunction_t)USART_Printf_Task,"USART_Printf_Task",256,NULL,1,&USART_Printf_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n USART_Printf_Task is successfully creaeted\r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t)Key_Scan_Task,"Key_Scan_Task",128,NULL,3,&Key_Scan_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n Key_Scan_Task is successfully creaeted\r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t)LED_Task,"LED_Tsk",128,NULL,2,&LED_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n LED_Tsk is successfully creaeted\r\n");
	}
	Test_Queue_Handle=xQueueCreate((BaseType_t)Test_Queue_Length,(BaseType_t)Test_Queue_Size);
	if(Test_Queue_Handle!=NULL)
	{
		printf("\r\n Test_Queue is successfully created \r\n");
	}

	vTaskDelete(App_Task_Create_Handle);

	taskEXIT_CRITICAL();
}


/**
 * @brief 按键扫描任务函数
 * @param 无
 * @retval 无
 */
static void Key_Scan_Task(void)
{
	BaseType_t xReturn=pdTRUE;
	uint32_t send_data1 = 1;
  	uint32_t send_data2 = 2;
	while(1)
	{
		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
		{
			xReturn=xQueueSend(Test_Queue_Handle,&send_data1,0);
			if(xReturn==pdTRUE)
			{
				printf("\r\n send_data1 is sent \r\n");
			}
		}

		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN)==KEY_ON)
		{
			xReturn=xQueueSend(Test_Queue_Handle,&send_data2,0);
			if(xReturn==pdTRUE)
			{
				printf("\r\n send_data2 is sent \r\n");
			}
		}
		vTaskDelay(20);
	}
}



/**
 * @brief LED灯任务
 * @param 无
 * @retval 无
 */
static void LED_Task(void)
{
	BaseType_t xReturn=pdTRUE;
	uint32_t Queue_Massage=0;
	while (1)
	{
		xReturn=xQueuePeek(Test_Queue_Handle,&Queue_Massage,portMAX_DELAY);
		if(xReturn==pdTRUE)
		{
			if(Queue_Massage==1)
			{
				LED_RED;
			}
			else if(Queue_Massage==2)
			{
				LED_GREEN;
			}
			else
			{
				LED_BLUE;
			}
		}
		
		vTaskDelay(20);
	}
	
}


/**
 * @brief 串口打印数据函数
 * @param 无
 * @retval 无
 */
static void USART_Printf_Task(void)
{
	BaseType_t xReturn=pdTRUE;
	uint32_t Queue_Message=0;
	while(1)
	{
		xReturn=xQueueReceive(Test_Queue_Handle,&Queue_Message,0);
		if(xReturn==pdTRUE)
		{
			printf("\r\n the data is %d \r\n",Queue_Message);
		}
		vTaskDelay(10);
	}
}


