#include "SerialPortOld.h"
#include "stm32f10x.h"
#include "usart.h"
#include "InputPort.h"
#include "AlternatePort.h"

SerialPortOld::SerialPortOld(COM_Def comPort, uint bound)
{
    this->ComPort = comPort;

    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    InputPort *tx;
    InputPort *rx;
    switch (this->ComPort)
    {
        case COM1:
            //����1����
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ʹ��USART1

            tx = new InputPort(PA9);
            rx = new InputPort(PA10);


            //Usart1 NVIC ����
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�3
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�3
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
            NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

            //USART ��ʼ������

            USART_InitStructure.USART_BaudRate = bound; //���ڲ�����
            USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
            USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
            USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
            USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
            USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ

            USART_Init(USART1, &USART_InitStructure); //��ʼ������1
            USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
            USART_Cmd(USART1, ENABLE); //ʹ�ܴ���1
            USART_ClearFlag(USART1, USART_FLAG_TC);
            break;
        case COM2:
            tx=new InputPort(PA2);
            rx=new InputPort(PA3);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

            //Usart2 NVIC ����
            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�3
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�3
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
            NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

            //USART ��ʼ������

            USART_Init(USART2, &USART_InitStructure); //��ʼ������2
            USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
            USART_Cmd(USART2, ENABLE); //ʹ�ܴ���2
            USART_ClearFlag(USART2, USART_FLAG_TC);
            break;
        case COM3:
            tx=new InputPort(PB11);
            rx=new InputPort(PB10);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

            //Usart3 NVIC ����
            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
            NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

            //USART ��ʼ������

            USART_Init(USART3, &USART_InitStructure); //��ʼ������3
            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
            USART_Cmd(USART3, ENABLE); //ʹ�ܴ���3
            USART_ClearFlag(USART3, USART_FLAG_TC);
            break;
        case COM4:
            tx=new InputPort(PC10);
            rx=new InputPort(PC11);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

            //Usart4 NVIC ����
            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
            NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

            //USART ��ʼ������

            USART_Init(UART4, &USART_InitStructure); //��ʼ������4
            USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
            USART_Cmd(UART4, ENABLE); //ʹ�ܴ���4
            USART_ClearFlag(UART4, USART_FLAG_TC);
            break;
        case COM5:
            tx=new InputPort(PC12);
            rx=new InputPort(PD3);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

            //Usart5 NVIC ����
            //NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
            NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

            //USART ��ʼ������

            USART_Init(UART5, &USART_InitStructure); //��ʼ������5
            USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
            USART_Cmd(UART5, ENABLE); //ʹ�ܴ���5
            USART_ClearFlag(UART5, USART_FLAG_TC);
            break;
        default:
            break;
    }
    tx->SetModeAF_PP();
    rx->SetModeIN_FLOATING();
}

#ifdef __cplusplus
    extern "C"
    {
    #endif 
    #if EN_USART1_RX   //���ʹ���˽���
        //����1�жϷ������
        //ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
        /*
         */
        void USART1_IRQHandler(void) //����1�жϷ������
        {
            if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
            //���յ�һ�ֽ�
            {
                USART_ClearITPendingBit(USART1, USART_IT_RXNE);
                byte inch = USART1->DR; //��ȡ���յ�������

                com1buf.Push(inch);
                com1timeidle = 0; //���м�ʱ������
            }
        }
    #endif 
    #if EN_USART2_RX   //���ʹ���˽���

        void USART2_IRQHandler(void) //����1�жϷ������
        {
            if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
            //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
            {
                USART_ClearITPendingBit(USART2, USART_IT_RXNE);
                byte inch = USART_ReceiveData(USART2); //��ȡ���յ�������


            }
        }
    #endif 
    #ifdef __cplusplus
    }
#endif
