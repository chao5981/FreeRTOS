/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-15 09:27:59
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-08-15 11:25:37
 */
#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "BSP.h"

/* *************����������************** */
TaskHandle_t App_Task_Create_Handle;
TaskHandle_t USART_Printf_Task_Handle;
TaskHandle_t Key_Scan_Task_Handle;
TaskHandle_t LED_Task_Handle;

/* **************�ں˶�����*************** */
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */

QueueHandle_t Test_Queue_Handle=NULL;


/* ************������صĺ궨��************** */
#define Test_Queue_Length    4
#define Test_Queue_Size 	 sizeof(uint8_t)

/* **************���񴴽�����***************** */
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
 * @brief ����������
 * @param ��
 * @retval ��
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
 * @brief ����ɨ��������
 * @param ��
 * @retval ��
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
 * @brief LED������
 * @param ��
 * @retval ��
 */
static void LED_Task(void)
{
	BaseType_t xReturn=pdTRUE;
	uint32_t Queue_Massage=0;
	while (1)
	{
		xReturn=xQueuePeek(Test_Queue_Handle,&Queue_Massage,0);
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
 * @brief ���ڴ�ӡ���ݺ���
 * @param ��
 * @retval ��
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

