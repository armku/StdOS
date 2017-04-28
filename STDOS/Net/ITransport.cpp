#include "ITransport.h"
#include "string.h"
#include "Array.h"




// ��ʼ��
ITransport::ITransport()
	{
	}
// ��������ȷ���ر�
	ITransport::~ITransport()
	{
	}
// �򿪴����
bool ITransport::Open()
{
    return this->OnOpen();
}
// �رմ����
	void ITransport::Close()
	{
	}

// ��������
	bool ITransport::Write(const Buffer& bs)
	{
		return false;
	}
	bool ITransport::Write(const Buffer& bs, const void* opt)
	{
		return false;
	}
	// ��������
	uint ITransport::Read(Buffer& bs)
	{
		return 0;
	}

// ע��ص�����
	void ITransport::Register(TransportHandler handler, void* param)
	{
	this->pHandler=handler;
}
bool ITransport::OnWrite(const Buffer& bs)
{
	return false;
}

bool ITransport::OnWriteEx(const Buffer& bs, const void* opt)
{
	return false;
}
uint ITransport::OnRead(Buffer& bs)
{
	return 0;
}

// �������ݵ����¼�
	uint ITransport::OnReceive(Buffer& bs, void* param)
	{
		
	}













//��������
uint ITransport::Read(byte *buf, uint len)
{
	return this->OnRead(buf,len);
}

uint ITransport::Write(byte *buf, uint len)
{
	return this->OnWrite(buf,len);
}

int ITransport::OnReceive(byte *buf, uint len)
{
	if(this->pHandler)
	{
		Buffer buffer(buf,len);
		this->pHandler(this,buffer,this,this);
	}
    return len;
}

void ITransport::SendData(byte data, uint times){}

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
        length = ArrayLength(buff);        
    }    

    this->OnWrite(buff, length);
}

//��������
void ITransport::SendBuffer(char *buff, int length)
{
    this->SendBuffer((byte*)buff, length);
}
