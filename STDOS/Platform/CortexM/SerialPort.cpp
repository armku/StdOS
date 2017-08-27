#include "TInterrupt.h"
#include "TTime.h"
#include "SerialPort.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif


/* 通用同步/异步收发器(USART)针脚 ------------------------------------------------------------------*/
#if defined(STM32F1)
	#define UARTS {USART1, USART2, USART3, UART4, UART5}
#elif defined STM32F4
	#define UARTS {USART1, USART2, USART3, UART4, UART5,USART6}
#elif defined STM32F0
	#define UARTS {USART1, USART2, USART3}
#endif

void SerialPort_Opening()
{
  ;
}
void SerialPort_Closeing()
{
  ;
}
void SerialPort::OnRxHandler()
{
	
}
void SerialPort::OnTxHandler()
{
	
}
bool SerialPort::OnSet()
{
	return false;
}
void SerialPort::Init()
{
    this->Index=COM_NONE;
    RS485 = NULL;
    Error = 0;

    Remap = 0;
}

void SerialPort::OnOpen2()
{

}

void SerialPort::OnClose2()
{

}
void SerialPort::OnWrite2()
{

}
// 发送单一字节数据
int SerialPort::SendData(byte data, int times)
{
    USART_TypeDef *const g_Uart_Ports[] = UARTS;
    switch (this->Index)
    {
        case COM1:
            while (USART_GetFlagStatus(g_Uart_Ports[0], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
        case COM2:
            while (USART_GetFlagStatus(g_Uart_Ports[1], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
        case COM3:
            while (USART_GetFlagStatus(g_Uart_Ports[2], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
        case COM4:
			#if defined(STM32F1) || defined(STM32F4)
				while (USART_GetFlagStatus(g_Uart_Ports[3], USART_FLAG_TXE) == RESET && --times > 0){}
			#elif defined STM32F0
			#endif
            break;
        case COM5:
			#if defined(STM32F1) || defined(STM32F4)
				while (USART_GetFlagStatus(g_Uart_Ports[4], USART_FLAG_TXE) == RESET && --times > 0){}
			#elif defined STM32F0
			#endif
            break;
        case COM6:
			#if defined(STM32F1) || defined(STM32F4)
				while (USART_GetFlagStatus(g_Uart_Ports[5], USART_FLAG_TXE) == RESET && --times > 0){}
			#elif defined STM32F0
			#endif
			break;
		case COM7:
			break;
		case COM8:
			break;
		default:
            //while (USART_GetFlagStatus(g_Uart_Ports[0], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
    }
    //等待发送完毕
    if (times > 0)
    {
        switch (this->Index)
        {
            case COM1:
                USART_SendData(g_Uart_Ports[0], (ushort)data);
                break;
            case COM2:
                USART_SendData(g_Uart_Ports[1], (ushort)data);
                break;
            case COM3:
                USART_SendData(g_Uart_Ports[2], (ushort)data);
                break;
            case COM4:
				#if defined(STM32F1) || defined(STM32F4)
					USART_SendData(g_Uart_Ports[3], (ushort)data);
				#elif defined STM32F0
				#endif
                break;
            case COM5:
				#if defined(STM32F1) || defined(STM32F4)
					USART_SendData(g_Uart_Ports[4], (ushort)data);
				#elif defined STM32F0
				#endif
                break;
            case COM6:
				#if defined(STM32F1) || defined(STM32F4)
					USART_SendData(g_Uart_Ports[5], (ushort)data);
				#elif defined STM32F0
				#endif
                break;
			case COM7:
				break;
			case COM8:
				break;
			default:
                //USART_SendData(g_Uart_Ports[0], (ushort)data);
                break;
        }
    }
    else
    {
        Error++;
    }
    return 0;
}
void SerialPort::OnHandler(ushort num, void* param)
{
	
}
