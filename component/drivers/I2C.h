#ifndef _I2C_H_
#define _I2C_H_

#include "../Bsp/Porting.h"

//SCL		开漏复用输出
//SDA		开漏复用输出

// 软件模拟I2C
class SoftI2C
{
public:
	bool HasSecAddress;	// 设备是否有子地址

	// 使用端口和最大速度初始化，因为需要分频，实际速度小于等于该速度
    SoftI2C(uint32_t speedHz = 100000);    

	virtual void SetPin(Pin scl, Pin sda);

	virtual void Start();
	virtual void Stop();

	virtual void WriteByte(uint8_t dat);	
	virtual uint8_t ReadByte(bool ack);	
	virtual bool WaitAck(int retry=0);
private:
	virtual uint8_t ReadByte();
	virtual void Ack(bool ack = true);
private:
	int _delay;			// 根据速度匹配的延时

	mcuGpio	SCL;	// 时钟。开漏输出
	mcuGpio	SDA;	// 数据。开漏输出，直接具备读写功能
};

/*
I2C的核心，SCL为高时SDA不许改变，开始停止除外。
SCL为高时，SDA下降沿表示开始，SDA上升沿表示停止。
发送数据后，拉高SCL等待接收方通过SDA发过来的ACK确认。
读取数据时，如果数据提供方来不及提供数据，可拉低SCL。
*/
#endif
