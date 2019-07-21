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
	byte Address;	// ��ַ
	byte Code;		// ������
	byte Error;		// �Ƿ��쳣
	byte Length;		// ���ݳ���
	ushort regAddr;
	ushort regLength;
	byte data[MAX_FRAMEMODBUS_DATA_LENGTH];
	bool isUpdated;
	ushort Crc;		// У����
	int frameLength;//��ǰ����֡����

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

	//���ô���Ĵ���ֵ����ַ��0��ʼ����
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

	//������������
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
			case 0x0F://���д 0 ������̵���(ָ�����: 0X0F) 
			case 0x10://���д 4 ������Ĵ���(ָ�����: 0X10) 
				frameLength = data[6] + frameLength + 1;
				break;
			default:
				frameLength = 0;
				break;
			}
			if (frameLength == 0)
			{
				//�Ƿ�ָ�ֱ����ջ�����
				frameLength = this->Length;
				return false;
			}
			//��Ҫ�ĳ��Ȳ�����ֱ�ӷ���
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
	// �������
	enum Errors
	{
		// ����Ĺ��ܴ���
		Code = 1,

		// ��������ݵ�ַ
		Address = 2,

		// ���������ֵ
		Value = 3,

		// ����ĸ���
		Count,

		// �������
		Process,

		// ��������ݳ���
		Length,

		// CrcУ�����
		Crc
	};
};

typedef struct
{
	int Addr0;//��ʼ��ַ
	int Lenth;//�Ĵ�������
	ushort *Reg;//�Ĵ���ָ��
}ModbusReg_T;//�Ĵ����ṹ��

class ModbusLink
{
public:
	DataFrameModbus txFrame;
	DataFrameModbus rxFrame;
	pFun_UART_buf send_buf;		// ���ͻ�����
};

#endif // !_MODBUSLINK_H
