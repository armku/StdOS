#include "ITransport.h"
#include "string.h"
// ע�����ݵ����¼�
void ITransport::Register(IOnUsartRead handler,void *param)
{
	
}
//��������
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

//��������
void ITransport::SendBuffer(char *buff, int length)
{
    this->SendBuffer((byte*)buff, length);
}
