#include "TInterrupt.h"
#include "TTime.h"
#include "SerialPort.h"
#include "Task.h"

SerialPort::SerialPort()
{
    Init();
}

SerialPort::SerialPort(COM index, int baudRate)
{
    this->Set(index,baudRate);    
}
void SerialPort::OnInit()
{
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
    if (this->_baudRate <= 0)
    {
        //		int clock = Sys.Clock;
        //		if(clock > 72)
        //			_baudRate = 2048000;
        //		else if (clock > 36)
        //			_baudRate = 1024000;
        //		else
        this->_baudRate = 256000;
    }
    if (this->_baudRate <= 9600)
        this->ByteTime = 1000 / (this->_baudRate / 10) + 1;
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
	this->_baudRate=baudRate;
}

void SerialPort::ChangePower(int level)
{
	if (level)
    this->Close();
}

SerialPort *printf_sp;
bool isInFPutc;//���ڴ������
static char com1rx[1024],com1tx[1024];
uint OnUsart1Read123(ITransport *transport, Buffer &bs, void *para, void *para2)
{
	//��ʱ�����
	return 0;
}
SerialPort *SerialPort::GetMessagePort()
{
	if (printf_sp && Sys.MessagePort != printf_sp->Index)
	{
		if ( printf_sp )
			printf_sp->Close();
		printf_sp=nullptr;
	}
	if ( !printf_sp )
	{
		if(Sys.MessagePort==COM_NONE)		
			return nullptr;
		if ( isInFPutc )
			return nullptr;
		isInFPutc = true;
		
		printf_sp=new SerialPort((COM)Sys.MessagePort);
		printf_sp->Tx.SetBuf(com1tx,ArrayLength(com1tx));
		printf_sp->Rx.SetBuf(com1rx,ArrayLength(com1rx));
		printf_sp->Register(OnUsart1Read123);
		printf_sp->Open();
		
		isInFPutc=false;
	}
	
    return printf_sp;
}

// �رն˿�
void SerialPort::OnClose()
{
    
}
// ��ĳ���˿�д�����ݡ����sizeΪ0�����data�����ַ�����һֱ����ֱ������\0Ϊֹ
bool SerialPort::OnWrite(const Buffer& bs)
{
	bool ret;
	
	if(bs.Length())
	{	
		TInterrupt::GlobalDisable();
		this->Tx.Write(bs);
		TInterrupt::GlobalEnable();
		this->Set485(true);
		this->OnWrite2();
		//this->Set485(false);//���
		ret=true;
	}
	else
	{
		ret=false;
	}
	
    return ret;
}

void SerialPort::Set485(bool flag)
{
    if (this->RS485)
    {       
        this->RS485->Write(flag);
    }
}

void SerialPort::ReceiveTask()
{
	char buf[512];
	Buffer bs(buf,ArrayLength(buf));
	this->Rx.Read(bs);
	this->OnReceive(bs,this); 
	((Task*)(this->_task))->Set(false,0);
}

static char *itoa(int value, char *string, int radix)
{
    int i, d;
    int flag = 0;
    char *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *=  - 1;

    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d *i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;
}
#include <stdarg.h>
//ģ��printf
void SerialPort::Printf(char *Data, ...)
{
	const char *s;
    int d;
    char buf[16];

    char bufSend[200];
    int bufSendPos;
    //int bufSendMax = ArrayLength(bufSend);

    bufSendPos = 0;

    va_list ap;
    va_start(ap, Data);

    while (*Data != 0)
    // �ж��Ƿ񵽴��ַ���������
    {
        if (*Data == 0x5c)
        //'\'
        {
            switch (*++Data)
            {
                case 'r':
                    //�س���
                    bufSend[bufSendPos++] = 0X0D;
                    Data++;
                    break;

                case 'n':
                    //���з�
                    bufSend[bufSendPos++] = 0X0A;
                    Data++;
                    break;

                default:
                    Data++;
                    break;
            }
        }

        else if (*Data == '%')
        {
            //
            switch (*++Data)
            {
                case 's':
                    //�ַ���
                    s = va_arg(ap, const char*);
                    for (;  *s; s++)
                    {
                        bufSend[bufSendPos++] =  *s;
                    }
                    Data++;
                    break;
                case 'd':
                    //ʮ����
                    d = va_arg(ap, int);
                    itoa(d, buf, 10);
                    for (s = buf;  *s; s++)
                    {
                        bufSend[bufSendPos++] =  *s;
                    }
                    Data++;
                    break;
                default:
                    Data++;
                    break;
            }
        }
        else
            bufSend[bufSendPos++] =  *Data++;
    }
	Buffer bs(bufSend,bufSendPos);
	this->Write(bs);
}
#include "TInterrupt.h"
#include "TTime.h"
#include "SerialPort.h"
#include "Task.h"

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
	((Task*)(this->_task))->Set(true, 0);
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
	this->Index = COM_NONE;
	RS485 = NULL;
	Error = 0;

	Remap = 0;
	this->OnInit();
}

void SerialPort::OnOpen2()
{

}

void SerialPort::OnClose2()
{

}

void SerialPort::OnHandler(ushort num, void* param)
{

}

