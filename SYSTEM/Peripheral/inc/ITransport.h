#pragma once

#include "Type.h"
#include "Buffer.h"

class ITransport;
class SerialPortOld;

//回调函数定义,当有数据到达时触发
typedef uint (*IOnUsartRead)(ITransport* transport,Buffer& bs,void* param);


typedef uint (*TransportHandler)(ITransport* transport,Buffer& bs,void* param); //临时不能用

class ITransport
{
	public:
	void SendData(byte data);		
	void SendBuffer(byte *buff,int length=-1);//发送数据
	void SendBuffer(char *buff,int length=-1);//发送数据
	void Register(IOnUsartRead handler,SerialPortOld *sp); // 注册数据到达事件
};
