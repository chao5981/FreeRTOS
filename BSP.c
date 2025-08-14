/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-12 11:16:28
 * @LastEditors: 
 * @LastEditTime: 2025-08-12 11:37:55
 */
#include "BSP.h"
#include "USART.h"

void BSP_Init(void)
{
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/*********Ӳ���ĳ�ʼ��************/
	USART_Configuration();
	KEY_GPIO_Config();
	Key_EXTI_Configuration();
	LED_GPIO_Init();
	
	/****Ϊ�˲���Ӳ����������ó��������һ���������������Ҫע�͵�****/
//	while(1)
//	{
//		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
//		{
//			LEDB(ON);
//		}
//		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN)==KEY_ON)
//		{
//			LEDB(OFF);
//		}
//	}
}
