/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-24 11:38:19
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-09-02 11:49:21
 */
#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "limits.h"
#include "BSP.h"

#include<string.h>


/* ****************任务句柄****************** */
TaskHandle_t App_Task_Create_Handle;
TaskHandle_t Key_Task_Handle;
TaskHandle_t USART_Printf_Task_Handle;
TaskHandle_t CPU_Task_Handle;


/* *****************内核句柄********************** */



/* ******************任务函数******************** */
static void App_Task_Create(void);
static void Key_Task(void);
static void USART_Printf_Task(void);
static void CPU_Task(void);


/* ********************软件定时器函数************* */



/* *****************全局定义************************* */


/* *********************宏定义*********************** */

//用任务通知代替消息队列的宏定义
// #define USE_CHAR  1


//用任务通知代替事件的宏定义
#define KEY1_EVENT  (0x01<<0)
#define kEY2_EVENT	(0x01<<1)


int main(void)
{
	BaseType_t xReturn=pdFAIL;
	BSP_Init();
	xReturn=xTaskCreate((TaskFunction_t)App_Task_Create,"app task",512,NULL,1,&App_Task_Create_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n app task is created\r\n");
		vTaskStartScheduler();
		printf("1");
	}
	else
	{
		printf("\r\n app task error \r\n");
		while(1);
	}

}

static void App_Task_Create(void)
{
	BaseType_t xReturn=pdFAIL;
	taskENTER_CRITICAL();
	xReturn=xTaskCreate((TaskFunction_t)Key_Task,"key task",512,NULL,2,&Key_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n key task is created\r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t)USART_Printf_Task,"USART task",512,NULL,3,&USART_Printf_Task_Handle); //注意优先级的分配，否则可能由于任务抢占出现部分乱码
	if(xReturn==pdPASS)
	{
		printf("\r\n USART task is created\r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t)CPU_Task,"CPU task",512,NULL,3,&CPU_Task_Handle); //注意优先级的分配，否则可能由于任务抢占出现部分乱码
	if(xReturn==pdPASS)
	{
		printf("\r\n CPU task is created\r\n");
	}
	vTaskDelete(App_Task_Create_Handle);
	taskEXIT_CRITICAL();
}



/**
 * @brief 用任务通知代替事件组的发送函数
 * @param 无
 * @retval 无
 */
static void Key_Task(void)
{
	while(1)
	{
		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
		{
			xTaskNotify(USART_Printf_Task_Handle,KEY1_EVENT,eSetBits);
			printf("\r\n key1 set \r\n");
		}
		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN)==KEY_ON)
		{
			xTaskNotify(USART_Printf_Task_Handle,kEY2_EVENT,eSetBits);
			printf("\r\n key2 set \r\n");
		}
		vTaskDelay(20);
	}
}


/**
 * @brief 用任务通知代替事件组的接收函数
 * @param 无
 * @retval 无
 */
static void USART_Printf_Task(void)
{
	uint32_t r_event=0;		//定义一个事件的接受量
	uint32_t l_event=0;		//定义一个保存事件的变量
	while(1)
	{
		xTaskNotifyWait(0,ULONG_MAX,&r_event,0);
		l_event|=r_event;
		if(r_event==KEY1_EVENT)
		{
			printf("\r\n key 1 is put \r\n");
		}
		else if(r_event==kEY2_EVENT)
		{
			printf("\r\n key 2 is put \r\n");
		}
		else if(l_event==(KEY1_EVENT|kEY2_EVENT))
		{
			printf("\r\n key 1 and key 2 are put \r\n");
			l_event=0;
		}
		vTaskDelay(100);
	}
}

static void CPU_Task(void)
{
	char CPU_RunInfo[400];
	while (1)
	{
		memset(CPU_RunInfo,0,sizeof(CPU_RunInfo));
		vTaskList(CPU_RunInfo);
		printf("---------------------------------------------\r\n");
		printf("任务名 任务状态 优先级 剩余栈 任务序号\r\n");
		printf("%s", CPU_RunInfo);
		printf("---------------------------------------------\r\n");
		memset(CPU_RunInfo,0,sizeof(CPU_RunInfo));
		
		vTaskGetRunTimeStats(CPU_RunInfo);
 		printf("任务名 运行计数 使用率\r\n");
		printf("%s", CPU_RunInfo);
		printf("---------------------------------------------\r\n\n");
		vTaskDelay(1000);
	}
	
}
