#include "TInterrupt.h"
#include "SerialPort.h"

TInterrupt Interrupt;

typedef void (*OnUsartReceive)(ushort num, void *param);

SerialPort *onSerialPortRcv[5];

void TInterrupt::SetPriority(byte intp,byte level)
{
	
}
void TInterrupt::Activate(byte irq, void *OnUsartReceive, void * param)
{
	SerialPort *sp=(SerialPort*)param;
	onSerialPortRcv[irq]=sp;
}
//�ر��ж�
void TInterrupt::Deactivate(byte irq)
{
	
}
extern SerialPort *_printf_sp;
#ifdef __cplusplus
    extern "C"
    {
    #endif 
//����1�жϷ������
    //ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
    /*
     */
    void USART1_IRQHandler(void) //����1�жϷ������
    {
        #if 0
            if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
            //���յ�һ�ֽ�
            {

                USART_ClearITPendingBit(USART1, USART_IT_RXNE);
                byte inch = USART1->DR; //��ȡ���յ�������
                printf("COM1 in data \r\n");
                com1buf.Push(inch);
                com1timeidle = 0; //���м�ʱ������
            }
        #else 
//            if (_printf_sp)
//            {
//				
//                SerialPort::OnUsartReceive(0, _printf_sp);
//            }
			if(onSerialPortRcv[0])
			{
				SerialPort::OnUsartReceive(0, onSerialPortRcv[0]);
			}
        #endif 
    }	
void USART2_IRQHandler(void) //����1�жϷ������
    {
		#if 0
        if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
        {
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
            byte inch = USART_ReceiveData(USART2); //��ȡ���յ�������

//            com2buf.Push(inch);
//            com2timeidle = 0; //���м�ʱ������
        }
		#else
		if(onSerialPortRcv[1])
			{
				SerialPort::OnUsartReceive(1, onSerialPortRcv[1]);
			}
		#endif
    }
    void USART3_IRQHandler(void) //����1�жϷ������
    {
		#if 0
        if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
        {
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
            byte inch = USART_ReceiveData(USART3); //��ȡ���յ�������
            printf("com3 in data \r\n");
//            com3buf.Push(inch);
//            com3timeidle = 0; //���м�ʱ������
        }
		#else
		if(onSerialPortRcv[2])
			{
				SerialPort::OnUsartReceive(2, onSerialPortRcv[2]);
			}
		#endif
    }
    void USART4_IRQHandler(void) //����1�жϷ������
    {
        if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
        {
            USART_ClearITPendingBit(UART4, USART_IT_RXNE);
            byte inch = USART_ReceiveData(UART4); //��ȡ���յ�������
        }
    }
    void USART5_IRQHandler(void) //����1�жϷ������
    {
        if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
        //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
        {
            USART_ClearITPendingBit(UART5, USART_IT_RXNE);
            byte inch = USART_ReceiveData(UART5); //��ȡ���յ�������
        }
    }	
		
		    #ifdef __cplusplus
    }
#endif 
