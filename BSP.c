#include "BSP.h"
#include "USART.h"

void BSP_Init(void)
{
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/*********Ӳ���ĳ�ʼ��************/
	USART_Configuration();
	
	
	/****Ϊ�˲���Ӳ����������ó��������һ���������������Ҫע�͵�****/
	// while(1)
	// {
	// }
}
