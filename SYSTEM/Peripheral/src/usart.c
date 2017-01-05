#include "stm32f10x.h"
#include "usart.h"
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
    USART1->DR = (byte)ch;
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
    byte USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.
    //����״̬
    //bit15��	������ɱ�־
    //bit14��	���յ�0x0d
    //bit13~0��	���յ�����Ч�ֽ���Ŀ
    ushort USART_RX_STA = 0; //����״̬���

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
