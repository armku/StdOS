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

// 析构时自动关闭
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
	
    // 根据系统时钟自动计算默认波特率
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

// 刷出某个端口中的数据
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

SerialPort *printf_sp;
bool isInFPutc;//正在串口输出
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
		printf_sp->Tx.SetCapacity(512);
		printf_sp->Open();
		
		isInFPutc=false;
	}
	
    return printf_sp;
}

// 关闭端口
void SerialPort::OnClose()
{
    
}
// 向某个端口写入数据。如果size为0，则把data当作字符串，一直发送直到遇到\0为止
bool SerialPort::OnWrite(const Buffer& bs)
{
	bool ret;
	int i=0;
	
	if(bs.Length())
	{	
		this->Tx.Write(bs);
		this->Set485(true);
		this->OnWrite2();
		this->Set485(false);//添加
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
        if (!flag)
            Sys.Sleep(1);
        this->RS485->Write(flag);
        if (flag)
            Sys.Sleep(1);
    }
}

void SerialPort::ReceiveTask()
{
	char buf[512];
	Buffer bs(buf,ArrayLength(buf));
	this->Rx.Read(bs);
	this->OnReceive(bs,this); 
	((Task*)(this->_task))->Set(false,20);
}

void SerialPort::ReceiveTask2()
{
	((Task*)(this->_task))->Set(true,20);	
}
//空闲中断收到数据接收完成。
void SerialPort::ReceiveTask3()
{
	((Task*)(this->_task))->Set(true,1);	
}
