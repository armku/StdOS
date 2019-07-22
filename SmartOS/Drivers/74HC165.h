﻿#ifndef _74HC165_H_
#define _74HC165_H_

#include "Device\Port.h"
#include "Device\Power.h"

// 温湿度传感器
class IC74HC165
{
private:
	OutputPort	_PL;
	OutputPort	_SCK;
	InputPort	_In;
	OutputPort _CE;
	
public:
	bool Opened;
	// pl采集，cp时钟，Q7输出，ce使能
	IC74HC165(Pin pl, Pin sck, Pin in, Pin ce = P0);
	
	bool Open();
	bool Close();
	
	// 级联读取  buf,数据放置位置，count，级联ic数
	bool Read(byte *buf, byte count);
	// 读一个IC 的数据
	byte Read();
};

#endif
