#include "ModbusLink.h"
#include "../component/lib/Buffer.h"

ModbusBase::ModbusBase(USART &uart) : com(uart)
{

}
//��������Ĵ���
void ModbusBase::SetRegInput(int addr0, int reglen, uint16_t* reg, int reggroup)
{
	//�Ƿ��Ĵ�����
	if (reggroup >= ModbusSlaveLink::RegInputLen)
		return;
	this->RegInputs[reggroup].Addr0 = addr0;
	this->RegInputs[reggroup].Lenth = reglen;
	this->RegInputs[reggroup].Reg = reg;
}
//���ñ��ּĴ���
void ModbusBase::SetRegHoid(int addr0, int reglen, uint16_t* reg, int reggroup)
{
	//�Ƿ��Ĵ�����
	if (reggroup >= ModbusSlaveLink::RegHoildingLen)
		return;
	this->RegHoildings[reggroup].Addr0 = addr0;
	this->RegHoildings[reggroup].Lenth = reglen;
	this->RegHoildings[reggroup].Reg = reg;
}
bool ModbusBase::CheckFrame()
{
	int rxlen = com.RxSize();

	if (com.GetBytes(&rxFrame.data[rxFrame.dataLength], rxlen))
	{
		rxFrame.dataLength += rxlen;
	}
	//�ж�����֡��С����Ҫ��
	if (rxFrame.dataLength < 8)
		return false;
#ifdef  DEBUG
	Buffer(rxFrame.data, rxFrame.dataLength).ShowHex(true);
#endif //  DEBUG	
	rxFrame.frameLength = rxFrame.dataLength;
	if (!rxFrame.CheckFrame())
		return false;
	else
		return true;
}

