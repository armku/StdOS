#include "Type.h"
#include "Security/Crc.h"
#include "../DataFrame.h"

#ifndef _DATAFRAMEMODBUS_H
#define _DATAFRAMEMODBUS_H

struct DataFrameModbus:public DataFrame
{
public:
	uint8_t Address;	// ��ַ
	uint8_t Code;		// ������
	uint8_t Error;		// �Ƿ��쳣
	uint16_t regAddr;
	uint16_t regLength;
	bool isUpdated;
	uint16_t Crc;		// У����
	uint16_t Crc2;		// ��̬����õ���У����
	int frameLength;//��ǰ����֡����
	int Cnt;//����֡����

public:
	DataFrameModbus()                 //constructor
	{
		isUpdated = false;
		Cnt = 0;
	}

	bool VerifyCheckCode()
	{
		this->Crc2 = Crc::CRC16RTU(data, frameLength-2);
		if (this->Crc2 == this->Crc)
			return true;
		else
			return false;
	}
	//���ô���Ĵ���ֵ����ַ��0��ʼ����
	void SetReg(uint8_t regaddr, uint16_t val)
	{
		this->data[3 + regaddr * 2] = val >> 8;
		this->data[3 + regaddr * 2 + 1] = val & 0xff;
	}
	void SetRegLen(uint8_t len)
	{
		this->data[2] = len * 2;
		this->frameLength = 5 + len * 2;
		this->dataLength = this->frameLength;
	}
	bool RemoveOneFrame()
	{
		if (frameLength<0 || frameLength>this->dataLength)
			return false;
		for (int i = 0; i < this->dataLength - frameLength; i++)
		{
			this->data[i] = this->data[i + frameLength];
		}
		this->dataLength -= frameLength;
		this->frameLength = 0;
		return true;
	}
	bool CheckFrame()
	{
		if (this->dataLength >= 8)
		{
			this->Address = this->data[0];
			this->Code = this->data[1];
			this->regAddr = this->data[2];
			this->regAddr <<= 8;
			this->regAddr |= this->data[3];
			this->regLength = this->data[4];
			this->regLength <<= 8;
			this->regLength |= this->data[5];

			//this->frameLength = 8;
			//switch (this->Code)
			//{
			//case 0X02:
			//case 0X01:
			//case 0X05:
			//case 0X04:
			//case 0X03:
			//case 0X06:
			//	frameLength = 8;
			//	break;
			//case 0x0F://���д 0 ������̵���(ָ�����: 0X0F) 
			//case 0x10://���д 4 ������Ĵ���(ָ�����: 0X10) 
			//	frameLength = data[6] + frameLength + 1;
			//	break;
			//default:
			//	frameLength = 0;
			//	break;
			//}
			if (frameLength == 0)
			{
				//�Ƿ�ָ�ֱ����ջ�����
				frameLength = this->dataLength;
				this->RemoveOneFrame();
				return false;
			}
			//��Ҫ�ĳ��Ȳ�����ֱ�ӷ���
			if (frameLength > this->dataLength)
				return false;
			this->Crc = this->data[this->frameLength - 1];
			this->Crc <<= 8;
			this->Crc |= this->data[this->frameLength - 2];

			if (this->VerifyCheckCode())
			{				
				return true;
			}
			else
			{
				this->RemoveOneFrame();
				return false;
			}
		}
		return false;
	}
	void CreateCheckCode()
	{
		Crc = Code + frameLength;
		for (uint8_t i = 0; i < frameLength; i++)
			Crc += data[i];
	}
};

#endif // !_DATAFRAMEMODBUS_H
