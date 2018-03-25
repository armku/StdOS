#include "TInterrupt.h"
#include "TTime.h"
#include "SerialPort.h"
#include "Task.h"
//#include "_Core.h"
#include "Platform\stm32.h"

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

// 真正的串口中断函数
void OnUsartReceive(uint16_t num, void *param)
{
    SerialPort *sp = (SerialPort*)param;
    USART_TypeDef *const g_Uart_Ports[] = UARTS;
	volatile byte ch;

    //if (sp && sp->HasHandler())
    if (sp)
    {
        if (USART_GetITStatus(g_Uart_Ports[sp->Index], USART_IT_RXNE) != RESET)
        {
            ch = USART_ReceiveData(g_Uart_Ports[sp->Index]);
            sp->Rx.Enqueue(ch);			
        }
		if (USART_GetITStatus(g_Uart_Ports[sp->Index], USART_IT_IDLE) == SET)
        //数据帧接收完毕
        {
            ch = USART_ReceiveData(g_Uart_Ports[sp->Index]); //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)    
			sp->OnRxHandler();			
        }
		/* 处理发送缓冲区空中断 */
		if (USART_GetITStatus(g_Uart_Ports[sp->Index], USART_IT_TXE) != RESET)
		{
			if (sp->Tx.Empty())
			{
				/* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
				USART_ITConfig(g_Uart_Ports[sp->Index], USART_IT_TXE, DISABLE);
				/* 使能数据发送完毕中断 */
				USART_ITConfig(g_Uart_Ports[sp->Index], USART_IT_TC, ENABLE);
			}
			else
			{
				/* 从发送FIFO取1个字节写入串口发送数据寄存器 */
				USART_SendData(g_Uart_Ports[sp->Index], sp->Tx.Dequeue());
			}

		}
		/* 数据bit位全部发送完毕的中断 */
		else if (USART_GetITStatus(g_Uart_Ports[sp->Index], USART_IT_TC) != RESET)
		{
			if (sp->Tx.Empty())
			{
				/* 如果发送FIFO的数据全部发送完毕，禁止数据发送完毕中断 */
				USART_ITConfig(g_Uart_Ports[sp->Index], USART_IT_TC, DISABLE);

				/* 回调函数, 一般用来处理RS485通信，将RS485芯片设置为接收模式，避免抢占总线 */
				sp->Tx.Clear();
				if(sp->RS485)
				{
					*sp->RS485=0;
				}
			}
			else
			{
				/* 正常情况下，不会进入此分支 */
				/* 如果发送FIFO的数据还未完毕，则从发送FIFO取1个数据写入发送数据寄存器 */
				USART_SendData(g_Uart_Ports[sp->Index], sp->Tx.Dequeue());
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
        Interrupt.SetPriority(irq, 0);
        Interrupt.Activate(irq, OnUsartReceive, this);
    }
    this->_taskidRx = Sys.AddTask(&SerialPort::ReceiveTask, this,  - 1,  - 1, "serialrcv");
    this->_task = Task::Get(this->_taskidRx);
}

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
            case USART_StopBits_0_5:
                debug_printf(", StopBits_0_5");
                break;
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
    this->Ports[0] = new AlternatePort();
    this->Ports[1] = new InputPort();
    this->Ports[0]->Set(this->Pins[0]);
    this->Ports[1]->Set(this->Pins[1]);
    this->Ports[0]->Open();
    this->Ports[1]->Open();

    // 不要关调试口，否则杯具
    //    if (_index != Sys.MessagePort)
    //        USART_DeInit(_port);
    // USART_DeInit其实就是关闭时钟，这里有点多此一举。但为了安全起见，还是使用

    // 检查重映射
    if (Remap)
    {
        switch (this->Index)
        {
            case COM1:
                AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
                break;
            case COM2:
                AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;
                break;
            case COM3:
                AFIO->MAPR |= AFIO_MAPR_USART3_REMAP_FULLREMAP;
                break;
            case COM4:
                break;
            case COM5:
                break;
            case COM6:
                break;
            case COM7:
                break;
            case COM8:
                break;
            default:
                break;
        }
    }
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 打开 UART 时钟。必须先打开串口时钟，才配置引脚
    switch (this->Index)
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

    USART_StructInit(&p);
    p.USART_BaudRate = _baudRate;
/*
#define USART_WordLength_8b                  ((uint16_t)0x0000)
#define USART_WordLength_9b                  ((uint16_t)0x1000)
*/
	switch (this->_dataBits)
	{
	case 8:
		p.USART_WordLength = USART_WordLength_8b;
		break;
	case 9:
		p.USART_WordLength = USART_WordLength_9b;
		break;
	default:
		p.USART_WordLength = USART_WordLength_8b;
		break;
	}
/*
#define USART_StopBits_1                     ((uint16_t)0x0000)
#define USART_StopBits_0_5                   ((uint16_t)0x1000)
#define USART_StopBits_2                     ((uint16_t)0x2000)
#define USART_StopBits_1_5                   ((uint16_t)0x3000)
*/
	switch (this->_stopBits)
	{
	case 0:
		p.USART_StopBits = USART_StopBits_1;
		break;
	case 1:
		p.USART_StopBits = USART_StopBits_1;
		break;
	case 2:
		p.USART_StopBits = USART_StopBits_2;
		break;
	default:
		p.USART_StopBits = USART_StopBits_1;
		break;
	}
/*
#define USART_Parity_No                      ((uint16_t)0x0000)
#define USART_Parity_Even                    ((uint16_t)0x0400)
#define USART_Parity_Odd                     ((uint16_t)0x0600)
*/
	switch (this->_parity)
	{
	case 0:
		p.USART_Parity = USART_Parity_No;
		break;
	default:
		p.USART_Parity = USART_Parity_No;
		break;
	}
    USART_TypeDef *const g_Uart_Ports[] = UARTS;
    USART_Init(g_Uart_Ports[this->Index], &p);

    USART_ITConfig(g_Uart_Ports[this->Index], USART_IT_RXNE, ENABLE); // 串口接收中断配置
	USART_ITConfig(g_Uart_Ports[this->Index], USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 
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
uint32_t SerialPort::OnRead(Buffer &bs)
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

// 发送单一字节数据
int SerialPort::SendData(byte data, int times)
{
    if (this->_baudRate < 9600)
    {
        times = 30000;
    }
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
            while (USART_GetFlagStatus(g_Uart_Ports[3], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
        case COM5:
            while (USART_GetFlagStatus(g_Uart_Ports[4], USART_FLAG_TXE) == RESET && --times > 0){}
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
                USART_SendData(g_Uart_Ports[0], (uint16_t)data);
                break;
            case COM2:
                USART_SendData(g_Uart_Ports[1], (uint16_t)data);
                break;
            case COM3:
                USART_SendData(g_Uart_Ports[2], (uint16_t)data);
                break;
            case COM4:
                USART_SendData(g_Uart_Ports[3], (uint16_t)data);
                break;
            case COM5:
                USART_SendData(g_Uart_Ports[4], (uint16_t)data);
                break;
            default:
                //USART_SendData(g_Uart_Ports[0], (uint16_t)data);
                break;
        }
    }
    else
    {
        Error++;
    }
    return 0;
}
//调用中断发送
void SerialPort::OnWrite2()
{		
	USART_TypeDef *const g_Uart_Ports[] = UARTS;	
	USART_ITConfig(g_Uart_Ports[this->Index], USART_IT_TXE, ENABLE);
}
