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
//关闭中断
void TInterrupt::Deactivate(byte irq)
{
	
}
extern SerialPort *_printf_sp;
#ifdef __cplusplus
    extern "C"
    {
    #endif 
//串口1中断服务程序
    //注意,读取USARTx->SR能避免莫名其妙的错误
    /*
     */
    void USART1_IRQHandler(void) //串口1中断服务程序
    {
        
			if(onSerialPortRcv[0])
			{
				SerialPort::OnUsartReceive(0, onSerialPortRcv[0]);
			}
    }	
void USART2_IRQHandler(void) //串口1中断服务程序
    {		
		if(onSerialPortRcv[1])
			{
				SerialPort::OnUsartReceive(1, onSerialPortRcv[1]);
			}
    }
    void USART3_IRQHandler(void) //串口1中断服务程序
    {		
		if(onSerialPortRcv[2])
			{
				SerialPort::OnUsartReceive(2, onSerialPortRcv[2]);
			}
    }
    void USART4_IRQHandler(void) //串口1中断服务程序
    {
        if(onSerialPortRcv[3])
			{
				SerialPort::OnUsartReceive(3, onSerialPortRcv[3]);
			}
    }
    void USART5_IRQHandler(void) //串口1中断服务程序
    {
        if(onSerialPortRcv[4])
			{
				SerialPort::OnUsartReceive(4, onSerialPortRcv[4]);
			}
    }	
		
		    #ifdef __cplusplus
    }
#endif 
