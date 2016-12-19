#include "sys.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}

void sendch(int ch)
{
    while ((USART1->SR &0X40) == 0){}
    //ѭ������,ֱ���������
    USART1->DR = (uint8_t)ch;
}

//�ض���fputc����
int fputc(int ch, FILE *f)
{
    sendch(ch);
    return ch;
}

//c++��
void _ttywrch(int ch)
{
    sendch(ch);
}

int GetKey(void)
{

    while (!(USART1->SR &USART_FLAG_RXNE))
        ;

    return ((int)(USART1->DR &0x1FF));
}

#if EN_USART1_RX   //���ʹ���˽���
    //����1�жϷ������
    //ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
    uint8_t USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.
    //����״̬
    //bit15��	������ɱ�־
    //bit14��	���յ�0x0d
    //bit13~0��	���յ�����Ч�ֽ���Ŀ
    uint16_t USART_RX_STA = 0; //����״̬���

    /*
    �����յ��ӵ��Է����������ݣ��ѽ��յ������ݱ����� USART_RX_BUF �У�ͬʱ�ڽ���
    ״̬�Ĵ����� USART_RX_STA���м������յ�����Ч���ݸ��������յ��س����س��ı�ʾ�� 2
    ���ֽ���ɣ� 0X0D �� 0X0A���ĵ�һ���ֽ� 0X0D ʱ�����������������ӣ��ȴ� 0X0A �ĵ�����
    ����� 0X0A û������������Ϊ��ν���ʧ�ܣ����¿�ʼ��һ�ν��ա����˳�����յ� 0X0A��
    ���� USART_RX_STA �ĵ� 15 λ���������һ�ν��գ����ȴ���λ����������������Ӷ���
    ʼ��һ�εĽ��գ�������ٳ�û���յ� 0X0D����ô�ڽ������ݳ��� USART_REC_LEN ��ʱ��
    ��ᶪ��ǰ������ݣ����½��ա�
     */

#endif 
//�򴮿�1��������
void sendToUsart1(uint8_t ch)
{
    #if 0
        while ((USART1->SR &0X40) == 0){}
        //ѭ������,ֱ���������
        USART1->DR = (uint8_t)ch;
    #else 
        USART_SendData(USART1, ch);

        /* �ȴ�������� */
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
    #endif 
}

//�򴮿�2��������
void sendToUsart2(uint8_t ch)
{
    USART_SendData(USART2, ch);

    /* �ȴ�������� */
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}
}

//�򴮿�3��������
void sendToUsart3(uint8_t ch)
{
    USART_SendData(USART3, ch);

    /* �ȴ�������� */
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
}

//�򴮿�4��������
void sendToUsart4(uint8_t ch)
{
    USART_SendData(UART4, ch);

    /* �ȴ�������� */
    while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET){}
}

//�򴮿�5��������
void sendToUsart5(uint8_t ch)
{
    USART_SendData(UART5, ch);

    /* �ȴ�������� */
    while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET){}
}
