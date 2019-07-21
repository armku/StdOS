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
//3421写数据   --需查看芯片时序图和芯片指令
void MCP3421::MCP3421_WriteOne(byte  dt)
{
	this->IIC.Start();
	this->IIC.WriteByte(0XD0);
	this->IIC.WaitAck();
	this->IIC.WriteByte(dt);
	this->IIC.WaitAck();
	this->IIC.Stop();
}
//3421读数据   --需查看芯片时序图和芯片指令
int MCP3421::MCP3421_ReadOne(void)
{
	int volatil = 0;
//	byte Step, Temp;
	this->IIC.Start();
	this->IIC.WriteByte(0XD1);
	this->IIC.WaitAck();
	byte adh = this->IIC.ReadByte(true);
	byte adm = this->IIC.ReadByte(true);
	byte adl = this->IIC.ReadByte(false);
	volatil = adh;
	volatil <<= 8;
	volatil += adm;
	volatil <<= 8;
	volatil += adl;
	volatil &= 0X3FFFF;
	
	this->IIC.WaitAck();
	this->IIC.Stop();
	volatil |= 0X3FFFF;
	if (volatil & 0x20000)//最高位为符号为   当采集负数时需特殊处理
	{
		volatil |= 0XFFFE0000;
	}
	return volatil;
}
int MCP3421::GetADValue(_Gain3421 gain)
{
	int das;
	das = MCP3421_ReadOne();
	MCP3421_WriteOne(0x88 | gain);//调ad增益  需查手册
	return das;//(pow(2,gain));
}
float MCP3421::GetVolt()
{
	int val = this->GetADValue(GAIN1);
	return val * 2.048 / 0X20000;
}
