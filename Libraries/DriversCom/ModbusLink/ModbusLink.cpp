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

extern uint16_t RegHoilding16[60];

//处理数据帧
void ModbusSlaveLink::DealFrame()
{
	if ((this->rxFrame.devid != this->id) && (this->rxFrame.devid != 0))
		return;

	switch (this->rxFrame.fnCode)
	{
	case ReadInputRegisters:
		//读取输入寄存器
		//处理广播地址
		if (this->rxFrame.devid == 0)
			break;		
		if (this->dealRegInputRead(this->rxFrame.regAddr, this->rxFrame.regLength) == 0)
		{
			this->txFrame.frameLength = this->rxFrame.regLength * 2 + 5;
			this->txFrame.isUpdated = true;
			this->Send();
		}
		break;
	case ReadHoldingRegisters:
		//读取保持寄存器
		//处理广播地址
		if (this->rxFrame.devid == 0)
			break;
					
		if (this->dealRegHoildRead(this->rxFrame.regAddr, this->rxFrame.regLength) == 0)
		{
			this->txFrame.frameLength = this->rxFrame.regLength * 2 + 5;
			this->txFrame.isUpdated = true;
			this->Send();
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
		//处理广播地址
		if (this->rxFrame.devid == 0)
			break;
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
		if (this->dealRegHoildWrite(this->rxFrame.regAddr, this->rxFrame.regLength) == 0)
		{
			//处理广播地址
			if (this->rxFrame.devid == 0)
				break;
			this->txFrame.devid = this->id;
			this->txFrame.fnCode = WriteMultipleRegisters;
			this->txFrame.regLength = this->rxFrame.regLength;
			this->txFrame.data[2] = this->rxFrame.regLength * 2;
			this->txFrame.frameLength = 8;
			this->txFrame.isUpdated = true;
			this->Send();
		}
		break;
	default:
		break;
	}
}
//设置输入寄存器
void ModbusSlaveLink::SetRegInput(int addr0, int reglen, uint16_t* reg, int reggroup)
{
	//非法寄存器组
	if (reggroup >= ModbusSlaveLink::RegInputLen)
		return;
	this->RegInputs[reggroup].Addr0 = addr0;
	this->RegInputs[reggroup].Lenth = reglen;
	this->RegInputs[reggroup].Reg = reg;
}
//设置保持寄存器
void ModbusSlaveLink::SetRegHoid(int addr0, int reglen, uint16_t* reg, int reggroup)
{
	//非法寄存器组
	if (reggroup >= ModbusSlaveLink::RegHoildingLen)
		return;
	this->RegHoildings[reggroup].Addr0 = addr0;
	this->RegHoildings[reggroup].Lenth = reglen;
	this->RegHoildings[reggroup].Reg = reg;
}
//处理读取输入寄存器 0 正确 1 非法地址 2非法长度
int ModbusSlaveLink::dealRegInputRead(uint16_t addr, uint16_t len)
{
	int ret = this->searchRegInGroup(addr,len);
	if (ret == -1)
		return 1;
	if (ret == -2)
		return 2;
	if (ret < 0)
		return 3;

	this->txFrame.devid = this->id;
	this->txFrame.fnCode = ReadInputRegisters;
	this->txFrame.regLength = this->rxFrame.regLength;
	this->txFrame.data[2] = this->rxFrame.regLength * 2;

	
	for (int i = 0; i < this->rxFrame.regLength; i++)
	{
		this->txFrame.SetReg(i,this->RegInputs[ret].Reg[this->rxFrame.regAddr + i- this->RegInputs[ret].Addr0]);
	}

	return 0;
}
//处理读取保持寄存器 0 正确 1 非法地址 2非法长度
int ModbusSlaveLink::dealRegHoildRead(uint16_t addr, uint16_t len)
{
	int ret = this->searchRegHoildGroup(addr, len);
	if (ret == -1)
		return 1;
	if (ret == -2)
		return 2;
	if (ret < 0)
		return 3;
	this->txFrame.devid = this->id;
	this->txFrame.fnCode = ReadHoldingRegisters;
	this->txFrame.regLength = this->rxFrame.regLength;
	this->txFrame.data[2] = this->rxFrame.regLength * 2;
	for (int i = 0; i < this->rxFrame.regLength; i++)
	{
		this->txFrame.SetReg(i, this->RegHoildings[ret].Reg[this->rxFrame.regAddr + i - this->RegHoildings[ret].Addr0]);
	}
	return 0;
}
//处理写入保持寄存器 0 正确 1 非法地址 2非法长度
int ModbusSlaveLink::dealRegHoildWrite(uint16_t addr, uint16_t len)
{
	int ret = this->searchRegHoildGroup(addr, len);
	if (ret == -1)
		return 1;
	if (ret == -2)
		return 2;
	if (ret < 0)
		return 3;

	uint16_t tt;

	for (int i = 0; i < this->rxFrame.regLength; i++)
	{
		tt = this->rxFrame.data[i * 2 + 7];
		tt <<= 8;
		tt += this->rxFrame.data[i * 2 + 8];
		this->RegHoildings[ret].Reg[this->rxFrame.regAddr + i - this->RegHoildings[ret].Addr0] = tt;
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
		if (this->pupdatedevparafrompc != 0)
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

	return 0;
}
//查找寄存器组，没有查找到返回负值
int ModbusSlaveLink::searchRegInGroup(uint16_t addr, uint16_t len)
{
	for (int i = 0; i < this->RegInputLen; i++)
	{
		if ((addr >= this->RegInputs[i].Addr0) && //起始地址对
			((addr + len) < (this->RegInputs[i].Addr0 + this->RegInputs[i].Lenth)) && //长度对
			(this->RegInputs[i].Reg!=0)) //寄存器指针不为空
			return i;
	}

	return -1;
}
//查找保持寄存器组，没有查找到返回负值
int ModbusSlaveLink::searchRegHoildGroup(uint16_t addr, uint16_t len)
{
	for (int i = 0; i < this->RegHoildingLen; i++)
	{
		if ((addr >= this->RegHoildings[i].Addr0) && //起始地址对
			((addr + len) < (this->RegHoildings[i].Addr0 + this->RegHoildings[i].Lenth)) && //长度对
			(this->RegHoildings[i].Reg != 0)) //寄存器指针不为空
			return i;
	}

	return -1;
}
