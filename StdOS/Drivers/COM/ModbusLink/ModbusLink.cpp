#include "ModbusLink.h"

ModbusSlaveLink::ModbusSlaveLink()
{
	this->OnUpdateRegHoid = 0;
	this->id = 1;
	this->send_buf = NULL;
	/*for (int i = 0; i < ArrayLength(this->RegHoildings); i++)
	{
		this->RegHoildings[i].Reg = 0;
	}*/
}
#include "Buffer.h"
bool ModbusSlaveLink::CheckFrame()
{		
#ifdef  DEBUG
	Buffer(rxFrame.data, rxFrame.Length).ShowHex(true);
#endif //  DEBUG	
	if (!rxFrame.CheckFrame())
		return false;
	else
		return true;
}

bool ModbusSlaveLink::Send()
{
	if (!txFrame.isUpdated) //no new frame data, no need to send
		return false;	
	txFrame.data[0] = txFrame.Address;
	txFrame.data[1] = txFrame.Code;
	txFrame.Crc2 = Crc::CRC16RTU(txFrame.data, txFrame.frameLength - 2);
	txFrame.data[txFrame.frameLength - 2] = txFrame.Crc2 & 0xff;
	txFrame.data[txFrame.frameLength - 1] = txFrame.Crc2 >> 8;
	/*com.SendBytes(txFrame.data, txFrame.frameLength);*/
	this->send_buf(txFrame.data, txFrame.frameLength);
	Buffer(txFrame.data, txFrame.frameLength).ShowHex();
	txFrame.isUpdated = false;
	rxFrame.RemoveOneFrame();//�Ƴ�������Ľ�������֡
	txFrame.Cnt++;
	return true;
}

//��������֡
void ModbusSlaveLink::DealFrame()
{
	if ((this->rxFrame.Address != this->id) && (this->rxFrame.Address != 0))
		return;

	switch (this->rxFrame.Code)
	{
	case 4:
		//ǿ�õ���Ȧ��ǿ��һ���߼���Ȧ��ͨ��״̬
		//��ȡ����Ĵ���
		//����㲥��ַ
		if (this->rxFrame.Address == 0)
			break;		
		if (this->dealRegInputRead(this->rxFrame.regAddr, this->rxFrame.regLength) == 0)
		{
			this->txFrame.frameLength = this->rxFrame.regLength * 2 + 5;
			this->txFrame.isUpdated = true;
			this->Send();
		}
		break;
	case 3:
		//��ȡ����Ĵ�������һ����������Ĵ�����ȡ�õ�ǰ�Ķ�����ֵ
		//��ȡ���ּĴ���
		//����㲥��ַ
		if (this->rxFrame.Address == 0)
			break;
					
		if (this->dealRegHoildRead(this->rxFrame.regAddr, this->rxFrame.regLength) == 0)
		{
			this->txFrame.frameLength = this->rxFrame.regLength * 2 + 5;
			this->txFrame.isUpdated = true;
			this->Send();
		}
		break;
	case 6:
		//��ȡ�쳣״̬��ȡ��8���ڲ���Ȧ��ͨ��״̬����8����Ȧ�ĵ�ַ�ɿ���������
		debug_printf("WriteSingleRegister address %d value %d\n", this->rxFrame.regAddr, this->rxFrame.regLength);
		//Ԥ�õ��Ĵ���
		
		//this->rxFrame.regLength��Ϊ��Ҫ���õ�ֵ
		if (this->dealRegHoildWriteOne(this->rxFrame.regAddr, this->rxFrame.regLength) == 0)
		{
			//����㲥��ַ
			if (this->rxFrame.Address == 0)
				break;
			this->txFrame.Address = this->id;
			this->txFrame.Code = 6;
			this->txFrame.regLength = this->rxFrame.regLength;
			this->txFrame.data[2] = this->rxFrame.data[2];
			this->txFrame.data[3] = this->rxFrame.data[3];
			this->txFrame.data[4] = this->rxFrame.data[4];
			this->txFrame.data[5] = this->rxFrame.data[5];

			this->txFrame.frameLength = 8;
			this->txFrame.isUpdated = true;
			this->Send();
		}
		break;
	case 16:
		//����ӻ���ʶ����ʹ�����жϱ�ַ�ӻ������ͼ��ôӻ�����ָʾ�Ƶ�״̬
		//���ö���Ĵ���		
		debug_printf("WriteMultipleRegisters\n");
		if (this->dealRegHoildWrite(this->rxFrame.regAddr, this->rxFrame.regLength) == 0)
		{
			//����㲥��ַ
			if (this->rxFrame.Address == 0)
				break;
			this->txFrame.Address = this->id;
			this->txFrame.Code = 16;
			this->txFrame.regLength = this->rxFrame.regLength;
			this->txFrame.data[2] = this->rxFrame.data[2];
			this->txFrame.data[3] = this->rxFrame.data[3];
			this->txFrame.data[4] = this->rxFrame.data[4];
			this->txFrame.data[5] = this->rxFrame.data[5];
			this->txFrame.frameLength = 8;
			this->txFrame.isUpdated = true;
			this->Send();
		}
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
//���ñ��ּĴ���
void ModbusSlaveLink::SetRegHoid(int addr0, int reglen, uint16_t* reg, int reggroup)
{
	//�Ƿ��Ĵ�����
	if (reggroup >= ModbusSlaveLink::RegHoildingLen)
		return;
	this->RegHoildings[reggroup].Addr0 = addr0;
	this->RegHoildings[reggroup].Lenth = reglen;
	this->RegHoildings[reggroup].Reg = reg;
}
//�����ȡ����Ĵ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
int ModbusSlaveLink::dealRegInputRead(uint16_t addr, uint16_t len)
{
	int ret = this->searchRegInGroup(addr,len);
	if (ret == -1)
		return 1;
	if (ret == -2)
		return 2;
	if (ret < 0)
		return 3;

	this->txFrame.Address = this->id;
	this->txFrame.Code = 4;
	this->txFrame.regLength = this->rxFrame.regLength;
	this->txFrame.data[2] = this->rxFrame.regLength * 2;

	
	for (int i = 0; i < this->rxFrame.regLength; i++)
	{
		this->txFrame.SetReg(i,this->RegInputs[ret].Reg[this->rxFrame.regAddr + i- this->RegInputs[ret].Addr0]);
	}

	return 0;
}
//�����ȡ���ּĴ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
int ModbusSlaveLink::dealRegHoildRead(uint16_t addr, uint16_t len)
{
	int ret = this->searchRegHoildGroup(addr, len);
	if (ret == -1)
		return 1;
	if (ret == -2)
		return 2;
	if (ret < 0)
		return 3;
	this->txFrame.Address = this->id;
	this->txFrame.Code = 3;
	this->txFrame.regLength = this->rxFrame.regLength;
	this->txFrame.data[2] = this->rxFrame.regLength * 2;
	for (int i = 0; i < this->rxFrame.regLength; i++)
	{
		this->txFrame.SetReg(i, this->RegHoildings[ret].Reg[this->rxFrame.regAddr + i - this->RegHoildings[ret].Addr0]);
	}
	return 0;
}
//����д�뱣�ּĴ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
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
	debug_printf("\nWriteMultipleRegisters reg:%d len:%d\n", addr, len);

	for (int i = 0; i < this->rxFrame.regLength; i++)
	{
		tt = this->rxFrame.data[i * 2 + 7];
		tt <<= 8;
		tt += this->rxFrame.data[i * 2 + 8];
		this->RegHoildings[ret].Reg[this->rxFrame.regAddr + i - this->RegHoildings[ret].Addr0] = tt;
	}
	if (this->OnUpdateRegHoid)
	{
		this->OnUpdateRegHoid(addr, len);
	}

	return 0;
}
//����д�뵥�����ּĴ��� 0 ��ȷ 1 �Ƿ���ַ 2�Ƿ�����
int ModbusSlaveLink::dealRegHoildWriteOne(uint16_t addr, uint16_t val)
{
	int ret = this->searchRegHoildGroup(addr, 1);
	if (ret == -1)
		return 1;
	if (ret == -2)
		return 2;
	if (ret < 0)
		return 3;
	this->RegHoildings[ret].Reg[addr] = val;
	if (this->OnUpdateRegHoid)
	{
		this->OnUpdateRegHoid(addr, 1);
	}

	return 0;
}
//���ҼĴ����飬û�в��ҵ����ظ�ֵ
int ModbusSlaveLink::searchRegInGroup(uint16_t addr, uint16_t len)
{
	for (int i = 0; i < this->RegInputLen; i++)
	{
		if ((addr >= this->RegInputs[i].Addr0) && //��ʼ��ַ��
			((addr + len) <= (this->RegInputs[i].Addr0 + this->RegInputs[i].Lenth)) && //���ȶ�
			(this->RegInputs[i].Reg!=0)) //�Ĵ���ָ�벻Ϊ��
			return i;
	}

	return -1;
}
//���ұ��ּĴ����飬û�в��ҵ����ظ�ֵ
int ModbusSlaveLink::searchRegHoildGroup(uint16_t addr, uint16_t len)
{
	for (int i = 0; i < this->RegHoildingLen; i++)
	{
		if ((addr >= this->RegHoildings[i].Addr0) && //��ʼ��ַ��
			((addr + len) <= (this->RegHoildings[i].Addr0 + this->RegHoildings[i].Lenth)) && //���ȶ�
			(this->RegHoildings[i].Reg != 0)) //�Ĵ���ָ�벻Ϊ��
			return i;
	}

	return -1;
}
