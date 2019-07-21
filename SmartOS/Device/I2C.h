﻿#ifndef _I2C_H_
#define _I2C_H_

#include "Device\Port.h"

//SCL		开漏复用输出
//SDA		开漏复用输出

// I2C外设
class I2C
{
public:
    int		Speed;		// 速度
    int		Retry;		// 等待重试次数，默认200
    int		Error;		// 错误次数

	ushort	Address;	// 设备地址。7位或10位
	byte	SubWidth;	// 子地址占字节数

	bool	Opened;		// 是否已经打开

	I2C();
	virtual ~I2C();

	virtual void SetPin(Pin scl, Pin sda) = 0;
	virtual void GetPin(Pin* scl = nullptr, Pin* sda = nullptr) = 0;

	virtual bool Open();		// 打开设备
	virtual void Close();		// 关闭设备

	virtual void Start() = 0;	// 开始会话
	virtual void Stop() = 0;	// 停止会话

	virtual void WriteByte(byte dat) = 0;	// 写入单个字节
	virtual byte ReadByte() = 0;			// 读取单个字节
	virtual void Ack(bool ack) = 0;			// 发送Ack/Nak
	virtual bool WaitAck(bool ack) = 0;		// 等待Ack/Nak

	// 新会话向指定地址写入
	bool Write(int addr, const Buffer& bs);
	bool Write(int addr, byte data);
	// 新会话从指定地址读取
	virtual uint Read(int addr, Buffer& bs);
	virtual byte Read(int addr);
	virtual ushort Read2(int addr);
	virtual uint Read4(int addr);

protected:
	virtual bool OnOpen() = 0;	// 打开设备
	virtual void OnClose() = 0;	// 外部设备

	virtual bool SendAddress(int addr, bool tx);
	virtual bool SendSubAddr(int addr);
};

// I2C会话类。初始化时打开，超出作用域析构时关闭
class I2CScope
{
private:
	I2C* _iic;

public:
	I2CScope(I2C* iic)
	{
		_iic = iic;
		_iic->Start();
	}

	~I2CScope()
	{
		_iic->Stop();
	}
};

// 硬件I2C
class HardI2C : public I2C
{
private:
	void Init(byte index, uint speedHz);

public:
	// 使用端口和最大速度初始化，因为需要分频，实际速度小于等于该速度
    //HardI2C(I2C_TypeDef* iic = I2C1, uint speedHz = 100000);
	HardI2C(byte index, uint speedHz = 100000);
    virtual ~HardI2C();

	virtual void SetPin(Pin scl, Pin sda);
	virtual void GetPin(Pin* scl = nullptr, Pin* sda = nullptr);

	virtual void Start();
	virtual void Stop();

	virtual void WriteByte(byte dat);
	virtual byte ReadByte();
	virtual void Ack(bool ack);
	virtual bool WaitAck(bool ack);

	//virtual bool Write(int addr, byte* buf, uint len);	// 新会话向指定地址写入多个字节
	//virtual uint Read(int addr, byte* buf, uint len);	// 新会话从指定地址读取多个字节

private:
    byte	_index;
	void*	_IIC;
	uint	_Event;

	AlternatePort SCL;
	AlternatePort SDA;

	virtual bool SendAddress(int addr, bool tx = true);

	void OnInit();
	virtual bool OnOpen();
	virtual void OnClose();
};

// 软件模拟I2C
class SoftI2C : public I2C
{
public:
	bool HasSecAddress;	// 设备是否有子地址

	// 使用端口和最大速度初始化，因为需要分频，实际速度小于等于该速度
    SoftI2C(uint speedHz = 100000);
    virtual ~SoftI2C();

	virtual void SetPin(Pin scl, Pin sda);
	virtual void GetPin(Pin* scl = nullptr, Pin* sda = nullptr);

	virtual void Start();
	virtual void Stop();

	virtual void WriteByte(byte dat);
	virtual byte ReadByte();
	virtual void Ack(bool ack);
	virtual bool WaitAck(bool ack);

private:
	int _delay;			// 根据速度匹配的延时

	OutputPort	SCL;	// 时钟。开漏输出
	OutputPort	SDA;	// 数据。开漏输出，直接具备读写功能

	virtual bool OnOpen();
	virtual void OnClose();
	
	void Delay();
};

/*
开发历史

2015-10-03
I2C的核心，SCL为高时SDA不许改变，开始停止除外。
SCL为高时，SDA下降沿表示开始，SDA上升沿表示停止。
发送数据后，拉高SCL等待接收方通过SDA发过来的ACK确认。
读取数据时，如果数据提供方来不及提供数据，可拉低SCL。
感谢夏宇@10068953

*/

#endif
