#pragma once

#include <stdio.h>
#include "Type.h"
#ifdef __cplusplus
    extern "C"
    {
    #endif     
    #define USART_REC_LEN  			200  	//�����������ֽ��� 200
    #define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

    extern byte USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
    extern ushort USART_RX_STA; //����״̬���	
    //����봮���жϽ��գ��벻Ҫע�����º궨��
    #ifdef __cplusplus
    }
#endif
	
