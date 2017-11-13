#include "TInterrupt.h"
#include "TTime.h"
#include "SerialPort.h"
#include "Task.h"
#include "stm32f0xx.h"

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
// ��ȡ����
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
//////////////////////����Ϊ�ƶ�/////////////
/* ͨ��ͬ��/�첽�շ���(USART)��� ------------------------------------------------------------------*/
	#define UARTS {USART1, USART2, USART3}
	#define UART_IRQs {USART1_IRQn,USART2_IRQn}
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
			byte ch=USART_ReceiveData(g_Uart_Ports[sp->Index]);
			if(sp->Index<10)
			{
				sp->Rx.Enqueue(ch);
			}
			sp->ReceiveTask2();
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

// �򿪴���
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

        // �п����Ǵ򿪴�������Ժ�������
        if (Opened)
            return true;

    #endif 

    USART_InitTypeDef p;

    //�������ų�ʼ��
    this->Ports[0]=new AlternatePort();
	this->Ports[1]=new AlternatePort();
	this->Ports[0]->Set(this->Pins[0]);
    this->Ports[1]->Set(this->Pins[1]);
	this->Ports[0]->Open();
	this->Ports[1]->Open();

    // ��Ҫ�ص��Կڣ����򱭾�
    //    if (_index != Sys.MessagePort)
    //        USART_DeInit(_port);
    // USART_DeInit��ʵ���ǹر�ʱ�ӣ������е���һ�١���Ϊ�˰�ȫ���������ʹ��

    // �����ӳ��
	
    // �� UART ʱ�ӡ������ȴ򿪴���ʱ�ӣ�����������    
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

        GPIO_PinAFConfig(_GROUP(this->Pins[0]), _PIN(this->Pins[0]), GPIO_AF_1); //��IO��ӳ��ΪUSART�ӿ�
        GPIO_PinAFConfig(_GROUP(this->Pins[1]), _PIN(this->Pins[1]), GPIO_AF_1);

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
//���ݶ�ȡ�¼�
uint SerialPort::OnRead(Buffer &bs)
{
    USART_TypeDef *const g_Uart_Ports[] = UARTS;
    this->OnRxHandler();
    // ��ѯ���ռĴ������յ���������뻺����
    if (USART_GetFlagStatus(g_Uart_Ports[this->Index], USART_FLAG_RXNE) != RESET)
    {
        this->Rx.Enqueue((byte)USART_ReceiveData(g_Uart_Ports[this->Index]));
    }
    return bs.Length();
}
// ���͵�һ�ֽ�����
int SerialPort::SendData(byte data, int times)
{
	if(this->_baudRate<9600)
	{
		times=30000;
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
		#if defined STM32F4
        case COM6:
			#if defined(STM32F1) || defined(STM32F4)
				while (USART_GetFlagStatus(g_Uart_Ports[5], (USART_FLAG_TXE) == RESET) && --times > 0){}
			#elif defined STM32F0
			#endif
			break;
		case COM7:
			break;
		case COM8:
			break;
		#endif
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
			#if defined STM32F4
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
			#endif
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
