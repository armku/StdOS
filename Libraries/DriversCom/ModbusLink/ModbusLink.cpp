#include "ModbusLink.h"

ModbusSlaveLink::ModbusSlaveLink(USART &uart) :com(uart)
{
	this->DUpdateReg = 0;
}

bool ModbusSlaveLink::CheckFrame()
{
	bool ret = false;
	int rxlen = com.RxSize();

	if (com.GetBytes(&rxFrame.data[rxFrame.dataLength], rxlen))
	{
		rxFrame.dataLength += rxlen;
	}
	if (!rxFrame.CheckFrame())
		return false;	
	if (rxFrame.frameLength > 3)
	{
		auto crc11 = Crc::CRC16RTU(rxFrame.data, rxFrame.frameLength - 2);
	}
#if defined DEBUG
	/*Buffer(rxFrame.data, rxFrame.dataLength).ShowHex(true,' ');*/
#endif
	ret= rxFrame.CheckFrame();
	if (ret)
		rxFrame.Cnt++;
	return ret;
}

bool ModbusSlaveLink::Send()
{
	if (!txFrame.isUpdated) //no new frame data, no need to send
		return false;	
	txFrame.data[0] = txFrame.devid;
	txFrame.data[1] = txFrame.fnCode;
	auto crc = Crc::CRC16RTU(txFrame.data, txFrame.frameLength - 2);
	txFrame.data[txFrame.frameLength - 2] = crc & 0xff;
	txFrame.data[txFrame.frameLength - 1] = crc >> 8;
	com.SendBytes(txFrame.data, txFrame.frameLength);
	txFrame.isUpdated = false;
	rxFrame.RemoveOneFrame();//移除处理完的接收数据帧
	txFrame.Cnt++;
	return true;
}

uint16_t RegInputu16[144]; //输入寄存器
uint16_t RegHoilding16[60];

//处理数据帧
void ModbusSlaveLink::DealFrame()
{
	if (this->rxFrame.devid != this->id)
		return;

	switch (this->rxFrame.fnCode)
	{
	case ReadInputRegisters:
		//读取输入寄存器
		this->txFrame.devid = this->id;
		this->txFrame.fnCode = ReadInputRegisters;
		this->txFrame.regLength = this->rxFrame.regLength;
		this->txFrame.data[2] = this->rxFrame.regLength * 2;
		if (this->rxFrame.regLength >= 70)
		{
			this->rxFrame.regLength = 70;
		}
		for (int i = 0; i < this->rxFrame.regLength; i++)
		{
			this->txFrame.SetReg(i, RegInputu16[this->rxFrame.regAddr + i]);
		}		
		this->txFrame.frameLength = this->rxFrame.regLength * 2 + 5;
		this->txFrame.isUpdated = true;
		this->Send();
		break;
	case ReadHoldingRegisters:
		//读取保持寄存器
		if (this->rxFrame.regAddr < 100)
		{
			this->txFrame.devid = this->id;
			this->txFrame.fnCode = ReadHoldingRegisters;
			this->txFrame.regLength = this->rxFrame.regLength;
			this->txFrame.data[2] = this->rxFrame.regLength * 2;
			if (this->rxFrame.regLength >= 60)
			{
				this->rxFrame.regLength = 60;
			}
			for (int i = 0; i < this->rxFrame.regLength; i++)
			{
				this->txFrame.SetReg(i, RegHoilding16[this->rxFrame.regAddr + i]);
			}
			this->txFrame.frameLength = this->rxFrame.regLength * 2 + 5;
			this->txFrame.isUpdated = true;
			this->Send();
		}
		else
		{
			//扩展测试指令
			this->DealExtChannel();
		}
		break;
	case WriteSingleRegister:
		debug_printf("WriteSingleRegister address %d value %d\n", this->rxFrame.regAddr, this->rxFrame.regLength);
		//预置单寄存器
		RegHoilding16[this->rxFrame.regAddr] = this->rxFrame.regLength;
		switch (this->rxFrame.regAddr)
		{
		case 2:
			//统一报警通道号
			if (this->pupdatewarparafrompc != 0)
				this->pupdatewarparafrompc(this->rxFrame.regLength - 1);
			break;
		case 23:
			//加载通道参数
			if(this->pcommupdatereghoildchannel)
				this->pcommupdatereghoildchannel(this->rxFrame.regLength);
			break;
		default:
			if (this->rxFrame.regAddr < 24)
			{
				//更新设备参数
				if (pupdatedevparafrompc != 0)
					this->pupdatedevparafrompc();
			}
			else if (this->rxFrame.regAddr > 23)
			{
				//更新通道参数
				if (this->pupdatechannelparafrompc != 0)
					this->pupdatechannelparafrompc();
			}
			else {}
			break;
		}
		if (this->DUpdateReg)
		{
			this->DUpdateReg(this->rxFrame.regAddr, 0);
		}
		this->txFrame.devid = this->id;
		this->txFrame.fnCode = WriteSingleRegister;
		this->txFrame.regLength = this->rxFrame.regLength;
		this->txFrame.data[2] = this->rxFrame.regLength * 2;
		this->txFrame.frameLength = 8;
		this->txFrame.isUpdated = true;
		this->Send();
		break;
	case WriteMultipleRegisters:
		//设置多个寄存器
		uint16_t tt;
		if ((this->rxFrame.regAddr + this->rxFrame.regLength) < 58)
		{
			for (int i = 0; i < this->rxFrame.regLength; i++)
			{
				tt = this->rxFrame.data[i * 2 + 7];
				tt <<= 8;
				tt += this->rxFrame.data[i * 2 + 8];
				RegHoilding16[this->rxFrame.regAddr + i] = tt;
			}
			if ((this->rxFrame.regAddr == 2) && (this->rxFrame.regLength == 1))
			{
				//统一报警通道号
				tt = this->rxFrame.data[7];
				tt <<= 8;
				tt += this->rxFrame.data[8];
				if (this->pupdatewarparafrompc)
					this->pupdatewarparafrompc(tt - 1);
			}
			else if ((this->rxFrame.regAddr == 23) && (this->rxFrame.regLength == 1))
			{
				//加载通道参数
				tt = this->rxFrame.data[7];
				tt <<= 8;
				tt += this->rxFrame.data[8];
				if (this->pcommupdatereghoildchannel)
					this->pcommupdatereghoildchannel(tt);
			}
			else if (this->rxFrame.regAddr < 24)
			{
				//更新设备参数
				if(this->pupdatedevparafrompc!=0)
					this->pupdatedevparafrompc();
			}
			else if (this->rxFrame.regAddr > 23)
			{
				//更新通道参数
				if (this->pupdatechannelparafrompc != 0)
					this->pupdatechannelparafrompc();
			}
			else if (this->DUpdateReg)
			{
				this->DUpdateReg(0xffff, 0);
			}
			else {}
		}
		else {}
		
		this->txFrame.devid = this->id;
		this->txFrame.fnCode = WriteMultipleRegisters;
		this->txFrame.regLength = this->rxFrame.regLength;
		this->txFrame.data[2] = this->rxFrame.regLength * 2; 
		this->txFrame.frameLength = 8;
		this->txFrame.isUpdated = true;
		this->Send();
		break;
	default:
		break;
	}
}
#if 0
//例程，需要放在app中
//处理扩展通道数据，测试数据通道
void ModbusSlaveLink::DealExtChannel()
{
	if (this->rxFrame.regLength == 21)
	{
		//读取通道参数
		int ch = this->rxFrame.regAddr / 1000 % 10 * 10 + this->rxFrame.regAddr / 100 % 10;
		ch -= 10;
		if (ch < 16 && ch >= 0)
		{
			//debug_printf("读取保持寄存器 地址;%d 长度 %d ch:%d\n", this->rxFrame.regAddr, this->rxFrame.regLength,ch);
			//通道在正常范围内
			this->txFrame.devid = this->id;
			this->txFrame.fnCode = ReadHoldingRegisters;
			this->txFrame.regLength = this->rxFrame.regLength;
			this->txFrame.data[2] = this->rxFrame.regLength * 2;

			this->txFrame.SetReg(0, 1);
			this->txFrame.SetReg(1, 2);
			this->txFrame.SetReg(20, 21);
			
			this->txFrame.frameLength = this->rxFrame.regLength * 2 + 5;
			this->txFrame.isUpdated = true;
			this->Send();
		}
	}
}
#endif
