#include "GCB.h"

/**
 * @brief ��ʼ������LED�Ƶ�IO��
 * @param ��
 * @retval ��
 */

void LED_GPIO_Init(void)
{
	//��ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ��ʱ��
	RCC_APB2PeriphClockCmd(LED_R_GPIO_RCC|LED_B_GPIO_RCC|LED_G_GPIO_RCC,ENABLE);
//	//ѡ������
//	GPIO_InitStructure.GPIO_Pin=LED_G_GPIO_PIN;
//	//ѡ���������
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	//ѡ������ٶ�
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	//GPIO��ʼ��
//	GPIO_Init(LED_G_GPIO_PORT,&GPIO_InitStructure);
//	//��һ�����ţ���ʼ��
//	GPIO_InitStructure.GPIO_Pin=LED_R_GPIO_PIN;
//	GPIO_Init(LED_R_GPIO_PORT,&GPIO_InitStructure);
//	//��һ�����ţ���ʼ��
//	GPIO_InitStructure.GPIO_Pin=LED_B_GPIO_PIN;
//	GPIO_Init(LED_B_GPIO_PORT,&GPIO_InitStructure);
/************��������д������***************/	
	
//	//	//ѡ������
//	GPIO_InitStructure.GPIO_Pin=LED_G_GPIO_PIN;
//	//ѡ���������
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	//ѡ������ٶ�
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	//GPIO��ʼ��
//	GPIO_Init(LED_G_GPIO_PORT,&GPIO_InitStructure);
//	//��һ�����ţ���ʼ��
//	GPIO_InitStructure.GPIO_Pin=LED_R_GPIO_PIN;
//	//ѡ���������
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	//ѡ������ٶ�
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(LED_R_GPIO_PORT,&GPIO_InitStructure);
//	//��һ�����ţ���ʼ��
//	GPIO_InitStructure.GPIO_Pin=LED_B_GPIO_PIN;
//	//ѡ���������
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	//ѡ������ٶ�
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(LED_B_GPIO_PORT,&GPIO_InitStructure);
/************����д�����ԣ���Ϊ������mode��speed�������޸�***************/	

	  //ѡ������
		GPIO_InitStructure.GPIO_Pin = LED_G_GPIO_PIN | LED_R_GPIO_PIN | LED_B_GPIO_PIN;
	  //ѡ���������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		//ѡ������ٶ�
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		//GPIO��ʼ��
		GPIO_Init(GPIOB, &GPIO_InitStructure);
/************����д�������Ƽ������***************/		
		
	/* �ر�����led��	*/
	GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
		
	/* �ر�����led��	*/
	GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);	 
    
  /* �ر�����led��	*/
	GPIO_SetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN);
}
