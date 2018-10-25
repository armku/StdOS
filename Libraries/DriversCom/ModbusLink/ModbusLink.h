#include "DataFrameModbus.h"
#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "USART.h"

#ifndef _MODBUSLINK_H
#define _MODBUSLINK_H

class ModbusSlaveLink
{
public:
	DataFrameModbus txFrame;
	DataFrameModbus rxFrame;
	USART &com;
public:
	ModbusSlaveLink(USART &uart);
	bool CheckFrame();
	bool Send();
private:
public:
	uint8_t id; //本机ID
	void DealFrame(); //处理数据帧
	void(*DUpdateReg)(uint16_t regaddr,uint16_t reglen); //更新参数
};

extern uint16_t RegInputu16[]; //输入寄存器
extern uint16_t RegHoilding16[]; //保持寄存器
#endif // !_MODBUSLINK_H
