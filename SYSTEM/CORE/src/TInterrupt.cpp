#include "TInterrupt.h"
#include "SerialPort.h"

TInterrupt Interrupt;

typedef void (*OnUsartReceive)(ushort num, void *param);

OnUsartReceive onSerialPortRcv[5];

void TInterrupt::SetPriority(byte intp,byte level)
{
	
}
void TInterrupt::Activate(byte irq, void *OnUsartReceive, void * param)
{
	
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
            if (_printf_sp)
            {
				
                SerialPort::OnUsartReceive(0, _printf_sp);
            }
        #endif 
    }		
		
		    #ifdef __cplusplus
    }
#endif 
