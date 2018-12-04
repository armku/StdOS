#include "DataFrameModbus.h"
#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "USART.h"

#ifndef _MODBUSLINK_H
#define _MODBUSLINK_H

typedef enum MBFunction
{
	/// <summary>读取线圈状态。取得一组逻辑线圈的当前状态（ON/OFF) </summary>
	ReadCoils = 1,
	/// <summary>读取离散量输入状态。取得一组开关输入的当前状态（ON/OFF) </summary>
	ReadInputs = 2,
	/// <summary>读取保持寄存器。在一个或多个保持寄存器中取得当前的二进制值 </summary>
	ReadHoldingRegisters = 3,
	/// <summary>读取输入寄存器。在一个或多个输入寄存器中取得当前的二进制值</summary>
	ReadInputRegisters = 4,
	/// <summary>强置单线圈。强置一个逻辑线圈的通断状态</summary>
	WriteSingleCoil = 5,
	/// <summary>预置单寄存器。把具体二进值装入一个保持寄存器</summary>
	WriteSingleRegister = 6,
	///// <summary>读取异常状态。取得8个内部线圈的通断状态，这8个线圈的地址由控制器决定 </summary>
	//ReadError = 7,
	/// <summary>回送诊断校验。把诊断校验报文送从机，以对通信处理进行评鉴</summary>
	Diagnostics = 8,
	/// <summary>强置多线圈。强置一串连续逻辑线圈的通断</summary>
	WriteMultipleCoils = 15,
	/// <summary>预置多寄存器。把具体的二进制值装入一串连续的保持寄存器</summary>
	WriteMultipleRegisters = 16,
	/// <summary>报告从机标识。可使主机判断编址从机的类型及该从机运行指示灯的状态</summary>
	ReportIdentity = 17,
} MBFunction;

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
	USART &com;
	const int static RegInputLen = 3;//输入寄存器组大小
	const int static RegHoilding = 3;//保持寄存器组大小
	ModbusReg_T RegInputs[RegInputLen];//输入寄存器组
	ModbusReg_T RegHoildings[RegInputLen];//保持寄存器组
public:
	ModbusSlaveLink(USART &uart);
	bool CheckFrame();
	bool Send();
	void SetRegInput(int addr0, int reglen, uint16_t* reg, int reggroup = 0);//设置输入寄存器	
private:
	int dealRegInputRead(uint16_t addr,uint16_t len);//处理读取输入寄存器 0 正确 1 非法地址 2非法长度
	int searchRegInGroup(uint16_t addr, uint16_t len);//查找寄存器组，没有查找到返回负值
public:
	uint8_t id; //本机ID
	void DealFrame(); //处理数据帧
	void(*DUpdateReg)(uint16_t regaddr,uint16_t reglen); //更新参数
	//更新设备参数从pc
	void (*pupdatedevparafrompc)();
	//更新通道参数 通道号 0-15
	void(*pcommupdatereghoildchannel)(uint8_t ch);
	//更新通道参数
	void(*pupdatechannelparafrompc)();
	void(*pupdatewarparafrompc)(uint16_t ch);
};

extern uint16_t RegHoilding16[]; //保持寄存器

#endif // !_MODBUSLINK_H
