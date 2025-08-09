#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "BSP.h"
#include "GCB.h"

/************警告屏蔽****************/
#pragma diag_suppress 191
#pragma diag_suppress 1

/***********创建任务句柄***********/

static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED_Task_Handle;

/**********创建任务栈*************/

//APP任务栈
static StackType_t AppTaskCreate_Stack[128];

//LED任务栈
static StackType_t LED_Task_Stack[128];

//空闲任务栈
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];

//定时器任务栈
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/************创建任务控制块(TCB)*************/

static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED_Task_TCB;
static StaticTask_t Idle_Task_TCB;
static StaticTask_t Timer_Task_TCB;

/***************空闲任务和定时器任务堆栈函数*******************/
//使用静态创建函数创建 FreeRTOS 任务时，需要记住一点：必须实现 vApplicationGetIdleTaskMemory 函数来完成空闲任务的内存分配
//定时器任务函数似乎可以不用
//空闲任务需要分配堆栈和任务控制块，而这些内存区域需要手动向 FreeRTOS 系统申请
//在这种情况下，我们需要实现 vApplicationGetIdleTaskMemory 函数，该函数会在空闲任务创建之前被调用，所以无需想之前的野火的一样把空闲任务放到任务列表

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
																		StackType_t **ppxTimerTaskStackBuffer, 
																		uint32_t *pulTimerTaskStackSize);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
																	 StackType_t **ppxIdleTaskStackBuffer, 
																	 uint32_t *pulIdleTaskStackSize);


/******************任务函数***********************/
static void LED_Task(void);
static void AppTaskCreate(void);

int main(void)
{
	
	BSP_Init();
	printf("这是一个STM32全系列开发板-FreeRTOS-静态创建单任务!\r\n");
	AppTaskCreate_Handle=xTaskCreateStatic((TaskFunction_t)AppTaskCreate,
																					(const char*)"APP_Task",
																					(const uint32_t) 128,
																					(void *const) NULL,
																					(UBaseType_t) 1,
																					(StackType_t*) AppTaskCreate_Stack,
																					(StaticTask_t*) &AppTaskCreate_TCB);
																					
	if(AppTaskCreate_Handle!=NULL)
	{
		//启动任务调度
		vTaskStartScheduler();
	}		
	else
	{
		printf("\r\n!!Unknown error!!\r\n");
		while(1);
	}
	
}


/**
 * @brief 所有任务的创建函数
 * @param  无
 * @retval  无
 */
static void AppTaskCreate(void)
{
	//进入临界段
	//非中断型的临界段保护不可以嵌套使用
	taskENTER_CRITICAL();
	
	//创建LED任务
	LED_Task_Handle=xTaskCreateStatic((TaskFunction_t) LED_Task,			//任务函数
																		(const char*) "LED_Task",				//任务名字
																		(const uint32_t) 128,				 	 	//任务堆栈大小
																		(void *const)NULL,							//传递给任务函数的形参
																			(UBaseType_t) 2,							//任务的优先级
																		(StackType_t*) LED_Task_Stack,	//任务栈
																		(StaticTask_t*)&LED_Task_TCB);	//任务控制块
																		
	if(LED_Task_Handle==NULL)
	{
		printf("LED_Task任务创建失败!\n");
	}
	else
	{
		printf("LED_Task任务创建成功!\n");
	}
	
	//删除AppTask任务
	vTaskDelete(AppTaskCreate_Handle);
	
	//退出临界段
	taskEXIT_CRITICAL();
}


/**
 * @brief LED任务
 * @param 无
 * @retval 无
 */
static void LED_Task(void)
{
	while(1)
	{
		LED_RED;
		vTaskDelay(500);
		printf("\r\n红灯亮\r\n");
		
		LED_GREEN;
		vTaskDelay(500);
		printf("\r\n绿灯亮\r\n");
	}
}

/**
 * @brief 	获取定时器任务的任务堆栈和任务控制块内存
 * @param 	ppxTimerTaskTCBBuffer:任务控制块内存
						ppxTimerTaskStackBuffer:任务堆栈的内存
						pulTimerTaskStackSize:任务堆栈的大小
 * @retval 
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
																		StackType_t **ppxTimerTaskStackBuffer, 
																		uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;
	*ppxTimerTaskStackBuffer=Timer_Task_Stack;
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;
}


/**
 * @brief 	获取空闲任务的任务堆栈和任务控制块内存
 * @param 	ppxIdleTaskTCBBuffer:任务控制块内存
						ppxIdleTaskStackBuffer:任务堆栈的内存
						pulIdleTaskStackSize:任务堆栈的大小
 * @retval 
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
																	 StackType_t **ppxIdleTaskStackBuffer, 
																	 uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;
	*ppxIdleTaskStackBuffer=Idle_Task_Stack;
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;
}