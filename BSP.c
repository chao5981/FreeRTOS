#include "BSP.h"
#include "USART.h"
#include "GCB.h"

void BSP_Init(void)
{
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/*********Ӳ���ĳ�ʼ��************/
	USART_Configuration();
	LED_GPIO_Init();
	
	/**************Ӳ������*************/
	
//  LED1_ON;
	
	/****Ϊ�˲���Ӳ����������ó��������һ���������������Ҫע�͵�****/
//	while(1)
//	{
//	}
}
