#include "ITransport.h"
// ע�����ݵ����¼�
void ITransport::Register(IOnUsartRead handler,void *param)
{
	
}
//��������
void ITransport::SendBuffer(byte *buff, int length)
{ 
    if (length < 0)
    {
        while (*buff)
        {
            this->SendData(*buff);
            buff++;
        }
    }
    else
    {
        for (int i = 0; i < length; i++)
        {
            this->SendData(buff[i]);
        }
    }
}

//��������
void ITransport::SendBuffer(char *buff, int length)
{
    this->SendBuffer((byte*)buff, length);
}
