#pragma once

#include <stdio.h>
#include "Type.h"
#ifdef __cplusplus
    extern "C"
    {
    #endif 
    //////////////////////////////////////////////////////////////////////////////////	 
    //������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
    //ALIENTEK STM32������
    //����1��ʼ��		   
    //����ԭ��@ALIENTEK
    //������̳:www.openedv.com
    //�޸�����:2012/8/18
    //�汾��V1.5
    //��Ȩ���У�����ؾ���
    //Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
    //All rights reserved
    //********************************************************************************
    //V1.3�޸�˵�� 
    //֧����Ӧ��ͬƵ���µĴ��ڲ���������.
    //�����˶�printf��֧��
    //�����˴��ڽ��������.
    //������printf��һ���ַ���ʧ��bug
    //V1.4�޸�˵��
    //1,�޸Ĵ��ڳ�ʼ��IO��bug
    //2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
    //3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
    //4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
    //V1.5�޸�˵��
    //1,�����˶�UCOSII��֧��
    #define USART_REC_LEN  			200  	//�����������ֽ��� 200
    #define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

    extern byte USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
    extern ushort USART_RX_STA; //����״̬���	
    //����봮���жϽ��գ��벻Ҫע�����º궨��

    void sendToUsart1(byte ch); //�򴮿�1��������
    void sendToUsart2(byte ch); //�򴮿�2��������
    void sendToUsart3(byte ch); //�򴮿�3��������
    void sendToUsart4(byte ch); //�򴮿�4��������
    void sendToUsart5(byte ch); //�򴮿�5��������
    #ifdef __cplusplus
    }
#endif
	
