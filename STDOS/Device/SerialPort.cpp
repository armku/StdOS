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

/* ͨ��ͬ��/�첽�շ���(USART)��� ------------------------------------------------------------------*/
#if defined(STM32F1)
	#define UARTS {USART1, USART2, USART3, UART4, UART5}
#elif defined STM32F4
	#define UARTS {USART1, USART2, USART3, UART4, UART5,USART6}
#elif defined STM32F0
	#define UARTS {USART1, USART2, USART3}
#endif
#define UART_PINS {\
/* TX   RX   CTS  RTS */	\
PA9, PA10,PA11,PA12,/* USART1 */	\
PA2, PA3, PA0, PA1, /* USART2 */	\
PB10,PB11,PB13,PB14,/* USART3 */	\
PC10,PC11,P0,  P0,  /* UART4  */	\
PC12, PD2,P0,  P0,  /* UART5  */	\
}
#define UART_PINS_FULLREMAP {\
/* TX   RX   CTS  RTS */	\
PB6, PB7, PA11,PA12,/* USART1 AFIO_MAPR_USART1_REMAP */	\
PD5, PD6, PD3, PD4, /* USART2 AFIO_MAPR_USART2_REMAP */	\
PD8, PD9, PD11,PD12,/* USART3 AFIO_MAPR_USART3_REMAP_FULLREMAP */	\
PC10,PC11,P0,  P0,  /* UART4  */	\
PC12, PD2,P0,  P0,  /* UART5  */	\
}

SerialPort::SerialPort()
{
    Init();
}

SerialPort::SerialPort(COM index, int baudRate)
{
    this->Set(index,baudRate);    
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
void SerialPort::Set(COM index, int baudRate)
{
	Index	= index;
	_baudRate	= baudRate;
	
	// ����ϵͳʱ���Զ�����Ĭ�ϲ�����
	if(_baudRate <= 0)
	{
//		int clock = Sys.Clock;
//		if(clock > 72)
//			_baudRate = 2048000;
//		else if (clock > 36)
//			_baudRate = 1024000;
//		else
			_baudRate = 115200;
	}
}
void SerialPort::Set(byte dataBits, byte parity, byte stopBits)
{
	
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
// ˢ��ĳ���˿��е�����
bool SerialPort::Flush(int times)
{
    //uint times = 3000;
    //    while (USART_GetFlagStatus(_port, USART_FLAG_TXE) == RESET && --times > 0)
    //        ;
    //�ȴ��������
    return times > 0;
}
void SerialPort::SetBaudRate(int baudRate)
{
	this->_baudRate=baudRate;
	this->OnOpen();
}
#ifdef STM32F0
	#define UART_IRQs {USART1_IRQn,USART2_IRQn}
#elif defined STM32F1
    #define UART_IRQs {USART1_IRQn,USART2_IRQn,USART3_IRQn,UART4_IRQn,UART5_IRQn}
#elif defined STM32F4 
    #define UART_IRQs {USART1_IRQn,USART2_IRQn,USART3_IRQn,UART4_IRQn,UART5_IRQn,USART6_IRQn}
#endif 
// �����Ĵ����жϺ���
void OnUsartReceive(ushort num, void *param)
{
    SerialPort *sp = (SerialPort*)param;
	USART_TypeDef *const g_Uart_Ports[] = UARTS;

    //if (sp && sp->HasHandler())
	if (sp)
    {
        if (USART_GetITStatus(g_Uart_Ports[sp->Index], USART_IT_RXNE) != RESET)
        {
            // ��ջ���䣬��ʡ�ڴ�
            byte buf[512];
			Buffer bs(buf,512);
            uint len = sp->Read(bs);
            if (len)
            {				
                //len = sp->OnReceive(bs,param);
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
}
void SerialPort::ChangePower(int level)
{

}
void SerialPort::OnTxHandler()
{
	
}
void SerialPort::OnRxHandler()
{
	
}
SerialPort *_printf_sp;
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
#ifdef DEBUG
	void SerialPort::Test()
	{
		
	}
#endif
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
#if defined(STM32F0) || defined(STM32F4)
	#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#endif
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
			#ifdef STM32F0
            #elif defined STM32F1
                case USART_StopBits_0_5:
                    debug_printf(", StopBits_0_5");
                    break;
            #elif defined STM32F4
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
	#ifdef STM32F0
		AlternatePort rxx;
	#elif defined STM32F1
		InputPort rxx;
	#elif defined STM32F4
		AlternatePort rxx;
	#endif
    txx.Set(tx);
    rxx.Set(rx);

    // ��Ҫ�ص��Կڣ����򱭾�
    //    if (_index != Sys.MessagePort)
    //        USART_DeInit(_port);
    // USART_DeInit��ʵ���ǹر�ʱ�ӣ������е���һ�١���Ϊ�˰�ȫ���������ʹ��

    // �����ӳ��
	#ifdef STM32F0
    #elif defined STM32F1
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
    #elif defined STM32F4
	#endif 

    // �� UART ʱ�ӡ������ȴ򿪴���ʱ�ӣ�����������
    #if defined(STM32F1) || defined(STM32F4) 
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
	#elif defined STM32F0
		switch (this->Index)
        {
            case COM1:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
                break; //����ʱ��
            case COM2:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
                break;
            case COM3:
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
    #endif 

    #ifdef STM32F0
        GPIO_PinAFConfig(_GROUP(tx), _PIN(tx), GPIO_AF_1); //��IO��ӳ��ΪUSART�ӿ�
        GPIO_PinAFConfig(_GROUP(rx), _PIN(rx), GPIO_AF_1);
    #elif defined STM32F1
	#elif defined STM32F4
        const byte afs[] = 
        {
            GPIO_AF_USART1, GPIO_AF_USART2, GPIO_AF_USART3, GPIO_AF_UART4, GPIO_AF_UART5, GPIO_AF_USART6, GPIO_AF_UART7, GPIO_AF_UART8
        };
		GPIO_PinAFConfig(_GROUP(tx),_PIN(tx),afs[Index]); //GPIOA9����ΪUSART1
		GPIO_PinAFConfig(_GROUP(rx),_PIN(rx),afs[Index]); //GPIOA10����ΪUSART1
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
    #ifdef DEBUG
        if (this->Index == Sys.MessagePort)
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
	#ifdef STM32F0
    #elif defined STM32F1
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
	#elif defined STM32F4
    #endif 
}
// ��ĳ���˿�д�����ݡ����sizeΪ0�����data�����ַ�����һֱ����ֱ������\0Ϊֹ
bool SerialPort::OnWrite(const Buffer& bs)
{
    if (RS485)
    {
        *RS485 = true;
    }

	for(int i=0;i<bs.Length();i++)
	{
		this->SendData(bs[i]);
	}

    if (RS485)
    {
        Sys.Delay(200);
        *RS485 = false;
    }
    return true;
}

//���ݶ�ȡ�¼�
uint SerialPort::OnRead(Buffer &bs)
{
	USART_TypeDef *const g_Uart_Ports[] = UARTS;
    // ��2ms�ڽ�������
    uint usTimeout = 2;
    UInt64 us = Time.Current() + usTimeout;

    uint count = 0; // �յ����ֽ���    
    while (count < this->MaxSize && Time.Current() < us)
    {
        // ��ѯ���ռĴ������յ���������뻺����
        if (USART_GetFlagStatus(g_Uart_Ports[this->Index], USART_FLAG_RXNE) != RESET)
        {
            bs[count] = (byte)USART_ReceiveData(g_Uart_Ports[this->Index]);			
            count++;   
			us = Time.Current() + usTimeout;
        }
    }
	bs.SetLength(count);
	this->OnReceive(bs,this);
    return count;
}
void SerialPort::OnHandler(ushort num, void* param)
{
	
}
void SerialPort::Set485(bool flag)
{
	
}
void SerialPort::ReceiveTask()
{
	
}
void SerialPort::Init()
{
    this->Index=COM_NONE;
    RS485 = NULL;
    Error = 0;

    Remap = 0;
}
bool SerialPort::OnSet()
{
	return false;
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


//���Դ���
/*
ISO-V2:PB5����485����
ISO-V3:PC2����485����
 */
