#pragma once

#include "Type.h"

typedef void (*OnUsartReceive)(ushort num, void *param);

class TInterrupt
{
	public:
		void SetPriority(byte irq,byte level);//设置中断优先级
		void Activate(byte irq,  OnUsartReceive onrcv, void * param);
		void Deactivate(byte irq);//关闭中断	
};
extern TInterrupt Interrupt;
