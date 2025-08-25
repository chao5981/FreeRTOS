/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-24 11:38:19
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-08-24 14:55:36
 */
#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "BSP.h"


/* ****************������****************** */
TaskHandle_t App_Task_Create_Handle;
TaskHandle_t Key_Task_Handle;


/* *****************�ں˾��********************** */
TimerHandle_t Swtmr1_Handle;
TimerHandle_t Swtmr2_Handle;

/* ******************������******************** */
static void App_Task_Create(void);
static void Key_Task(void);


/* ********************�����ʱ������************* */
static void Swtmr1_Callback(void);
static void Swtmr2_Callback(void);


/* *****************ȫ�ֶ���************************* */
static uint8_t count1=0;
static uint8_t count2=0;



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
	Swtmr1_Handle=xTimerCreate("swtmr1",1000,pdFALSE,(void*)1,(TimerCallbackFunction_t)Swtmr1_Callback);
	if(Swtmr1_Handle!=NULL)
	{
		printf("\r\n timer1 is created \r\n");
		xTimerStart(Swtmr1_Handle,0);
	}
	Swtmr2_Handle=xTimerCreate("swtmr2",1000,pdTRUE,(void*)2,(TimerCallbackFunction_t)Swtmr2_Callback);
	if(Swtmr2_Handle!=NULL)
	{
		printf("\r\n timer2 is created \r\n");
		xTimerStart(Swtmr2_Handle,0);
	}
	xReturn=xTaskCreate((TaskFunction_t)Key_Task,"key task",512,NULL,2,&Key_Task_Handle);
	if(xReturn==pdPASS)
	{
		printf("\r\n key task is created \r\n");
	}
	vTaskDelete(App_Task_Create_Handle);
	taskEXIT_CRITICAL();
}

static void Swtmr1_Callback(void)
{
	count1++;
	printf("\r\n timer1 run at %d  \r\n",count1);
}


static void Swtmr2_Callback(void)
{
	TickType_t tick_num2;
	count2++;
	tick_num2=xTaskGetTickCount();
	printf("\r\n �δ�ʱ������ֵΪ%d \r\n",tick_num2);
	printf("\r\n timer2 run at %d  \r\n",count2);
}

static void Key_Task(void)
{
	BaseType_t xReturn=pdFAIL;
	while(1)
	{
		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
		{
			xReturn=xTimerDelete(Swtmr1_Handle,0);
			if(xReturn==pdPASS)
			{
				printf("\r\n �δ�ʱ��1�Ѿ���ɾ�� \r\n");
			}
		}
		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN)==KEY_ON)
		{
			xReturn=xTimerStop(Swtmr2_Handle,0);
			if(xReturn==pdPASS)
			{
				printf("\r\n �δ�ʱ��2�Ѿ���ֹͣ \r\n");
			}
		}
	}
}

