#include "TInterrupt.h"
#include "SerialPort.h"

TInterrupt Interrupt;



SerialPort *onSerialPortRcv[5];

void TInterrupt::SetPriority(byte intp,byte level)
{
	
}
void TInterrupt::Activate(byte irq, OnUsartReceive onrcv, void * param)
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
        
			if(onSerialPortRcv[0])
			{
				SerialPort::OnUsartReceive(0, onSerialPortRcv[0]);
			}
    }	
void USART2_IRQHandler(void) //����1�жϷ������
    {		
		if(onSerialPortRcv[1])
			{
				SerialPort::OnUsartReceive(1, onSerialPortRcv[1]);
			}
    }
    void USART3_IRQHandler(void) //����1�жϷ������
    {		
		if(onSerialPortRcv[2])
			{
				SerialPort::OnUsartReceive(2, onSerialPortRcv[2]);
			}
    }
    void USART4_IRQHandler(void) //����1�жϷ������
    {
        if(onSerialPortRcv[3])
			{
				SerialPort::OnUsartReceive(3, onSerialPortRcv[3]);
			}
    }
    void USART5_IRQHandler(void) //����1�жϷ������
    {
        if(onSerialPortRcv[4])
			{
				SerialPort::OnUsartReceive(4, onSerialPortRcv[4]);
			}
    }	
		
		    #ifdef __cplusplus
    }
#endif 
