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
	uint8_t id; //����ID
	void DealFrame(); //��������֡
	void(*DUpdateReg)(uint16_t regaddr,uint16_t reglen); //���²���
};

extern uint16_t RegInputu16[]; //����Ĵ���
extern uint16_t RegHoilding16[]; //���ּĴ���
#endif // !_MODBUSLINK_H
