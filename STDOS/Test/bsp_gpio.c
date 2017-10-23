/**
  ******************************************************************************
  * @file    bsp_gpio.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   gpioӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_gpio.h"   

 /**
  * @brief  ��ʼ��GPIO
  * @param  ��
  * @retval ��
  */
void GPIO_Config( void )
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

	
		/*����GPIOA������ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE ); 

    /* ����WiFiģ���Ƭѡ��CH������	��λ������RST������*/
		/*ѡ��Ҫ���Ƶ�GPIOA0��CH�����ź�GPIOA1��RST������*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIOA*/
		GPIO_Init( GPIOA, &GPIO_InitStructure );	 
	

		/* ����WiFiģ���Ƭѡ����	*/
		GPIO_ResetBits( GPIOA, GPIO_Pin_0 );		
		
		/* ����WiFiģ��ĸ�λ��������	*/
		GPIO_SetBits( GPIOA, GPIO_Pin_1 );
		
 
}
/*********************************************END OF FILE**********************/
