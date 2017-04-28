#include "ITransport.h"

// 初始化
ITransport::ITransport(){}
// 析构函数确保关闭
ITransport::~ITransport(){}
// 打开传输口
bool ITransport::Open()
{
    return this->OnOpen();
}

// 关闭传输口
void ITransport::Close(){}

// 发送数据
bool ITransport::Write(const Buffer &bs)
{
    return false;
}

bool ITransport::Write(const Buffer &bs, const void *opt)
{
    return false;
}

// 接收数据
uint ITransport::Read(Buffer &bs)
{
    return 0;
}

// 注册回调函数
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

// 引发数据到达事件
uint ITransport::OnReceive(Buffer &bs, void *param)
{
    if (this->_handler)
    {

        this->_handler(this, bs, this, this);
    }
    return bs.Length();
}
