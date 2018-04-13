#include "ITransport.h"

// 初始化
ITransport::ITransport()
{
	this->Opening=false;
	this->Opened=false;
	this->_handler=nullptr;
}
// 打开传输口
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

// 关闭传输口
void ITransport::Close()
{
	this->Opened = false;
}

// 发送数据
bool ITransport::Write(const Buffer &bs)
{
    return this->OnWrite(bs);
}

bool ITransport::Write(const Buffer &bs, const void *opt)
{
    return this->OnWrite(bs);
}

// 接收数据
uint32_t ITransport::Read(Buffer &bs)
{
    return this->OnRead(bs);
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

uint32_t ITransport::OnRead(Buffer &bs)
{
    return bs.Length();
}

// 引发数据到达事件
uint32_t ITransport::OnReceive(Buffer &bs, void *param)
{
    if (this->_handler)
    {

        this->_handler(this, bs, this, this);
    }
    return bs.Length();
}
