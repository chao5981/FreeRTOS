#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "BSP.h"
#include "GCB.h"

/************��������****************/
#pragma diag_suppress 191
#pragma diag_suppress 1

/***********����������***********/

static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED_Task_Handle;

/**********��������ջ*************/

//APP����ջ
static StackType_t AppTaskCreate_Stack[128];

//LED����ջ
static StackType_t LED_Task_Stack[128];

//��������ջ
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];

//��ʱ������ջ
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/************����������ƿ�(TCB)*************/

static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED_Task_TCB;
static StaticTask_t Idle_Task_TCB;
static StaticTask_t Timer_Task_TCB;

/***************��������Ͷ�ʱ�������ջ����*******************/
//ʹ�þ�̬������������ FreeRTOS ����ʱ����Ҫ��סһ�㣺����ʵ�� vApplicationGetIdleTaskMemory ��������ɿ���������ڴ����
//��ʱ���������ƺ����Բ���
//����������Ҫ�����ջ��������ƿ飬����Щ�ڴ�������Ҫ�ֶ��� FreeRTOS ϵͳ����
//����������£�������Ҫʵ�� vApplicationGetIdleTaskMemory �������ú������ڿ������񴴽�֮ǰ�����ã�����������֮ǰ��Ұ���һ���ѿ�������ŵ������б�

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
																		StackType_t **ppxTimerTaskStackBuffer, 
																		uint32_t *pulTimerTaskStackSize);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
																	 StackType_t **ppxIdleTaskStackBuffer, 
																	 uint32_t *pulIdleTaskStackSize);


/******************������***********************/
static void LED_Task(void);
static void AppTaskCreate(void);

int main(void)
{
	
	BSP_Init();
	printf("����һ��STM32ȫϵ�п�����-FreeRTOS-��̬����������!\r\n");
	AppTaskCreate_Handle=xTaskCreateStatic((TaskFunction_t)AppTaskCreate,
																					(const char*)"APP_Task",
																					(const uint32_t) 128,
																					(void *const) NULL,
																					(UBaseType_t) 1,
																					(StackType_t*) AppTaskCreate_Stack,
																					(StaticTask_t*) &AppTaskCreate_TCB);
																					
	if(AppTaskCreate_Handle!=NULL)
	{
		//�����������
		vTaskStartScheduler();
	}		
	else
	{
		printf("\r\n!!Unknown error!!\r\n");
		while(1);
	}
	
}


/**
 * @brief ��������Ĵ�������
 * @param  ��
 * @retval  ��
 */
static void AppTaskCreate(void)
{
	//�����ٽ��
	//���ж��͵��ٽ�α���������Ƕ��ʹ��
	taskENTER_CRITICAL();
	
	//����LED����
	LED_Task_Handle=xTaskCreateStatic((TaskFunction_t) LED_Task,			//������
																		(const char*) "LED_Task",				//��������
																		(const uint32_t) 128,				 	 	//�����ջ��С
																		(void *const)NULL,							//���ݸ����������β�
																			(UBaseType_t) 2,							//��������ȼ�
																		(StackType_t*) LED_Task_Stack,	//����ջ
																		(StaticTask_t*)&LED_Task_TCB);	//������ƿ�
																		
	if(LED_Task_Handle==NULL)
	{
		printf("LED_Task���񴴽�ʧ��!\n");
	}
	else
	{
		printf("LED_Task���񴴽��ɹ�!\n");
	}
	
	//ɾ��AppTask����
	vTaskDelete(AppTaskCreate_Handle);
	
	//�˳��ٽ��
	taskEXIT_CRITICAL();
}


/**
 * @brief LED����
 * @param ��
 * @retval ��
 */
static void LED_Task(void)
{
	while(1)
	{
		LED_RED;
		vTaskDelay(500);
		printf("\r\n�����\r\n");
		
		LED_GREEN;
		vTaskDelay(500);
		printf("\r\n�̵���\r\n");
	}
}

/**
 * @brief 	��ȡ��ʱ������������ջ��������ƿ��ڴ�
 * @param 	ppxTimerTaskTCBBuffer:������ƿ��ڴ�
						ppxTimerTaskStackBuffer:�����ջ���ڴ�
						pulTimerTaskStackSize:�����ջ�Ĵ�С
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
 * @brief 	��ȡ��������������ջ��������ƿ��ڴ�
 * @param 	ppxIdleTaskTCBBuffer:������ƿ��ڴ�
						ppxIdleTaskStackBuffer:�����ջ���ڴ�
						pulIdleTaskStackSize:�����ջ�Ĵ�С
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