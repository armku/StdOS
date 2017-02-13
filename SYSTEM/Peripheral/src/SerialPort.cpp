#include "Sys.h"
#include <stdio.h>

#include "Port.h"
#include "SerialPort.h"
#include "FIFORing.h"
#include "TInterrupt.h"

#define COM_DEBUG 0
#define STM32F1XX

#define UARTS {USART1,USART2,USART3,UART4,UART5}

int ArrayLength(const USART_TypeDef *const buf[])
{
    return 5;
}

SerialPort::SerialPort()
{
    Init();
}


SerialPort::SerialPort(USART_TypeDef *com, int baudRate, byte parity, byte dataBits, byte stopBits)
{
    assert_param(com);
    const USART_TypeDef *const g_Uart_Ports[] = UARTS;
    byte _index = 0xFF;

    for (int i = 0; i < ArrayLength(g_Uart_Ports); i++)
    {
        if (g_Uart_Ports[i] == com)
        {
            _index = i;
            break;
        }
    }
    Init();
    Init(_index, baudRate, parity, dataBits, stopBits);
}

// 析构时自动关闭
SerialPort::~SerialPort()
{
    if (RS485)
    {
        delete RS485;
    }
    RS485 = NULL;
}

void SerialPort::Init()
{
    _index = 0xFF;
    RS485 = NULL;
    Error = 0;

    IsRemap = false;
}


void SerialPort::Init(byte index, int baudRate, byte parity, byte dataBits, byte stopBits)
{

    USART_TypeDef *const g_Uart_Ports[] = UARTS;
    _index = index;
    assert_param(_index < ArrayLength(g_Uart_Ports));

    _port = g_Uart_Ports[_index];
    _baudRate = baudRate;
    _parity = parity;
    _dataBits = dataBits;
    _stopBits = stopBits;
    // 根据端口实际情况决定打开状态
    if (_port->CR1 &USART_CR1_UE)
        Opened = true;
    // 设置名称
    //Name = "COMx";
    *(uint*)Name = *(uint*)"COMx";
    Name[3] = '0' + _index + 1;
    Name[4] = 0;
}

#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
// 打开串口
bool SerialPort::OnOpen()
{
    Pin rx, tx;
    GetPins(&tx, &rx);

    debug_printf("Serial%d Open(%d, %d, %d, %d)\r\n", _index + 1, _baudRate, _parity, _dataBits, _stopBits);
    #if COM_DEBUG
        if (_index != Sys.MessagePort)
        {
            ShowLog: debug_printf("Serial%d Open(%d", _index + 1, _baudRate);
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
                #ifdef STM32F10X
                    case USART_StopBits_0_5:
                        debug_printf(", StopBits_0_5");
                        break;
                    #endif 
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
            debug_printf(") TX=P%c%d RX=P%c%d\r\n", _PIN_NAME(tx), _PIN_NAME(rx));

            // 有可能是打开串口完成以后跳回来
            if (Opened)
                return true;
        }
    #endif 

    USART_InitTypeDef p;

    //串口引脚初始化
    _tx.Set(tx);
    #if defined(STM32F0) || defined(STM32F4)
        _rx.Set(rx);
    #else 
        _rx.Set(rx);
    #endif 

    // 不要关调试口，否则杯具
    if (_index != Sys.MessagePort)
        USART_DeInit(_port);
    // USART_DeInit其实就是关闭时钟，这里有点多此一举。但为了安全起见，还是使用

    // 检查重映射
    #ifdef STM32F1XX
        if (IsRemap)
        {
            switch (_index)
            {
                case 0:
                    AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
                    break;
                case 1:
                    AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;
                    break;
                case 2:
                    AFIO->MAPR |= AFIO_MAPR_USART3_REMAP_FULLREMAP;
                    break;
            }
        }
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    #endif 

    // 打开 UART 时钟。必须先打开串口时钟，才配置引脚
    #ifdef STM32F0XX
        switch (_index)
        {
            case COM1:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
                break; //开启时钟
            case COM2:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
                break;
            default:
                break;
        }
    #else 
        if (_index)
        {
            // COM2-5 on APB1
            RCC->APB1ENR |= RCC_APB1ENR_USART2EN >> 1 << _index;
        }
        else
        {
            // COM1 on APB2
            RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        }
    #endif 

    #ifdef STM32F0
        GPIO_PinAFConfig(_GROUP(tx), _PIN(tx), GPIO_AF_1); //将IO口映射为USART接口
        GPIO_PinAFConfig(_GROUP(rx), _PIN(rx), GPIO_AF_1);
    #elif defined(STM32F4)
        const byte afs[] = 
        {
            GPIO_AF_USART1, GPIO_AF_USART2, GPIO_AF_USART3, GPIO_AF_UART4, GPIO_AF_UART5, GPIO_AF_USART6, GPIO_AF_UART7, GPIO_AF_UART8
        };
        GPIO_PinAFConfig(_GROUP(tx), _PIN(tx), afs[_index]);
        GPIO_PinAFConfig(_GROUP(rx), _PIN(rx), afs[_index]);
    #endif 

    USART_StructInit(&p);
    p.USART_BaudRate = _baudRate;
    p.USART_WordLength = _dataBits;
    p.USART_StopBits = _stopBits;
    p.USART_Parity = _parity;
    USART_Init(_port, &p);

    USART_ITConfig(_port, USART_IT_RXNE, ENABLE); // 串口接收中断配置
    // 初始化的时候会关闭所有中断，这里不需要单独关闭发送中断
    //USART_ITConfig(_port, USART_IT_TXE, DISABLE); // 不需要发送中断

    USART_Cmd(_port, ENABLE); //使能串口

    if (RS485)
        *RS485 = false;

    //Opened = true;
    this->AddInterrupt();
    #if COM_DEBUG
        if (_index == Sys.MessagePort)
        {
            // 提前设置为已打开端口，ShowLog里面需要判断
            Opened = true;
            goto ShowLog;
        }
    #endif 

    return true;
}

// 关闭端口
void SerialPort::OnClose()
{
    debug_printf("~Serial%d Close\r\n", _index + 1);

    Pin tx, rx;

    GetPins(&tx, &rx);

    USART_DeInit(_port);

    // 检查重映射
    #ifdef STM32F1XX
        if (IsRemap)
        {
            switch (_index)
            {
                case 0:
                    AFIO->MAPR &= ~AFIO_MAPR_USART1_REMAP;
                    break;
                case 1:
                    AFIO->MAPR &= ~AFIO_MAPR_USART2_REMAP;
                    break;
                case 2:
                    AFIO->MAPR &= ~AFIO_MAPR_USART3_REMAP_FULLREMAP;
                    break;
            }
        }
    #endif 
}

// 发送单一字节数据
void SerialPort::SendData(byte data, uint times)
{
    while (USART_GetFlagStatus(_port, USART_FLAG_TXE) == RESET && --times > 0){}

    //等待发送完毕
    if (times > 0)
    {
        USART_SendData(_port, (ushort)data);
    }
    else
    {
        Error++;
    }
}

// 向某个端口写入数据。如果size为0，则把data当作字符串，一直发送直到遇到\0为止
bool SerialPort::OnWrite(const byte *buf, uint size)
{
    if (RS485)
    {
        *RS485 = true;
    }
    if (size > 0)
    {
        for (int i = 0; i < size; i++)
        {
            SendData(*buf++);
        }
    }
    else
    {
        while (*buf)
        {
            SendData(*buf++);
        }
    }

    if (RS485)
    {
        *RS485 = false;
    }
    return true;
}


// 从某个端口读取数据
uint SerialPort::OnRead(byte *buf, uint size)
{
    // 在100ms内接收数据
    #if 0
        uint msTimeout = 1;
        ulong us = Time.Current() + msTimeout * 1000;
    #else 
        uint msTimeout = 20;
        ulong us = Time.Current() + msTimeout;
    #endif 
    uint count = 0; // 收到的字节数    
    while (count < size && Time.Current() < us)
    {
        // 轮询接收寄存器，收到数据则放入缓冲区
        if (USART_GetFlagStatus(_port, USART_FLAG_RXNE) != RESET)
        {			
            *buf++ = (byte)USART_ReceiveData(_port);
            count++;
            #if 0
                us = Time.Current() + msTimeout * 1000;
            #else 
                us = Time.Current() + msTimeout;
            #endif 
        }
    }
    return count;
}

// 刷出某个端口中的数据
bool SerialPort::Flush(uint times)
{
    //uint times = 3000;
    while (USART_GetFlagStatus(_port, USART_FLAG_TXE) == RESET && --times > 0)
        ;
    //等待发送完毕
    return times > 0;
}

#define UART_IRQs {0,1,2,3,4,5}

void SerialPort::Register(TransportHandler handler, void *param)
{
    ITransport::Register(handler, param);

    const byte irqs[] = UART_IRQs;
    byte irq = irqs[_index];
    if (handler)
    {
        Interrupt.SetPriority(irq, 1);
        #if 1
            Interrupt.Activate(irq, OnUsartReceive, this);
        #endif 
    }
    else
    {
        Interrupt.Deactivate(irq);
    }
}

// 真正的串口中断函数
void SerialPort::OnUsartReceive(ushort num, void *param)
{

    SerialPort *sp = (SerialPort*)param;

    if (sp && sp->HasHandler())
    {
        if (USART_GetITStatus(sp->_port, USART_IT_RXNE) != RESET)
        {			
            // 从栈分配，节省内存
            byte buf[64];
            uint len = sp->Read(buf, sizeof(buf));
			if (len)
            {
				len = sp->OnReceive(buf, len);
                #if 0
                assert_param(len <= ArrayLength(buf));
				#endif
                // 如果有数据，则反馈回去
				#if 0
                if (len)
                {
                    sp->Write(buf, len);
                }
				#endif
            }			
        }
    }

}

#define UART_PINS {PA9,PA10,PA2,PA3,PB10,PB11,PC10,PC11,PC12,PD3}
#define UART_PINS_FULLREMAP {PA9,PA10,PA2,PA3,PB10,PB11,PC10,PC11,PC12,PD3}   //需要整理

// 获取引脚
void SerialPort::GetPins(Pin *txPin, Pin *rxPin)
{

    *rxPin =  *txPin = P0;

    const Pin g_Uart_Pins[] = UART_PINS;
    const Pin g_Uart_Pins_Map[] = UART_PINS_FULLREMAP;
    const Pin *p = g_Uart_Pins;
    if (IsRemap)
    {
        p = g_Uart_Pins_Map;
    }
    int n = _index << 1;
    *txPin = p[n];
    *rxPin = p[n + 1];
}

SerialPort *_printf_sp;
bool isInFPutc;
extern "C"
{
    /* 重载fputc可以让用户程序使用printf函数 */
    int fputc(int ch, FILE *f)
    {
        if (!Sys.Inited)
            return ch;

        int _index = Sys.MessagePort;
        if (_index == COM_NONE)
            return ch;

        USART_TypeDef *g_Uart_Ports[] = UARTS;
        USART_TypeDef *port = g_Uart_Ports[_index];

        if (isInFPutc)
            return ch;
        isInFPutc = true;
        // 检查并打开串口
        if ((port->CR1 &USART_CR1_UE) != USART_CR1_UE && _printf_sp == NULL)
        {
            _printf_sp = new SerialPort(port);
            _printf_sp->Open();
        }

        _printf_sp->SendData((byte)ch);

        isInFPutc = false;
        return ch;
    }
}

SerialPort *SerialPort::GetMessagePort()
{
    if (!_printf_sp)
    {
        int _index = Sys.MessagePort;
        if (_index == COM_NONE)
        {
            return NULL;
        }
        USART_TypeDef *g_Uart_Ports[] = UARTS;
        USART_TypeDef *port = g_Uart_Ports[_index];

        _printf_sp = new SerialPort(port);
        _printf_sp->Open();
    }

    return _printf_sp;
}

///////////////////////////////////////////////////////////
//old
SerialPortOld::SerialPortOld(COM_Def index, int baudRate, byte parity, byte dataBits, byte stopBits)
{
    this->_index = index;
    this->_baudRate = baudRate;
    this->_parity = parity;
    this->_dataBits = dataBits;
    this->_stopBits = stopBits;

    this->Name[0] = 'C';
    this->Name[1] = 'O';
    this->Name[2] = 'M';
    this->Name[3] = '1' + index;
    this->Name[4] = 0;

    USART_InitTypeDef usart;
    NVIC_InitTypeDef nvic;

    usart.USART_BaudRate = this->_baudRate;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_Parity = this->_parity;
    usart.USART_StopBits = this->_stopBits;
    usart.USART_WordLength = this->_dataBits;

    nvic.NVIC_IRQChannelCmd = ENABLE;
    rx.Floating = true;
    //初始化端口引脚
    switch (this->_index)
    {
        case COM1:
            tx.Set(PA9);
            rx.Set(PA10);
            break;
        case COM2:
            tx.Set(PA2);
            rx.Set(PA3);
            break;
        case COM3:
            tx.Set(PB10);
            rx.Set(PB11);
            break;
        case COM4:
            tx.Set(PC10);
            rx.Set(PC11);
            break;
        case COM5:
            tx.Set(PC12);
            rx.Set(PD3);
            break;
        default:
            break;
    }

    switch (this->_index)
    {
        case COM1:
            //串口1初始化
            //初始化时钟信号
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
            //初始化USART
            USART_Init(USART1, &usart);
            USART_Cmd(USART1, ENABLE);
            USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
            //初始化NVIC
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART1_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 0;
            nvic.NVIC_IRQChannelSubPriority = 0;
            NVIC_Init(&nvic);
            break;
        case COM2:
            //串口2初始化
            //初始化时钟信号
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
            //初始化USART            
            USART_Init(USART2, &usart);
            USART_Cmd(USART2, ENABLE);
            USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
            //初始化NVIC
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART2_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 0;
            nvic.NVIC_IRQChannelSubPriority = 1;
            NVIC_Init(&nvic);
            break;
        case COM3:
            //串口3初始化
            //初始化时钟信号   
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
            //初始化USART            
            USART_Init(USART3, &usart);
            USART_Cmd(USART3, ENABLE);
            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
            //初始化NVIC
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART3_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 0;
            nvic.NVIC_IRQChannelSubPriority = 2;
            NVIC_Init(&nvic);
            break;
            #if 0
            case COM4:
                //串口4初始化
                //初始化时钟信号   
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
                //初始化USART            
                USART_Init(UART4, &usart);
                USART_Cmd(UART4, ENABLE);
                USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
                //初始化NVIC
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannel = UART4_IRQn;
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 3;
                NVIC_Init(&nvic);
                break;
            case COM5:
                //串口5初始化
                //初始化时钟信号   
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
                //初始化USART            
                USART_Init(UART5, &usart);
                USART_Cmd(UART5, ENABLE);
                USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
                //初始化NVIC
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannel = UART5_IRQn;
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 2;
                NVIC_Init(&nvic);
                break;
            #endif 
        default:
            break;
    }

}

void SerialPort::AddInterrupt()
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannelCmd = ENABLE;

    switch (this->_index)
    {
        case COM1:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART1_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = 1;
            break;
        case COM2:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART2_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = 1;
            break;
        case COM3:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART3_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 1;
            nvic.NVIC_IRQChannelSubPriority = 2;
            break;
            #if 0
            case COM4:
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannel = UART4_IRQn;
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = 3;
                break;
            case COM5:
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannel = UART5_IRQn;
                nvic.NVIC_IRQChannelPreemptionPriority = 1;
                nvic.NVIC_IRQChannelSubPriority = 2;
                NVIC_Init(&nvic);
                break;
            #endif 
        default:
            break;
    }
    NVIC_Init(&nvic);
}

void SerialPortOld::Open(){

}

CFIFORing com1buf; //串口1接收缓冲区
uint com1timeidle; //串口1空闲时间

CFIFORing com2buf; //串口2接收缓冲区
uint com2timeidle; //串口2空闲时间

CFIFORing com3buf; //串口3接收缓冲区
uint com3timeidle; //串口3空闲时间
#ifdef __cplusplus
    extern "C"
    {
    #endif 

    

    void USART2_IRQHandler(void) //串口1中断服务程序
    {
        if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
        {
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
            byte inch = USART_ReceiveData(USART2); //读取接收到的数据

            com2buf.Push(inch);
            com2timeidle = 0; //空闲计时器清零
        }
    }
    void USART3_IRQHandler(void) //串口1中断服务程序
    {
        if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
        {
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
            byte inch = USART_ReceiveData(USART3); //读取接收到的数据
            printf("com3 in data \r\n");
            com3buf.Push(inch);
            com3timeidle = 0; //空闲计时器清零
        }
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


    //向串口1发送数据
    void sendToUsart1(byte ch)
    {
        #if 0
            while ((USART1->SR &0X40) == 0){}
            //循环发送,直到发送完毕
            USART1->DR = (byte)ch;
        #else 
            USART_SendData(USART1, ch);

            /* 等待发送完毕 */
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
        #endif 
    }

    //向串口2发送数据
    void sendToUsart2(byte ch)
    {
        USART_SendData(USART2, ch);

        /* 等待发送完毕 */
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}
    }

    //向串口3发送数据
    void sendToUsart3(byte ch)
    {
        USART_SendData(USART3, ch);

        /* 等待发送完毕 */
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
    }

    //向串口4发送数据
    void sendToUsart4(byte ch)
    {
        USART_SendData(UART4, ch);

        /* 等待发送完毕 */
        while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET){}
    }

    //向串口5发送数据
    void sendToUsart5(byte ch)
    {
        USART_SendData(UART5, ch);

        /* 等待发送完毕 */
        while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET){}
    }
    #ifdef __cplusplus
    }
#endif 
void SerialPortOld::SendData(byte data, uint times)
{
    if (this->RS485)
    {
        *this->RS485 = true;
    }
    switch (this->_index)
    {
        case COM1:
            sendToUsart1(data);
            break;
        case COM2:
            sendToUsart2(data);
            break;
        case COM3:
            sendToUsart3(data);
            break;
        case COM4:
            sendToUsart4(data);
            break;
        case COM5:
            sendToUsart5(data);
            break;
        default:
            break;
    }
    if (this->RS485)
    {
        //Sys.Delay(100); //延时，解决最后一个字节没有发出问题
        *this->RS485 = false;
    }
}

// 注册数据到达事件
void SerialPortOld::Register(TransportHandler handler, SerialPortOld *sp)
{
    this->OnRcv = handler;
}

byte buftmp[40];
//从串口收到数据
void SerialPortOld::OnUsartReceive(byte *buf, ushort length)
{
    Buffer bs(buftmp, length);
    for (int i = 0; i < length; i++)
    {
        buftmp[i] = buf[i];
    }
    if (this->OnRcv)
    {
        this->OnRcv(this, bs, this);
    }
}
