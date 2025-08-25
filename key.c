#include "key.h"
/**
 * @brief ������ʼ��
 * @param ��
 * @retval ��
 */
 void KEY_GPIO_Config(void)
 {
	 //��ʼ��һ���ṹ��
	  GPIO_InitTypeDef KeyInit;
	 //����ʱ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOC,ENABLE);
	 //����KEY1
	  KeyInit.GPIO_Pin=GPIO_Pin_0;
	 //ѡ�񸡿�����
	  KeyInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	 //ѡ���ٶ�
	  KeyInit.GPIO_Speed=GPIO_Speed_50MHz;
	 //��ʼ��IO��
	  GPIO_Init(KEY1_GPIO_PORT,&KeyInit);
	 //����KEY2
	  KeyInit.GPIO_Pin=GPIO_Pin_13;
	 //ѡ�񸡿�����
	  KeyInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	 //ѡ���ٶ�
	  KeyInit.GPIO_Speed=GPIO_Speed_50MHz;
	 //��ʼ��IO��
	  GPIO_Init(KEY2_GPIO_PORT,&KeyInit);
 }
 /**
 * @brief ����Ƿ�����������
 * @param GPIO�����ͣ�GPIO������
 * @retval �����Ƿ�����(KEY_ON/KEY_OFF)
 */
 uint8_t KEY_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
 {
	 if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON)
	 {
		 while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON);
		 return KEY_ON;
	 }
	 else
	 {
			return KEY_OFF;
	 }
}
  