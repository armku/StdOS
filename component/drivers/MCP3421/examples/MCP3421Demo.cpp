#include "Sys.h"
#include "../src/MCP3421.h"

#define _MCP3421TEST_CPP
#ifdef _MCP3421TEST_CPP

MCP3421 mcp3421ad;
void Mcp3421TestRoutin(void* param)
{
	static int icnt = 0;
	if (icnt++ % 2)
	{
		auto volt = mcp3421ad.GetVolt();
		debug_printf("%08d read Volt:%5.5f\n", icnt, volt);
	}
	else
	{
		auto val = mcp3421ad.GetADValue();
		debug_printf("%08d read Volt:%X %d\n", icnt, val, val);
	}
}

void Mcp3421TestInit()
{
	mcp3421ad.SetPin(PB6, PB7);
	Sys.AddTask(Mcp3421TestRoutin, 0, 5, 10000, "Mcp3421TestRoutin");
}
#endif
