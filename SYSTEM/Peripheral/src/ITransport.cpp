#include "ITransport.h"
#include "string.h"
// 注册数据到达事件
void ITransport::Register(IOnUsartRead handler,void *param)
{
	
}
//发送数据
void ITransport::SendBuffer(byte *buff, int length)
{ 
    if (length < 0)
    {
		length=strlen((char*)buff);
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
	
	this->OnWrite(buff,length);
}

//发送数据
void ITransport::SendBuffer(char *buff, int length)
{
    this->SendBuffer((byte*)buff, length);
}
