/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-24 11:38:19
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-08-27 11:00:04
 */
#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "limits.h"
#include "BSP.h"


/* ****************任务句柄****************** */
TaskHandle_t App_Task_Create_Handle;
TaskHandle_t Key_Task_Handle;
TaskHandle_t USART_Printf_Task_Handle;


/* *****************内核句柄********************** */



/* ******************任务函数******************** */
static void App_Task_Create(void);
static void Key_Task(void);
static void USART_Printf_Task(void);


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
	vTaskDelete(App_Task_Create_Handle);
	taskEXIT_CRITICAL();
}


/**
 * @brief 用任务通知代替消息队列的发送函数
 * @param 无
 * @retval 无
 */
// static void Key_Task(void)
// {
// 	BaseType_t xReturn=pdFALSE;
// #if USE_CHAR  
// 	char str[]="消息队列测试";
// #else
// 	uint32_t send1=2;
// #endif
// 	while(1)
// 	{
// 		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
// 		{
// 			xReturn=xTaskNotify(USART_Printf_Task_Handle,
// #if USE_CHAR
// 											(uint32_t)&str,
// #else
// 											send1,
// #endif
// 											eSetValueWithOverwrite);
// 			if(xReturn==pdPASS)
// 			{
// 				printf("\r\n success \r\n");
// 			}
// 			else if(xReturn==pdFAIL)
// 			{
// 				printf("\r\n error \r\n");
// 			}
// 		}
// 		vTaskDelay(50);
// 	}
// }


/**
 * @brief 用任务通知代替消息队列的接收函数
 * @param 无
 * @retval 无
 */
// static void USART_Printf_Task(void)
// {
// 	BaseType_t xReturn=pdFAIL;
// #if USE_CHAR
// 	char*str;
// #else
// 	uint32_t num;
// #endif
// 	while(1)
// 	{
// 		xReturn=xTaskNotifyWait(0,ULONG_MAX,
// #if	USE_CHAR
// 								(uint32_t*)&str,
// #else
// 										&num,
// #endif									
// 								portMAX_DELAY);

// 		if(xReturn==pdPASS)
// 		{
// #if	USE_CHAR
// 			printf("\r\n message is %s \r\n",str);
// #else
// 			printf("\r\n message is %d \r\n",num);
// #endif
// 		}
// 		vTaskDelay(50);
// 	}
// }


/**
 * @brief 用任务通知模拟二值信号量发送函数
 * @param 无
 * @retval 无
 */
// static void Key_Task(void)
// {
// 	BaseType_t xReturn=pdFALSE;
// 	while(1)
// 	{
// 		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
// 		{
// 			xReturn=xTaskNotifyGive(USART_Printf_Task_Handle);
// 			if(xReturn==pdPASS)
// 			{
// 				printf("\r\n send successfully \r\n");
// 			}
// 		}
// 		vTaskDelay(50);
// 	}
// }

/**
 * @brief 用任务通知模拟二值信号量接收函数
 * @param 无
 * @retval 无
 */
// static void USART_Printf_Task(void)
// {
// 	while(1)
// 	{
// 		ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
// 		printf("\r\n receive successfully \r\n");
// 		vTaskDelay(100);
// 	}
// }


/**
 * @brief 用任务通知模拟递减信号量的发送函数
 * @param 无
 * @retval 无
 */
// static void Key_Task(void)
// {
// 	BaseType_t xReturn=pdFALSE;
// 	while(1)
// 	{
// 		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
// 		{
// 			xReturn=xTaskNotifyGive(USART_Printf_Task_Handle);
// 			if(xReturn==pdPASS)
// 			{
// 				printf("\r\n send successfully \r\n");
// 			}
// 		}
// 		vTaskDelay(50);
// 	}
// }



/**
 * @brief 用任务通知模拟递减信号量的接收函数
 * @param 无
 * @retval 无
 */
// static void USART_Printf_Task(void)
// {
// 	uint32_t num=pdTRUE;
// 	while(1)
// 	{
// 		num=ulTaskNotifyTake(pdFALSE,0);
// 		printf("\r\n 递减信号量为%d \r\n",num);
// 		vTaskDelay(1000);
// 	}
// }


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
