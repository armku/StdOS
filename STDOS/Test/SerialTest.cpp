#include "TInterrupt.h"
#include "TTime.h"
#include "SerialPort.h"
#include "Fifo.h"
#include "stm32f10x.h"

#define DEBUGSerialPort

#ifdef DEBUGSerialPort
    void SerialPort::Test(){

    }
    void Rs485Test();
    void SerialPortTest()
    {
        Rs485Test();
    }
    //#define ISOV2
    #define ISOV3

    //    uint OnUsart485Read(ITransport *transport, Buffer &bs, void *para, void *para2)
    //    {
    //        //transport->Write(bs);
    //        bs.Show();
    //        return 0;
    //    }

    OutputPort port485;

    //SerialPort com485(COM2);
    void USART2_Config();
    void Rs485Test()
    {
		#ifdef ISOV2
			port485.Set(PB5);
		#elif defined ISOV3
			port485.Set(PC2);
		#endif 
		port485.OpenDrain = false;
		port485.Invert = 0;
		port485.Open();
		port485=0;

        //        //com485.RS485=&port485;
        //        com485.SetBaudRate(115200);
        //        com485.Register(OnUsart485Read);
        //        com485.Open();


        USART2_Config(); //����2�����շ�485������
        uint8_t temp;
        while (1)
        {
            //GPIO_ResetBits(GPIOC, GPIO_Pin_2); //�������ģʽ
            while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != SET)
                ;
            //��ѯֱ��485���յ�����
            temp = USART_ReceiveData(USART2);
            printf("%02X ", temp);

            temp++; //��temp��ֵ��1���ٷ��͸�����

            //    GPIO_SetBits(mac485REDE_PORT,mac485REDE_PIN); //���뷢��ģʽ
            //    USART_SendData(USART2, temp);	 //��������
            //    while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);//��ѯֱ�������������
            //    printf("\r\n�������ݳɹ���\r\n");
        }
    }
#endif 
/*
 * ��������USART2_Config
 * ����  ��USART2 GPIO ����,����ģʽ����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void USART2_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* config USART2 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* USART2 GPIO config */
    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 485 ���շ���ʹ�� GPIO 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* USART2 mode config */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);
}
