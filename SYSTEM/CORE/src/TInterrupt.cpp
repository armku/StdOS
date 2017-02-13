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
void USART2_IRQHandler(void) //串口1中断服务程序
    {
		#if 0
        if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
        {
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
            byte inch = USART_ReceiveData(USART2); //读取接收到的数据

//            com2buf.Push(inch);
//            com2timeidle = 0; //空闲计时器清零
        }
		#else
		if(onSerialPortRcv[1])
			{
				SerialPort::OnUsartReceive(1, onSerialPortRcv[1]);
			}
		#endif
    }
    void USART3_IRQHandler(void) //串口1中断服务程序
    {
		#if 0
        if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
        {
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
            byte inch = USART_ReceiveData(USART3); //读取接收到的数据
            printf("com3 in data \r\n");
//            com3buf.Push(inch);
//            com3timeidle = 0; //空闲计时器清零
        }
		#else
		if(onSerialPortRcv[2])
			{
				SerialPort::OnUsartReceive(2, onSerialPortRcv[2]);
			}
		#endif
    }
    void USART4_IRQHandler(void) //串口1中断服务程序
    {
        if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
        {
            USART_ClearITPendingBit(UART4, USART_IT_RXNE);
            byte inch = USART_ReceiveData(UART4); //读取接收到的数据
        }
    }
    void USART5_IRQHandler(void) //串口1中断服务程序
    {
        if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
        //接收中断(接收到的数据必须是0x0d 0x0a结尾)
        {
            USART_ClearITPendingBit(UART5, USART_IT_RXNE);
            byte inch = USART_ReceiveData(UART5); //读取接收到的数据
        }
    }	
		
		    #ifdef __cplusplus
    }
#endif 
