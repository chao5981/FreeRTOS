#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "BSP.h"

#include <string.h>


/* ****************任务句柄********************* */
TaskHandle_t App_Task_Create_Handle;
TaskHandle_t Key1_Printf_Task_Handle;
TaskHandle_t USART_DMA_Printf_Handle;


/* ******************内核句柄*********************** */


/* ******************全局变量*********************** */


/* ******************宏定义*********************** */
extern char TX_Buffer[TOP_SIZE];


/* ******************任务函数*********************** */
static void App_Task_Create(void);
static void Key1_Printf_Task(void);
static void USART_DMA_Printf_Task(void);
//void printBinary(int num);

int main(void)
{
	BaseType_t xReturn=pdFAIL;
	BSP_Init();
	xReturn=xTaskCreate((TaskFunction_t)App_Task_Create,"app task",512,NULL,1,&App_Task_Create_Handle);
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
	xReturn=xTaskCreate((TaskFunction_t)Key1_Printf_Task,"key1 printf task",128,NULL,2,&Key1_Printf_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n key1 printf task is created \r\n");
	}
	xReturn=xTaskCreate((TaskFunction_t)USART_DMA_Printf_Task,"usart dma printf task",256,NULL,2,&USART_DMA_Printf_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n usart dma printf task is created \r\n");
	}
	vTaskDelete(App_Task_Create_Handle);
	taskEXIT_CRITICAL();
}

static void Key1_Printf_Task(void)
{
	uint8_t flag=0;
//	uint32_t ISR=0;
//	uint32_t CCR=0;
	while(1)
	{
		flag=ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		if(flag==1)
		{
			printf("\r\n key1 is put \r\n");
			
			//测试DMA中断用到的
			printf("\r\n the message is %s \r\n",TX_Buffer);
			memset(TX_Buffer,0,TOP_SIZE);
//			CCR=*(volatile uint32_t*)(0x40020058);
//			ISR=*(volatile uint32_t*)(0x40020000);
//			printBinary(ISR);
//			printBinary(CCR);
		}
		vTaskDelay(100);
	}
}


static void USART_DMA_Printf_Task(void)
{
	uint8_t flag=0;
	while (1)
	{
		flag=ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		if(flag==1)
		{
			printf("\r\n the message is %s \r\n",TX_Buffer);
			memset(TX_Buffer,0,TOP_SIZE);
		}
		vTaskDelay(100);
	}
	
}

//void printBinary(int num) 
//{
//		int i=0;
//    for (i = 31; i >= 0; i--) 
//		{  // 打印8位二进制
//        printf("%d", (num >> i) & 1);
//    }
//    printf("\n");
//}
