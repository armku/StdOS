#include "ITransport.h"

// ��ʼ��
ITransport::ITransport()
{
	this->Opening=false;
	this->Opened=false;
	this->_handler=nullptr;
}
// �򿪴����
bool ITransport::Open()
{
	if (!this->Opened)
		this->OnOpen();
	this->Opened = true;
	if (!this->Opened)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// �رմ����
void ITransport::Close()
{
	this->Opened = false;
}

// ��������
bool ITransport::Write(const Buffer &bs)
{
    return this->OnWrite(bs);
}

bool ITransport::Write(const Buffer &bs, const void *opt)
{
    return this->OnWrite(bs);
}

// ��������
uint32_t ITransport::Read(Buffer &bs)
{
    return this->OnRead(bs);
}

// ע��ص�����
void ITransport::Register(TransportHandler handler, void *param)
{
    this->_handler = handler;
}

bool ITransport::OnWrite(const Buffer &bs)
{
    return false;
}

bool ITransport::OnWriteEx(const Buffer &bs, const void *opt)
{
    return false;
}

uint32_t ITransport::OnRead(Buffer &bs)
{
    return bs.Length();
}

// �������ݵ����¼�
uint32_t ITransport::OnReceive(Buffer &bs, void *param)
{
    if (this->_handler)
    {

        this->_handler(this, bs, this, this);
    }
    return bs.Length();
}
