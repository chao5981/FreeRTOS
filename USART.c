
/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-29 10:19:29
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-08-30 09:26:16
 */

#include "USART.h"


char TX_Buffer[TOP_SIZE];


/**
 * @brief 配置DMA在NVIC的优先级
 * @param 无
 * @retval 无
 */
static void USART_NVIC_Configuration(void)
{
	NVIC_InitTypeDef DMA_NVIC_structure;
	//一般地，我们在USART中选择2分组
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_4);
	//配置抢占优先级
	DMA_NVIC_structure.NVIC_IRQChannelPreemptionPriority=6;
	//配置子优先级
	DMA_NVIC_structure.NVIC_IRQChannelSubPriority=0;
	//选择中断渠道
	DMA_NVIC_structure.NVIC_IRQChannel=DEBUG_USART_IRQ;
	//使能中断
	DMA_NVIC_structure.NVIC_IRQChannelCmd=ENABLE;
	//初始化NVIC结构体
	NVIC_Init(&DMA_NVIC_structure);
}


/**
 * @brief 配置DMA，配合USART完成接受和发送数据，DMA用于重发数据给电脑
 * @param  无
 * @retval  无
 */
void USARTx_DMA_Configuration(void)
{
	DMA_InitTypeDef DEBUG_USART_DMA_Sturcture;
	//开启时钟
	RCC_AHBPeriphClockCmd(USART_DMA_CLK,ENABLE);
	//要传输的数据数量
	DEBUG_USART_DMA_Sturcture.DMA_BufferSize=TOP_SIZE;
	//数据的传输方向，SRC是从外设到内存，DST是从内存到外设
	DEBUG_USART_DMA_Sturcture.DMA_DIR=DMA_DIR_PeripheralSRC;
	//关闭从寄存器到寄存器模式(只有单纯的从寄存器到寄存器且没有任何外设的情况下才可以启用)
	DEBUG_USART_DMA_Sturcture.DMA_M2M=DMA_M2M_Disable;
	//目标端的地址，注意，DMA_MemoryBaseAddr的类型是uint32_t，而TX-Buffer是指针,所以需要强转
	DEBUG_USART_DMA_Sturcture.DMA_MemoryBaseAddr=(uint32_t)TX_Buffer;
	//目标端的每项数据的大小
	DEBUG_USART_DMA_Sturcture.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	//目标端是否每传输一项自动增加"存储器地址"
	DEBUG_USART_DMA_Sturcture.DMA_MemoryInc=DMA_MemoryInc_Enable;
	//DMA模式选择，_Normal:一次传输就停止;Circular:自动重启
	DEBUG_USART_DMA_Sturcture.DMA_Mode=DMA_Mode_Normal;
	//源头端的地址，注意，DMA_PeripheralBaseAdd的类型是uint32_t，而&USART1->DR是指针,所以需要强转
	DEBUG_USART_DMA_Sturcture.DMA_PeripheralBaseAddr=(uint32_t)&USART1->DR;
	//源头端的每项数据的大小
	DEBUG_USART_DMA_Sturcture.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	//源头端是否每传输一项自动增加"存储器地址"
	DEBUG_USART_DMA_Sturcture.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	//设置通道优先级，当多个通道同时抢占总线时起到作用
	DEBUG_USART_DMA_Sturcture.DMA_Priority=DMA_Priority_High;
	//初始化结构体
	DMA_Init(USART_RX_DMA_CHANNEL, &DEBUG_USART_DMA_Sturcture);
	
	
}


/**
 * @brief 配置USART和GPIO的相关引脚
 * @param 无
 * @retval 无
 */
void USART_Configuration(void)
{
	USART_InitTypeDef DEBUG_USART_structure;
	GPIO_InitTypeDef  DEBUG_USART_TX_GPIO_structure,DEBUG_USART_RX_GPIO_structure;
	//配置USART和GPIO的时钟
	DEBUG_USART_APBxClkCmd(RCC_APB2Periph_USART1,ENABLE);
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK,ENABLE);
	//配置USART在GPIO的TX的引脚
	//选择推挽复用输出
	DEBUG_USART_TX_GPIO_structure.GPIO_Mode=GPIO_Mode_AF_PP;
	DEBUG_USART_TX_GPIO_structure.GPIO_Pin= DEBUG_USART_TX_GPIO_PIN;
	DEBUG_USART_TX_GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
	//初始化TX引脚
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT,&DEBUG_USART_TX_GPIO_structure);
	//配置USART在GPIO的RX的引脚
	//选择浮空输入模式
	DEBUG_USART_RX_GPIO_structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	DEBUG_USART_RX_GPIO_structure.GPIO_Pin= DEBUG_USART_RX_GPIO_PIN;
	//初始化RX引脚
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT,&DEBUG_USART_RX_GPIO_structure);
	//配置USART结构体
	//USART波特率选择最高115200
	DEBUG_USART_structure.USART_BaudRate=DEBUG_USART_BAUDRATE;
	//USART硬件流控制，因为我们这里不是高速大量数据传输，因此无需硬件控制流
	DEBUG_USART_structure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	//USART传输模式，选择全双工模式，收发一起
	DEBUG_USART_structure.USART_Mode=USART_Mode_Rx| USART_Mode_Tx;
	//USART校验符，这里不需要，选择无
	DEBUG_USART_structure.USART_Parity=USART_Parity_No;
	//USART停止符，选择1
	DEBUG_USART_structure.USART_StopBits=USART_StopBits_1;
	//USART传输数据字长，选择8b
	DEBUG_USART_structure.USART_WordLength=USART_WordLength_8b;
	//初始化USART结构体
	USART_Init(DEBUG_USARTx,&DEBUG_USART_structure);
	
}

void USART_DMA_Init(void)
{
	
  DMA_DeInit(USART_RX_DMA_CHANNEL);
	
	USARTx_DMA_Configuration();
	
	USART_Configuration();
	
	USART_NVIC_Configuration();
	
	USARTx_DMA_Configuration();
	
	// 开启 串口空闲IDEL 中断
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);  
	
	//开启USARTx与DMA的通道，也就是手册说的“设置USART_CR3寄存器上的DMAT位激活”
	USART_DMACmd(DEBUG_USARTx,USART_DMAReq_Rx, ENABLE);
		
	
//	DMA_Cmd(DMA1_Channel5,ENABLE);
	
	//使能USART
	USART_Cmd(DEBUG_USARTx,ENABLE);
}

/**
 * @brief 传输一个字节
 * @param 传入一个指针和要传输的数据
 * @retval 无
 */
void USART_SendByte(USART_TypeDef* pUSARTx,uint16_t Data)
{
	 USART_SendData(pUSARTx,Data);
	 while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

/**
 * @brief 传输一个8位数字的数组
 * @param 传入一个指针，一个8位数字的数组，要传入的数组中的个数
 * @retval 无
 */
void USART_SendArray(USART_TypeDef* pUSARTx,uint8_t *array,uint16_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		USART_SendByte(pUSARTx,array[i]);	
	}
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

/**
 * @brief 传输一个字符串
 * @param 传入一个指针，字符串的指针
 * @retval 无
 */
void USART_SendString(USART_TypeDef* pUSARTx,char *str)
{
	unsigned int i=0;
	while(*(str+i)!='\0')
	{
		USART_SendByte(pUSARTx,*(str+i));
		i++;
	}
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

/**
 * @brief 传输一个16位的数字
 * @param 传入一个指针，16位的数字
 * @retval 无
 */
void USART_SendHalfWord(USART_TypeDef* pUSARTx,uint16_t Data)
{
	uint8_t temp_h,temp_l;
	temp_h=(Data&0xFF00)>>8;
	temp_l=Data&0xFF;
	USART_SendData(pUSARTx,temp_h);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
	USART_SendData(pUSARTx,temp_l);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}
