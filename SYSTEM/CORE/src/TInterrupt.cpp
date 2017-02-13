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
        #if 0
            if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
            //接收到一字节
            {

                USART_ClearITPendingBit(USART1, USART_IT_RXNE);
                byte inch = USART1->DR; //读取接收到的数据
                printf("COM1 in data \r\n");
                com1buf.Push(inch);
                com1timeidle = 0; //空闲计时器清零
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
