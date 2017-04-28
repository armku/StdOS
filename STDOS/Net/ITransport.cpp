#include "ITransport.h"

// ��ʼ��
ITransport::ITransport()
{
	this->Opening=false;
	this->Opened=false;
	this->MinSize=1;
	this->MaxSize=100;
	this->_handler=nullptr;
}
// ��������ȷ���ر�
ITransport::~ITransport(){}
// �򿪴����
bool ITransport::Open()
{
    return this->OnOpen();
}

// �رմ����
void ITransport::Close(){}

// ��������
bool ITransport::Write(const Buffer &bs)
{
    return false;
}

bool ITransport::Write(const Buffer &bs, const void *opt)
{
    return false;
}

// ��������
uint ITransport::Read(Buffer &bs)
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

uint ITransport::OnRead(Buffer &bs)
{
    return bs.Length();
}

// �������ݵ����¼�
uint ITransport::OnReceive(Buffer &bs, void *param)
{
    if (this->_handler)
    {

        this->_handler(this, bs, this, this);
    }
    return bs.Length();
}

PackPort::PackPort()
{
	this->Port=nullptr;
}
PackPort::~PackPort()
{
}
void PackPort::Set(ITransport* port)
{
}
bool PackPort::OnOpen()
{
	return false;
}
void PackPort::OnClose()
{
}
bool PackPort::OnWrite(const Buffer& bs)
{
	return false;
}
uint PackPort::OnRead(Buffer& bs)
{
	return 0;
}
uint PackPort::OnPortReceive(ITransport* sender, Buffer& bs, void* param, void* param2)
{
	return 0;
}
