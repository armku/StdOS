#include "ITransport.h"

// ��ʼ��
ITransport::ITransport(){}
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
    return 0;
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
    return 0;
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
