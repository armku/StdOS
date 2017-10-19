#include "Modbus\modbus.h"

#ifdef DEBUG

	byte ModbusTestbuf[]={0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
	Stream streammodbustest(ModbusTestbuf,ArrayLength(ModbusTestbuf));
	Modbus Modbusent;
	void ModbusTest()
	{
		Modbusent.Read(streammodbustest);
	}

#endif
