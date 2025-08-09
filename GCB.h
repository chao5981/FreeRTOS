#ifndef __GCB_H
#define __GCB_H

#include "stm32f10x.h"
/**
 * @brief ����LED�̵ƵĶ˿�
 * @param ��
 * @retval��
 */
#define LED_G_GPIO_PORT  GPIOB
#define LED_G_GPIO_RCC   RCC_APB2Periph_GPIOB
#define LED_G_GPIO_PIN   GPIO_Pin_0

/**
 * @brief ����LED��ƵĶ˿�
 * @param ��
 * @retval��
 */
#define LED_R_GPIO_PORT  GPIOB
#define LED_R_GPIO_RCC   RCC_APB2Periph_GPIOB
#define LED_R_GPIO_PIN   GPIO_Pin_5

/**
 * @brief ����LED���ƵĶ˿�
 * @param ��
 * @retval��
 */
#define LED_B_GPIO_PORT  GPIOB
#define LED_B_GPIO_RCC   RCC_APB2Periph_GPIOB
#define LED_B_GPIO_PIN   GPIO_Pin_1


//���ÿ���ģʽ
#define ON  0
#define OFF 1

//�ù̼������IO�˿�
#define LEDG(a) if(a) \
				GPIO_SetBits(LED_G_GPIO_PORT,LED_G_GPIO_PIN); \
				else  \
				GPIO_ResetBits(LED_G_GPIO_PORT,LED_G_GPIO_PIN)

#define LEDR(a) if(a) \
				GPIO_SetBits(LED_R_GPIO_PORT,LED_R_GPIO_PIN); \
				else  \
				GPIO_ResetBits(LED_R_GPIO_PORT,LED_R_GPIO_PIN)

#define LEDB(a) if(a) \
				GPIO_SetBits(LED_B_GPIO_PORT,LED_B_GPIO_PIN); \
				else  \
				GPIO_ResetBits(LED_B_GPIO_PORT,LED_B_GPIO_PIN)	

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define LED1_TOGGLE		 digitalToggle(LED_R_GPIO_PORT,LED_R_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED_R_GPIO_PORT,LED_R_GPIO_PIN)
#define LED1_ON			   digitalLo(LED_R_GPIO_PORT,LED_R_GPIO_PIN)

#define LED2_TOGGLE		 digitalToggle(LED_G_GPIO_PORT,LED_G_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED_G_GPIO_PORT,LED_G_GPIO_PIN)
#define LED2_ON			   digitalLo(LED_G_GPIO_PORT,LED_G_GPIO_PIN)

#define LED3_TOGGLE		 digitalToggle(LED_B_GPIO_PORT,LED_B_GPIO_PIN)
#define LED3_OFF		   digitalHi(LED_B_GPIO_PORT,LED_B_GPIO_PIN)
#define LED3_ON			   digitalLo(LED_B_GPIO_PORT,LED_B_GPIO_PIN)

/* ������ɫ������߼��÷�ʹ��PWM�ɻ��ȫ����ɫ,��Ч������ */

//��
#define LED_RED  \
					LED1_ON;\
					LED2_OFF\
					LED3_OFF

//��
#define LED_GREEN		\
					LED1_OFF;\
					LED2_ON\
					LED3_OFF

//��
#define LED_BLUE	\
					LED1_OFF;\
					LED2_OFF\
					LED3_ON

					
//��(��+��)					
#define LED_YELLOW	\
					LED1_ON;\
					LED2_ON\
					LED3_OFF
//��(��+��)
#define LED_PURPLE	\
					LED1_ON;\
					LED2_OFF\
					LED3_ON

//��(��+��)
#define LED_CYAN \
					LED1_OFF;\
					LED2_ON\
					LED3_ON
					
//��(��+��+��)
#define LED_WHITE	\
					LED1_ON;\
					LED2_ON\
					LED3_ON
					
//��(ȫ���ر�)
#define LED_RGBOFF	\
					LED1_OFF;\
					LED2_OFF\
					LED3_OFF				
				
void LED_GPIO_Init(void);

#endif
