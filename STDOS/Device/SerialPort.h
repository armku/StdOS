#ifndef __SerialPort_H__
#define __SerialPort_H__

#include "Core\Queue.h"
#include "Port.h"
#include "Net\ITransport.h"

// 串口类
class SerialPort : public ITransport
{
private:
	friend class ComProxy;
	int		_baudRate;
	byte	_dataBits;
	byte	_parity;
	byte	_stopBits;

	void Init();

public:
	char 		Name[5];	// 名称。COMxx，后面1字节\0表示结束
	byte		Remap;		// 重映射组
	OutputPort* RS485;		// RS485使能引脚
	int 		Error;		// 错误计数
	int			ByteTime;	// 字节间隔，最小1ms
	Pin			Pins[2];	// Tx/Rx
	Port*		Ports[2];	// Tx/Rx
	COM			Index;

	void*	State;

	// 收发缓冲区
	Queue	Tx;
	Queue	Rx;

	SerialPort();
	SerialPort(COM index, int baudRate = 0);

	// 析构时自动关闭
	virtual ~SerialPort();

	void Set(COM index, int baudRate = 0);
	void Set(byte dataBits, byte parity, byte stopBits);

	int SendData(byte data, int times = 3000);

	bool Flush(int times = 3000);

	void SetBaudRate(int baudRate = 0);

	virtual void Register(TransportHandler handler, void* param = nullptr);
	
	virtual String& ToStr(String& str) const { return str + Name; }

	void OnRxHandler();

	static SerialPort* GetMessagePort();
protected:
	virtual bool OnOpen();

	virtual bool OnWrite(const Buffer& bs);
	virtual uint32_t OnRead(Buffer& bs);

private:
	void Set485(bool flag);

	void*	_task;
	uint32_t	_taskidRx;
	void ReceiveTask();

	void OnWrite2();//调用发送中断
public:	
	void Printf(char *Data, ...);//模拟printf
};

#endif
