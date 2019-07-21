#include "ModbusLink.h"

#ifndef _MODBUSRTULINKSLAVE_H
#define _MODBUSRTULINKSLAVE_H

class ModbusRtuLinkSlave :public ModbusLink
{
public:
	/*USARTHAL&com;*/
	const int static RegInputLen = 3;//输入寄存器组大小
	const int static RegHoildingLen = 17;//保持寄存器组大小
	ModbusReg_T RegInputs[RegInputLen];//输入寄存器组
	ModbusReg_T RegHoildings[RegHoildingLen];//保持寄存器组
public:
	ModbusRtuLinkSlave();
	bool CheckFrame();
	bool Send();
	void SetRegInput(int addr0, int reglen, ushort* reg, int reggroup = 0);//设置输入寄存器	
	void SetRegHoid(int addr0, int reglen, ushort* reg, int reggroup = 0);//设置保持寄存器	
private:
	int dealRegInputRead(ushort addr, ushort len);//处理读取输入寄存器 0 正确 1 非法地址 2非法长度
	int dealRegHoildRead(ushort addr, ushort len);//处理读取保持寄存器 0 正确 1 非法地址 2非法长度
	int dealRegHoildWrite(ushort addr, ushort len);//处理写入保持寄存器 0 正确 1 非法地址 2非法长度
	int dealRegHoildWriteOne(ushort addr, ushort val);//处理写入单个保持寄存器 0 正确 1 非法地址 2非法长度
	int searchRegInGroup(ushort addr, ushort len);//查找输入寄存器组，没有查找到返回负值
	int searchRegHoildGroup(ushort addr, ushort len);//查找保持寄存器组，没有查找到返回负值
public:
	byte id; //本机ID
	void DealFrame(); //处理数据帧
	void(*OnUpdateRegHoid)(ushort regaddr, ushort reglen); //更新参数	
};

#endif // !_MODBUSLINKSLAVE_H
