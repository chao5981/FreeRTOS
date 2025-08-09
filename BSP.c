#include "BSP.h"
#include "USART.h"
#include "GCB.h"

void BSP_Init(void)
{
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/*********硬件的初始化************/
	USART_Configuration();
	LED_GPIO_Init();
	
	/**************硬件调试*************/
	
//  LED1_ON;
	
	/****为了测试硬件，因此先让程序卡在这里，一旦测试完后这个务必要注释掉****/
//	while(1)
//	{
//	}
}
