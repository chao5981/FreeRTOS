/*
 * @Description: 
 * @Version: 
 * @Autor: chao
 * @Date: 2025-08-12 11:18:49
 * @LastEditors: 
 * @LastEditTime: 2025-08-12 20:20:21
 */
#include "exit_led.h"
/**
 * @brief ����NVIC
 * @param ��
 * @retval ��
 */
static void NVIC_Configuration()
{
	//����һ��NVIC�Ľṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	//�����������ȼ�����ռ���ȼ�����������λ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6;
	//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	//ʹ��NVIC_IQNͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//���û��߽�ֹ�ĸ��ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannel=KEY1_NVIC_IRQN;
	//��ʼ���ṹ��
	NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief ��ʼ��EXTI��GPIO
 * @param ��
 * @retval ��
 */
void Key_EXTI_Configuration()
{
	GPIO_InitTypeDef Keystructure;
	EXTI_InitTypeDef EXTIstructure;
	//Ƕ�׵���NVIC_Configuration()��ע�⣬��ֻ��Ҫ����һ��
	NVIC_Configuration();
	//********GPIO������*********//
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(KEY1_EXTI_GPIO_CLK,ENABLE);
	//ȷ������
	Keystructure.GPIO_Pin=GPIO_Pin_0;
	//ȷ������ģʽ
	Keystructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	//��ʼ��GPIO�ṹ��
	GPIO_Init(KEY1_GPIO_PORT,&Keystructure);
	//*******EXTI������*********//
	//ȷ���жϵ��������Դ
	GPIO_EXTILineConfig(KEY1_EXTI_PortSource,KEY1_EXTI_PinSource);
	//�����ĸ�������Ǹ����ŵ����ж�(ָ��EXTI��)
	EXTIstructure.EXTI_Line=KEY1_EXTI_LINE;
	//�����ǲ����¼��߻����ж���
	EXTIstructure.EXTI_Mode=EXTI_Mode_Interrupt;
	//���ñ��ؼ���·����β��ܲ����ж�
	EXTIstructure.EXTI_Trigger=EXTI_Trigger_Rising;
	//����EXTI��(��������ж��¼��Ĵ���)
	EXTIstructure.EXTI_LineCmd=ENABLE;
	//��ʼ��EXTI�ṹ��
	EXTI_Init(&EXTIstructure);
}
