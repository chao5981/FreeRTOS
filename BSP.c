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
	
	/*********硬件的初始化************/
	USART_Configuration();
	KEY_GPIO_Config();
	
	/****为了测试硬件，因此先让程序卡在这里，一旦测试完后这个务必要注释掉****/
	// while(1)
	// {
	// }
}
