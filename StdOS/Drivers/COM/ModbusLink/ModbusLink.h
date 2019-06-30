#include "DataFrameModbus.h"
#include "FIFOBuffer.h"
#include "USART.h"
#include "../HAL/STM32F1/ARCH_UART.h"

#ifndef _MODBUSLINK_H
#define _MODBUSLINK_H

class ModbusErrors
{
public:
	// 错误代码
	enum Errors
	{
		// 错误的功能代码
		Code = 1,

		// 错误的数据地址
		Address = 2,

		// 错误的数据值
		Value = 3,

		// 错误的个数
		Count,

		// 处理出错
		Process,

		// 错误的数据长度
		Length,

		// Crc校验错误
		Crc
	};
};

typedef struct
{
	int Addr0;//起始地址
	int Lenth;//寄存器长度
	uint16_t *Reg;//寄存器指针
}ModbusReg_T;//寄存器结构体

class ModbusSlaveLink
{
public:
	DataFrameModbus txFrame;
	DataFrameModbus rxFrame;
	USARTHAL&com;
	const int static RegInputLen = 3;//输入寄存器组大小
	const int static RegHoildingLen = 17;//保持寄存器组大小
	ModbusReg_T RegInputs[RegInputLen];//输入寄存器组
	ModbusReg_T RegHoildings[RegHoildingLen];//保持寄存器组
public:
	ModbusSlaveLink(USARTHAL&uart);
	bool CheckFrame();
	bool Send();
	void SetRegInput(int addr0, int reglen, uint16_t* reg, int reggroup = 0);//设置输入寄存器	
	void SetRegHoid(int addr0, int reglen, uint16_t* reg, int reggroup = 0);//设置保持寄存器	
private:
	int dealRegInputRead(uint16_t addr,uint16_t len);//处理读取输入寄存器 0 正确 1 非法地址 2非法长度
	int dealRegHoildRead(uint16_t addr, uint16_t len);//处理读取保持寄存器 0 正确 1 非法地址 2非法长度
	int dealRegHoildWrite(uint16_t addr, uint16_t len);//处理写入保持寄存器 0 正确 1 非法地址 2非法长度
	int dealRegHoildWriteOne(uint16_t addr, uint16_t val);//处理写入单个保持寄存器 0 正确 1 非法地址 2非法长度
	int searchRegInGroup(uint16_t addr, uint16_t len);//查找输入寄存器组，没有查找到返回负值
	int searchRegHoildGroup(uint16_t addr, uint16_t len);//查找保持寄存器组，没有查找到返回负值
public:
	uint8_t id; //本机ID
	void DealFrame(); //处理数据帧
	void(*OnUpdateRegHoid)(uint16_t regaddr,uint16_t reglen); //更新参数
	pFun_UART_buf send_buf;		// 发送缓冲区
};

#endif // !_MODBUSLINK_H
