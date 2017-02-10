#include "ITransport.h"
#include "string.h"

void ITransport::Open()
{
    this->OnOpen();
}

// 注册数据到达事件
void ITransport::Register(TransportHandler handler, void *param){

}
bool ITransport::HasHandler()
{
    return pHandler;
}

//发送数据
uint ITransport::Read(byte *buf, uint len)
{
	return this->OnRead(buf,len);
}

uint ITransport::Write(byte *buf, uint len)
{
    return len;
}

int ITransport::OnReceive(byte *buf, uint len)
{
    return len;
}

void ITransport::SendData(byte data, uint times){}
bool ITransport::OnOpen()
{
    return true;
}

bool ITransport::OnWrite(const byte *buf, uint size)
{
    return true;
}
uint ITransport::OnRead(byte *buf, uint size)
{
	return 0;
}
void ITransport::SendBuffer(byte *buff, int length)
{
    if (length < 0)
    {
        length = strlen((char*)buff);
        //        while (*buff)
        //        {
        //            this->SendData(*buff);
        //            buff++;
        //        }
    }
    //    else
    //    {
    //        for (int i = 0; i < length; i++)
    //        {
    //            this->SendData(buff[i]);
    //        }
    //    }

    this->OnWrite(buff, length);
}

//发送数据
void ITransport::SendBuffer(char *buff, int length)
{
    this->SendBuffer((byte*)buff, length);
}
