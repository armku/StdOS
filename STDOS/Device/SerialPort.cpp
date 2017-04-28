#include "Sys.h"
#include <stdio.h>
#include "stm32f10x.h"
#include "SerialPort.h"
#include "TInterrupt.h"
#include "TTime.h"

#define COM_DEBUG 0
#define STM32F1XX

#define UARTS {USART1,USART2,USART3,UART4,UART5}

SerialPort::SerialPort()
{
    Init();
}

SerialPort::SerialPort(COM index, int baudRate)
{
    this->Index = index;
    if (baudRate <= 0)
    {
        this->_baudRate = 115200;
    }
    else
    {
        this->_baudRate = baudRate;
    }
}

// ����ʱ�Զ��ر�
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
    //    _index = 0xFF;
    RS485 = NULL;
    Error = 0;

    Remap = 0;
}

bool SerialPort::OnWrite(const Buffer &bs)
{
    return false;
}

#define UART_PINS {PA9,PA10,PA2,PA3,PB10,PB11,PC10,PC11,PC12,PD3}
#define UART_PINS_FULLREMAP {PA9,PA10,PA2,PA3,PB10,PB11,PC10,PC11,PC12,PD3}   //��Ҫ����
// ��ȡ����
void GetPins(Pin *txPin, Pin *rxPin, COM index, bool Remap = false)
{

    *rxPin =  *txPin = P0;

    const Pin g_Uart_Pins[] = UART_PINS;
    const Pin g_Uart_Pins_Map[] = UART_PINS_FULLREMAP;
    const Pin *p = g_Uart_Pins;
    if (Remap)
    {
        p = g_Uart_Pins_Map;
    }
    int n = index << 1;
    *txPin = p[n];
    *rxPin = p[n + 1];
}

// �򿪴���
bool SerialPort::OnOpen()
{
    Pin rx, tx;


    if (COM_NONE == Sys.MessagePort)
    {
        return false;
    }
    GetPins(&tx, &rx, this->Index);

    //    debug_printf("Serial%d Open(%d, %d, %d, %d)\r\n", _index + 1, _baudRate, _parity, _dataBits, _stopBits);
    #if COM_DEBUG        

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

        // �п����Ǵ򿪴�������Ժ�������
        if (Opened)
            return true;

    #endif 

    USART_InitTypeDef p;

    //�������ų�ʼ��
    AlternatePort txx;
    InputPort rxx;
    txx.Set(tx);
    rxx.Set(rx);

    // ��Ҫ�ص��Կڣ����򱭾�
    //    if (_index != Sys.MessagePort)
    //        USART_DeInit(_port);
    // USART_DeInit��ʵ���ǹر�ʱ�ӣ������е���һ�١���Ϊ�˰�ȫ���������ʹ��

    // �����ӳ��
    #ifdef STM32F1XX
        if (Remap)
        {
            switch (this->Index)
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

    // �� UART ʱ�ӡ������ȴ򿪴���ʱ�ӣ�����������
    #ifdef STM32F0XX
        switch (this->Index)
        {
            case COM1:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
                break; //����ʱ��
            case COM2:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
                break;
            default:
                break;
        }
    #else 
        if (this->Index)
        {
            // COM2-5 on APB1
            RCC->APB1ENR |= RCC_APB1ENR_USART2EN >> 1 << this->Index;
        }
        else
        {
            // COM1 on APB2
            RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
            //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //�˴��빦��ͬ����
        }
    #endif 

    #ifdef STM32F0
        GPIO_PinAFConfig(_GROUP(tx), _PIN(tx), GPIO_AF_1); //��IO��ӳ��ΪUSART�ӿ�
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
    USART_TypeDef *const g_Uart_Ports[] = UARTS;
    USART_Init(g_Uart_Ports[this->Index], &p);

    USART_ITConfig(g_Uart_Ports[this->Index], USART_IT_RXNE, ENABLE); // ���ڽ����ж�����
    // ��ʼ����ʱ���ر������жϣ����ﲻ��Ҫ�����رշ����ж�
    //USART_ITConfig(_port, USART_IT_TXE, DISABLE); // ����Ҫ�����ж�

    USART_Cmd(g_Uart_Ports[this->Index], ENABLE); //ʹ�ܴ���

    if (RS485)
    {
        *RS485 = false;
    }

    //Opened = true;    
    #if COM_DEBUG
        if (_index == Sys.MessagePort)
        {
            // ��ǰ����Ϊ�Ѵ򿪶˿ڣ�ShowLog������Ҫ�ж�
            Opened = true;
            goto ShowLog;
        }
    #endif 

    return true;
}

// �رն˿�
void SerialPort::OnClose()
{
    //    debug_printf("~Serial%d Close\r\n", _index + 1);

    //    Pin tx, rx;

    //    GetPins(&tx, &rx);

    //    USART_DeInit(_port);

    // �����ӳ��
    #ifdef STM32F1XX
        if (Remap)
        {
            //            switch (_index)
            //            {
            //                case 0:
            //                    AFIO->MAPR &= ~AFIO_MAPR_USART1_REMAP;
            //                    break;
            //                case 1:
            //                    AFIO->MAPR &= ~AFIO_MAPR_USART2_REMAP;
            //                    break;
            //                case 2:
            //                    AFIO->MAPR &= ~AFIO_MAPR_USART3_REMAP_FULLREMAP;
            //                    break;
            //            }
        }
    #endif 
}

// ���͵�һ�ֽ�����
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
            while (USART_GetFlagStatus(g_Uart_Ports[3], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
        case COM5:
            while (USART_GetFlagStatus(g_Uart_Ports[4], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
        default:
            //while (USART_GetFlagStatus(g_Uart_Ports[0], USART_FLAG_TXE) == RESET && --times > 0){}
            break;
    }
    //�ȴ��������
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
                USART_SendData(g_Uart_Ports[3], (ushort)data);
                break;
            case COM5:
                USART_SendData(g_Uart_Ports[4], (ushort)data);
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

void SerialPort::ChangePower(int level){

}

// ��ĳ���˿�д�����ݡ����sizeΪ0�����data�����ַ�����һֱ����ֱ������\0Ϊֹ
//bool SerialPort::OnWrite(const byte *buf, uint size)
//{
//    if (RS485)
//    {
//        *RS485 = true;
//    }
//    if (size > 0)
//    {
//        for (int i = 0; i < size; i++)
//        {
//            SendData(*buf++);
//        }
//    }
//    else
//    {
//        while (*buf)
//        {
//            SendData(*buf++);
//        }
//    }

//    if (RS485)
//    {
//        Sys.Delay(200);
//        *RS485 = false;
//    }
//    return true;
//}


// ��ĳ���˿ڶ�ȡ����
//uint SerialPort::OnRead(byte *buf, uint size)
//{
//    // ��100ms�ڽ�������
//    uint usTimeout = 100;
//    UInt64 us = Time.Current() + usTimeout;

//    uint count = 0; // �յ����ֽ���    
//    while (count < size && Time.Current() < us)
//    {
//        // ��ѯ���ռĴ������յ���������뻺����
//        if (USART_GetFlagStatus(_port, USART_FLAG_RXNE) != RESET)
//        {
//            *buf++ = (byte)USART_ReceiveData(_port);
//            count++;
//            us = Time.Current() + usTimeout;
//        }
//    }
//    return count;
//}
uint SerialPort::OnRead(Buffer &bs)
{
    return 0;
}

// ˢ��ĳ���˿��е�����
bool SerialPort::Flush(int times)
{
    //uint times = 3000;
    //    while (USART_GetFlagStatus(_port, USART_FLAG_TXE) == RESET && --times > 0)
    //        ;
    //�ȴ��������
    return times > 0;
}

#ifdef STM32F10X_HD
    #define UART_IRQs {USART1_IRQn,USART2_IRQn,USART3_IRQn,UART4_IRQn,UART5_IRQn}
#else 
    #define UART_IRQs {USART1_IRQn,USART2_IRQn,USART3_IRQn}
#endif 
void OnUsartReceive(ushort num, void *param);
void SerialPort::Register(TransportHandler handler, void *param)
{
    ITransport::Register(handler, param);

    const byte irqs[] = UART_IRQs;
    byte irq = irqs[this->Index];
    if (handler)
    {
        Interrupt.SetPriority(irq, 1);
        Interrupt.Activate(irq, OnUsartReceive, this);
    }
    else
    {
        //        Interrupt.Deactivate(irq);
    }
}

// �����Ĵ����жϺ���
void OnUsartReceive(ushort num, void *param)
{
    SerialPort *sp = (SerialPort*)param;
	USART_TypeDef *const g_Uart_Ports[] = UARTS;

    if (sp && sp->HasHandler())
    {
        if (USART_GetITStatus(g_Uart_Ports[sp->Index], USART_IT_RXNE) != RESET)
        {
            // ��ջ���䣬��ʡ�ڴ�
            byte buf[512];
            uint len = sp->Read(buf, sizeof(buf));
            if (len)
            {
                len = sp->OnReceive(buf, len);
//                #if 0
//                    assert_param(len <= ArrayLength(buf));
//                #endif 
//                // ��������ݣ�������ȥ
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

SerialPort *_printf_sp;
bool isInFPutc; //���ڴ������
extern "C"
{
    /* ����fputc�������û�����ʹ��printf���� */
    int fputc(int ch, FILE *f)
    {
        //        if (!Sys.Inited)
        //            return ch;

        if (Sys.MessagePort == COM_NONE)
            return ch;

        if (isInFPutc)
            return ch;
        isInFPutc = true;

        // ��鲢�򿪴���
        //if ((port->CR1 &USART_CR1_UE) != USART_CR1_UE && _printf_sp == NULL)
        if (_printf_sp == NULL)
        {
            _printf_sp = new SerialPort(COM(Sys.MessagePort));
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
        if (Sys.MessagePort == COM_NONE)
        {
            return NULL;
        }
        _printf_sp = new SerialPort(COM(Sys.MessagePort));
        _printf_sp->Open();
    }

    return _printf_sp;
}
