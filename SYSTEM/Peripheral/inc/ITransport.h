#pragma once

#include "Type.h"
#include "Buffer.h"

class ITransport;
class SerialPortOld;
#define NULL 0

//回调函数定义,当有数据到达时触发
typedef uint(*TransportHandler)(ITransport *transport, Buffer &bs, void *param); //临时不能用

class ITransport
{
    public:
        void Open();
        //打开端口
        bool HasHandler();
        uint Read(byte *buf, uint len);
        uint Write(byte *buf, uint len);
        int OnReceive(byte *buf, uint len);
        void SendData(byte data, uint times = 3000);
        void SendBuffer(byte *buff, int length =  - 1); //发送数据
        void SendBuffer(char *buff, int length =  - 1); //发送数据
        void Register(TransportHandler handler, void *param = NULL); // 注册数据到达事件
        bool Opened; //是否打开
    protected:
        virtual bool OnOpen();
        virtual bool OnWrite(const byte *buf, uint size);
		virtual uint OnRead(byte *buf, uint size);
        TransportHandler pHandler;

};
