/*
*********************************************************************************************************
*
*	ģ������ : �ײ�����ģ��
*	�ļ����� : bsp.h
*	��    �� : V1.0
*	˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ���
*	 	       Ӧ�ó���ֻ�� #include bsp.h ���ɣ�����Ҫ#include ÿ��ģ��� h �ļ�
*
*	�޸ļ�¼ :
*		�汾��  ����         ����    ˵��
*		v1.0    2012-12-17  Eric2013  ST�̼���V1.0.2�汾��
*	
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_

#define STM32_V5
//#define STM32_X3


/* ����Ƿ����˿������ͺ� */
#if !defined (STM32_V5) && !defined (STM32_X3)
	#error "Please define the board model : STM32_X3 or STM32_V5"
#endif

/* ���� BSP �汾�� */
#define __STM32F1_BSP_VERSION		"1.1"

/* CPU����ʱִ�еĺ��� */
//#define CPU_IDLE()		bsp_Idle()

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

/* ���������ڵ��Խ׶��Ŵ� */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/*
	EXTI9_5_IRQHandler ���жϷ�������ɢ�ڼ��������� bsp�ļ��С�
	��Ҫ���ϵ� stm32f4xx_it.c �С�

	���������б�ʾEXTI9_5_IRQHandler��ں������зŵ� stm32f4xx_it.c��
*/
#define EXTI9_5_ISR_MOVE_OUT

#define DEBUG_GPS_TO_COM1	/* ��ӡGPS���ݵ�����1 */

/* ͨ��ȡ��ע�ͻ������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
#include "bsp_uart_fifo.h"
//#include "bsp_led.h"
#include "bsp_timer.h"
#ifdef __cplusplus
extern "C"  {
#endif

void bsp_Idle(void);
//void bsp_Init(void);
//static void NVIC_Configuration(void);
#ifdef __cplusplus
}
#endif
#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
