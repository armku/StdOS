#ifndef _I2C_H_
#define _I2C_H_

#include "OnChip\Port.h"

//SCL		开漏复用输出
//SDA		开漏复用输出

class I2CBase
{
public:
	virtual void SetPin(Pin scl, Pin sda)=0;

	virtual void Start()=0;
	virtual void Stop()=0;

	virtual void WriteByte(byte dat)=0;
	virtual byte ReadByte(bool ack)=0;
	virtual bool WaitAck(int retry = 0)=0;
};

// 软件模拟I2C
class I2CSoft:public I2CBase
{
public:
	bool HasSecAddress;	// 设备是否有子地址

	// 使用端口和最大速度初始化，因为需要分频，实际速度小于等于该速度
	I2CSoft(uint speedHz = 100000);

	virtual void SetPin(Pin scl, Pin sda);

	virtual void Start();
	virtual void Stop();

	virtual void WriteByte(byte dat);	
	virtual byte ReadByte(bool ack);	
	virtual bool WaitAck(int retry=0);
private:
	virtual byte ReadByte();
	virtual void Ack(bool ack = true);
private:
	int _delay;			// 根据速度匹配的延时

	Port	SCL;	// 时钟。开漏输出
	Port	SDA;	// 数据。开漏输出，直接具备读写功能
};

/*
I2C的核心，SCL为高时SDA不许改变，开始停止除外。
SCL为高时，SDA下降沿表示开始，SDA上升沿表示停止。
发送数据后，拉高SCL等待接收方通过SDA发过来的ACK确认。
读取数据时，如果数据提供方来不及提供数据，可拉低SCL。
*/
#endif
