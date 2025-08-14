#ifndef GCB_H
#define GCB_H

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

//��HAL�̼������IO�˿�
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
				
void LED_GPIO_Init(void);

#endif
