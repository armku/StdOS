#ifndef __ITransport_H__
#define __ITransport_H__

#include "Kernel\Sys.h"

class ITransport;

// 传输口数据到达委托。传入数据缓冲区地址和长度，如有反馈，仍使用该缓冲区，返回数据长度
typedef uint (*TransportHandler)(ITransport* port, Buffer& bs, void* param, void* param2);

// 帧数据传输接口
// 实现者确保数据以包的形式传输，屏蔽数据的粘包和拆包
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

#endif
