#include "Modbus\modbus.h"

//#define DEBUGModbusTest

#ifdef DEBUGModbusTest

	byte ModbusTestbuf[]={0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
	byte ModbusTestBufout[10];
	Stream streammodbustest(ModbusTestbuf,ArrayLength(ModbusTestbuf));
	Stream streammodbustestout(ModbusTestBufout,ArrayLength(ModbusTestBufout));
	Modbus Modbusent;
	void ModbusTest()
	{
		Modbusent.Read(streammodbustest);
		Modbusent.Write(streammodbustestout);
	}

#endif
