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
	rxFrame.RemoveOneFrame();//�Ƴ�������Ľ�������֡
	txFrame.Cnt++;
	return true;
}

uint16_t RegInputu16[144]; //����Ĵ���
uint16_t RegHoilding16[60];

//��������֡
void ModbusSlaveLink::DealFrame()
{
	if ((this->rxFrame.devid != this->id) && (this->rxFrame.devid != 0))
		return;

	switch (this->rxFrame.fnCode)
	{
	case ReadInputRegisters:
		//��ȡ����Ĵ���
		//����㲥��ַ
		if (this->rxFrame.devid == 0)
			break;
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
		if (this->DealRegInputRead(this->rxFrame.regAddr, this->rxFrame.regLength) == 0)
		{
			this->txFrame.frameLength = this->rxFrame.regLength * 2 + 5;
			this->txFrame.isUpdated = true;
			this->Send();
		}
		break;
	case ReadHoldingRegisters:
		//��ȡ���ּĴ���
		//����㲥��ַ
		if (this->rxFrame.devid == 0)
			break;
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
			//��չ����ָ��
			//this->DealExtChannel();
		}
		break;
	case WriteSingleRegister:
		debug_printf("WriteSingleRegister address %d value %d\n", this->rxFrame.regAddr, this->rxFrame.regLength);
		//Ԥ�õ��Ĵ���
		RegHoilding16[this->rxFrame.regAddr] = this->rxFrame.regLength;
		switch (this->rxFrame.regAddr)
		{
		case 2:
			//ͳһ����ͨ����
			if (this->pupdatewarparafrompc != 0)
				this->pupdatewarparafrompc(this->rxFrame.regLength - 1);
			break;
		case 23:
			//����ͨ������
			if(this->pcommupdatereghoildchannel)
				this->pcommupdatereghoildchannel(this->rxFrame.regLength);
			break;
		default:
			if (this->rxFrame.regAddr < 24)
			{
				//�����豸����
				if (pupdatedevparafrompc != 0)
					this->pupdatedevparafrompc();
			}
			else if (this->rxFrame.regAddr > 23)
			{
				//����ͨ������
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
		//����㲥��ַ
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
		//���ö���Ĵ���
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
				//ͳһ����ͨ����
				tt = this->rxFrame.data[7];
				tt <<= 8;
				tt += this->rxFrame.data[8];
				if (this->pupdatewarparafrompc)
					this->pupdatewarparafrompc(tt - 1);
			}
			else if ((this->rxFrame.regAddr == 23) && (this->rxFrame.regLength == 1))
			{
				//����ͨ������
				tt = this->rxFrame.data[7];
				tt <<= 8;
				tt += this->rxFrame.data[8];
				if (this->pcommupdatereghoildchannel)
					this->pcommupdatereghoildchannel(tt);
			}
			else if (this->rxFrame.regAddr < 24)
			{
				//�����豸����
				if(this->pupdatedevparafrompc!=0)
					this->pupdatedevparafrompc();
			}
			else if (this->rxFrame.regAddr > 23)
			{
				//����ͨ������
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
		//����㲥��ַ
		if (this->rxFrame.devid == 0)
			break;
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
//��������Ĵ���
void ModbusSlaveLink::SetRegInput(int addr0, int reglen, uint16_t* reg, int reggroup)
{
	//�Ƿ��Ĵ�����
	if (reggroup >= ModbusSlaveLink::RegInputLen)
		return;
	this->RegInputs[reggroup].Addr0 = addr0;
	this->RegInputs[reggroup].Lenth = reglen;
	this->RegInputs[reggroup].Reg = reg;
}
//�����ȡ����Ĵ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
int ModbusSlaveLink::DealRegInputRead(uint16_t addr, uint16_t len)
{

	return 0;
}
