#pragma once

#include "Type.h"

class TInterrupt
{
	public:
		void SetPriority(byte irq,byte level);//设置中断优先级
		void Deactivate(byte irq);//关闭中断
	
};
extern TInterrupt Interrupt;
