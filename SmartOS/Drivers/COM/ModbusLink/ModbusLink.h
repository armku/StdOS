#include <string.h>
#include "Security/Crc.h"
#include "USART.h"
#include "../HAL/STM32F1/ARCH_UART.h"

#ifndef _MODBUSLINK_H
#define _MODBUSLINK_H

#define MAX_FRAMEMODBUS_DATA_LENGTH 100

struct DataFrameModbus
{
public:
	byte Address;	// 地址
	byte Code;		// 功能码
	byte Error;		// 是否异常
	byte Length;		// 数据长度
	ushort regAddr;
	ushort regLength;
	byte data[MAX_FRAMEMODBUS_DATA_LENGTH];
	bool isUpdated;
	ushort Crc;		// 校验码
	int frameLength;//当前数据帧长度

public:
	DataFrameModbus()                 //constructor
	{
		this->Address = 1;
		this->Code = 0;
		this->Error = 0;
		this->Length = 0;
		this->regAddr = 0;
		this->regLength = 0;

		memset(this->data, 0, MAX_FRAMEMODBUS_DATA_LENGTH);
		this->isUpdated = false;
		this->Crc = 0;
		this->frameLength = 0;

	}

	//设置传输寄存器值，地址从0开始编码
	void SetReg(byte regaddr, ushort val)
	{
		this->data[3 + regaddr * 2] = val >> 8;
		this->data[3 + regaddr * 2 + 1] = val & 0xff;
	}
	void SetRegLen(byte len)
	{
		this->data[2] = len * 2;
		this->frameLength = 5 + len * 2;
		this->Length = this->frameLength;
	}

	//处理缓冲区数据
	bool CheckFrame(char* buf, int length)
	{
		if (length >= 8)
		{
			for (int i = 0; i < length; i++)
			{
				this->data[i] = buf[i];
			}
			this->Length = length;
			this->CheckFrame();
			return true;
		}
		return false;
	}
	bool CheckFrame()
	{
		if (Length >= 8)
		{
			this->Address = this->data[0];
			this->Code = this->data[1];
			this->regAddr = this->data[2];
			this->regAddr <<= 8;
			this->regAddr |= this->data[3];
			this->regLength = this->data[4];
			this->regLength <<= 8;
			this->regLength |= this->data[5];

			this->frameLength = 8;
			switch (this->Code)
			{
			case 0X02:
			case 0X01:
			case 0X05:
			case 0X04:
			case 0X03:
			case 0X06:
				frameLength = 8;
				break;
			case 0x0F://多个写 0 区输出继电器(指令代码: 0X0F) 
			case 0x10://多个写 4 区输出寄存器(指令代码: 0X10) 
				frameLength = data[6] + frameLength + 1;
				break;
			default:
				frameLength = 0;
				break;
			}
			if (frameLength == 0)
			{
				//非法指令，直接清空缓冲区
				frameLength = this->Length;
				return false;
			}
			//需要的长度不够，直接返回
			if (frameLength > this->Length)
				return false;
			this->Crc = this->data[this->frameLength - 1];
			this->Crc <<= 8;
			this->Crc |= this->data[this->frameLength - 2];

			return true;
		}
		return false;
	}
};

class ModbusErrors
{
public:
	// 错误代码
	enum Errors
	{
		// 错误的功能代码
		Code = 1,

		// 错误的数据地址
		Address = 2,

		// 错误的数据值
		Value = 3,

		// 错误的个数
		Count,

		// 处理出错
		Process,

		// 错误的数据长度
		Length,

		// Crc校验错误
		Crc
	};
};

typedef struct
{
	int Addr0;//起始地址
	int Lenth;//寄存器长度
	ushort *Reg;//寄存器指针
}ModbusReg_T;//寄存器结构体

class ModbusLink
{
public:
	DataFrameModbus txFrame;
	DataFrameModbus rxFrame;
	pFun_UART_buf send_buf;		// 发送缓冲区
};

#endif // !_MODBUSLINK_H
