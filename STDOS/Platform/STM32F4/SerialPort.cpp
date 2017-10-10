#include "TInterrupt.h"
#include "TTime.h"
#include "SerialPort.h"
#include "Task.h"
#include "stm32f4xx.h"

	#define UART_PINS {\
    /* TX   RX   CTS  RTS */	\
    PA9, PA10,PA11,PA12,/* USART1 */	\
    PD5, PD6, PD7, PD4, /* USART2 */	\
    PD8,PD9,PB13,PB14,/* USART3 */	\
    PC10,PC11,P0,  P0,  /* UART4  */	\
    PC12, PD2,P0,  P0,  /* UART5  */	\
	PC6, PC7,PG7,  PG8,  /* USART6  */	\
    }
    #define UART_PINS_FULLREMAP {\
    /* TX   RX   CTS  RTS */	\
    PB6, PB7, PA11,PA12,/* USART1 AFIO_MAPR_USART1_REMAP */	\
    PD5, PD6, PD3, PD4, /* USART2 AFIO_MAPR_USART2_REMAP */	\
    PD8, PD9, PD11,PD12,/* USART3 AFIO_MAPR_USART3_REMAP_FULLREMAP */	\
    PC10,PC11,P0,  P0,  /* UART4  */	\
    PC12, PD2,P0,  P0,  /* UART5  */	\
    }
// 获取引脚
void SerialPort_GetPins(Pin *txPin, Pin *rxPin, COM index, bool Remap = false)
{

    *rxPin =  *txPin = P0;

    const Pin g_Uart_Pins[] = UART_PINS;
    const Pin g_Uart_Pins_Map[] = UART_PINS_FULLREMAP;
    const Pin *p = g_Uart_Pins;
    if (Remap)
    {
        p = g_Uart_Pins_Map;
    }
    int n = index << 2;
    *txPin = p[n];
    *rxPin = p[n + 1];
}
int SerialPort_Opening(int a1)
{
	return 0;
}
int SerialPort_Closeing(int result)
{
	return 0;
}

/////////////////////////////////////////////
//////////////////////以下为移动/////////////
/* 通用同步/异步收发器(USART)针脚 ------------------------------------------------------------------*/
	#define UARTS {USART1, USART2, USART3, UART4, UART5,USART6}

    #define UART_IRQs {USART1_IRQn,USART2_IRQn,USART3_IRQn,UART4_IRQn,UART5_IRQn,USART6_IRQn}
// 真正的串口中断函数
void OnUsartReceive(ushort num, void *param)
{
    SerialPort *sp = (SerialPort*)param;
	USART_TypeDef *const g_Uart_Ports[] = UARTS;

    //if (sp && sp->HasHandler())
	if (sp)
    {
        if (USART_GetITStatus(g_Uart_Ports[sp->Index], USART_IT_RXNE) != RESET)
        {
            // 从栈分配，节省内存
            byte buf[512];
			Buffer bs(buf,ArrayLength(buf));
            uint len = sp->Read(bs);
            if (len)
            {				
                //len = sp->OnReceive(bs,param);
//                #if 0
//                    assert_param(len <= ArrayLength(buf));
//                #endif 
//                // 如果有数据，则反馈回去
//                #if 0
//                    if (len)
//                    {
//                        sp->Write(buf, len);
//                    }
//                #endif 
            }
        }
    }
}
void SerialPort::Register(TransportHandler handler, void *param)
{
    ITransport::Register(handler, param);

    const byte irqs[] = UART_IRQs;
    byte irq = irqs[this->Index];
    if (handler)
    {
        Interrupt.SetPriority(irq,1);
        Interrupt.Activate(irq, OnUsartReceive, this);
    }
    else
    {
        Interrupt.Deactivate(irq);
    }
	this->_taskidRx= Sys.AddTask(&SerialPort::ReceiveTask,this,-1,-1,"serialrcv");
	this->_task=Task::Get(this->_taskidRx);
}

	#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))

// 打开串口
bool SerialPort::OnOpen()
{    
    if (COM_NONE == Sys.MessagePort)
    {
        return false;
    }
    SerialPort_GetPins(&this->Pins[0], &this->Pins[1], this->Index);
    //    debug_printf("Serial%d Open(%d, %d, %d, %d)\r\n", _index + 1, _baudRate, _parity, _dataBits, _stopBits);
    #ifdef DEBUG        

        ShowLog: debug_printf("Serial%d Open(%d", this->Index + 1, _baudRate);
        switch (_parity)
        {
            case USART_Parity_No:
                debug_printf(", Parity_None");
                break;
            case USART_Parity_Even:
                debug_printf(", Parity_Even");
                break;
            case USART_Parity_Odd:
                debug_printf(", Parity_Odd");
                break;
        }
        switch (_dataBits)
        {
            case USART_WordLength_8b:
                debug_printf(", WordLength_8b");
                break;
            case USART_WordLength_9b:
                debug_printf(", WordLength_9b");
                break;
        }
        switch (_stopBits)
        {
			case USART_StopBits_1:
                debug_printf(", StopBits_1");
                break;
            case USART_StopBits_1_5:
                debug_printf(", StopBits_1_5");
                break;
            case USART_StopBits_2:
                debug_printf(", StopBits_2");
                break;
        }
        debug_printf(") TX=P%c%d RX=P%c%d\r\n", _PIN_NAME(this->Pins[0]), _PIN_NAME(this->Pins[1]));

        // 有可能是打开串口完成以后跳回来
        if (Opened)
            return true;

    #endif 

    USART_InitTypeDef p;

    //串口引脚初始化
    this->Ports[0]=new AlternatePort();
	this->Ports[1]=new AlternatePort();
	this->Ports[0]->Set(this->Pins[0]);
    this->Ports[1]->Set(this->Pins[1]);
	this->Ports[0]->Open();
	this->Ports[1]->Open();

    // 不要关调试口，否则杯具
    //    if (_index != Sys.MessagePort)
    //        USART_DeInit(_port);
    // USART_DeInit其实就是关闭时钟，这里有点多此一举。但为了安全起见，还是使用

    // 检查重映射
	
    // 打开 UART 时钟。必须先打开串口时钟，才配置引脚
    	switch(this->Index)
		{
				case COM1:
				case COM6:
					RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
					break;
				case COM2:
				case COM3:
				case COM4:
				case COM5:
					RCC->APB1ENR |= (RCC_APB1ENR_USART2EN >> 1) << this->Index;
					break;
				default:
					break;
		}
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //此代码功能同上行
	    const byte afs[] = 
        {
            GPIO_AF_USART1, GPIO_AF_USART2, GPIO_AF_USART3, GPIO_AF_UART4, GPIO_AF_UART5, GPIO_AF_USART6, GPIO_AF_UART7, GPIO_AF_UART8
        };
		GPIO_PinAFConfig(_GROUP(this->Pins[0]),_PIN(this->Pins[0]),afs[Index]); //GPIOA9复用为USART1
		GPIO_PinAFConfig(_GROUP(this->Pins[1]),_PIN(this->Pins[1]),afs[Index]); //GPIOA10复用为USART1
    
    USART_StructInit(&p);
    p.USART_BaudRate = _baudRate;
    p.USART_WordLength = _dataBits;
    p.USART_StopBits = _stopBits;
    p.USART_Parity = _parity;
    USART_TypeDef *const g_Uart_Ports[] = UARTS;
    USART_Init(g_Uart_Ports[this->Index], &p);

    USART_ITConfig(g_Uart_Ports[this->Index], USART_IT_RXNE, ENABLE); // 串口接收中断配置
    // 初始化的时候会关闭所有中断，这里不需要单独关闭发送中断
    //USART_ITConfig(_port, USART_IT_TXE, DISABLE); // 不需要发送中断

    USART_Cmd(g_Uart_Ports[this->Index], ENABLE); //使能串口

    if (RS485)
    {
        *RS485 = false;
    }

    //Opened = true;    
    #ifdef DEBUG
        if (this->Index == Sys.MessagePort)
        {
            // 提前设置为已打开端口，ShowLog里面需要判断
            Opened = true;
            goto ShowLog;
        }
    #endif 

    return true;
}
//数据读取事件
uint SerialPort::OnRead(Buffer &bs)
{
    USART_TypeDef *const g_Uart_Ports[] = UARTS;
    this->OnRxHandler();
    // 轮询接收寄存器，收到数据则放入缓冲区
    if (USART_GetFlagStatus(g_Uart_Ports[this->Index], USART_FLAG_RXNE) != RESET)
    {
        this->Rx.Enqueue((byte)USART_ReceiveData(g_Uart_Ports[this->Index]));
    }
    return bs.Length();
}
