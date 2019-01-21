#include "MCP3421.h"

MCP3421::MCP3421()
{
	//this->Address = 0X0D;
}
void MCP3421::SetPin(Pin pscl, Pin psda)
{
	this->IIC.SetPin(pscl, psda);
}
void MCP3421::Init()
{

}
//3421д����   --��鿴оƬʱ��ͼ��оƬָ��
void MCP3421::MCP3421_WriteOne(uint8_t  dt)
{
	this->IIC.Start();
	this->IIC.WriteByte(0XD0);
	this->IIC.WaitAck();
	this->IIC.WriteByte(dt);
	this->IIC.WaitAck();
	this->IIC.Stop();
}
//3421������   --��鿴оƬʱ��ͼ��оƬָ��
int32_t MCP3421::MCP3421_ReadOne(void)
{
	volatil = 0;
	this->IIC.Start();
	this->IIC.WriteByte(0XD1);
	this->IIC.WaitAck();
	adh = this->IIC.ReadByte(true);
	adm = this->IIC.ReadByte(true);
	adl = this->IIC.ReadByte(false);
	volatil = adh;
	volatil <<= 8;
	volatil += adm;
	volatil <<= 8;
	volatil += adl;
	volatil &= 0X3FFFF;
	
	this->IIC.WaitAck();
	this->IIC.Stop();
	if (volatil & 0x20000)//���λΪ����Ϊ   ���ɼ�����ʱ�����⴦��
	{
		volatil |= 0XFFFE0000;
	}
	this->adOrigin = volatil;
	this->Value = this->adOrigin;
	return this->adOrigin;
}
int32_t MCP3421::GetADValue(_Gain3421 gain)
{
	int32_t das;
	das = MCP3421_ReadOne();
	MCP3421_WriteOne(0x88 | gain);//��ad����  ����ֲ�
	return das;//(pow(2,gain));
}
float MCP3421::GetVolt()
{
	auto val = this->GetADValue(GAIN1);
	this->ValueVolt= val * 2.048 / 0X20000;
	return this->ValueVolt;
}
