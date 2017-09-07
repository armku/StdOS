#include "TInterrupt.h"
#include "TTime.h"
#include "SerialPort.h"

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
    this->Index = index;
    this->_baudRate = baudRate;

    // ����ϵͳʱ���Զ�����Ĭ�ϲ�����
    if (_baudRate <= 0)
    {
        //		int clock = Sys.Clock;
        //		if(clock > 72)
        //			_baudRate = 2048000;
        //		else if (clock > 36)
        //			_baudRate = 1024000;
        //		else
        _baudRate = 115200;
    }
    if (baudRate <= 9600)
        this->ByteTime = 1000 / (baudRate / 10) + 1;
    else
        this->ByteTime = 1;
    this->OnSet();
}
void SerialPort::Set(byte dataBits, byte parity, byte stopBits)
{
	this->_dataBits=dataBits;
	this->_parity=parity;
	this->_stopBits=stopBits;
}

// ˢ��ĳ���˿��е�����
bool SerialPort::Flush(int times)
{
  this->Set485(true);
  while ( (!this->Tx.Empty()) && times > 0 )
  {
    times = this->SendData(this->Tx.Dequeue(), times);
  }
  this->Set485(false);
  return times > 0;
}
void SerialPort::SetBaudRate(int baudRate)
{
	#if 0
	this->Set(this->Index,baudRate);
	#else
	this->_baudRate=baudRate;
	this->OnOpen();
	#endif	
}


void SerialPort::ChangePower(int level)
{
	if (level)
    this->Close();
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

// �رն˿�
void SerialPort::OnClose()
{
    //    debug_printf("~Serial%d Close\r\n", _index + 1);

    //    Pin tx, rx;

    //    SerialPort_GetPins(&tx, &rx);

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
	bool ret;
	int i=0;
	
	if(bs.Length())
	{		
		if(this->Tx.Capacity()==0)
		{
			for ( i = 64; bs.Length() > i && i < 1024; i *= 2 );
			this->Tx.SetCapacity(i);
		}
		this->Tx.Write(bs);
		this->Set485(true);
		this->OnWrite2();
		ret=true;
	}
	else
	{
		ret=false;
	}
	
	
	
	
	
	
	
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
    return ret;
}

void SerialPort::Set485(bool flag)
{
    if (this->RS485)
    {
        if (!flag)
            Sys.Sleep(1);
        this->RS485->Write(flag);
        if (flag)
            Sys.Sleep(1);
    }
}

void SerialPort::ReceiveTask()
{
	
}

//���Դ���
/*
ISO-V2:PB5����485����
ISO-V3:PC2����485����
 */
