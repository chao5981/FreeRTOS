/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-17 11:01:25
 * @LastEditors: 
 * @LastEditTime: 2025-08-17 11:52:16
 */
#include "BSP.h"
#include "USART.h"

void BSP_Init(void)
{
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/*********Ӳ���ĳ�ʼ��************/
	USART_Configuration();
	KEY_GPIO_Config();
	
	/****Ϊ�˲���Ӳ����������ó��������һ���������������Ҫע�͵�****/
	// while(1)
	// {
	// }
}
