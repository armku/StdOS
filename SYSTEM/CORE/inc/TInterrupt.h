#pragma once

#include "Type.h"

class TInterrupt
{
	public:
		void SetPriority(byte irq,byte level);//�����ж����ȼ�
		void Deactivate(byte irq);//�ر��ж�
	
};
extern TInterrupt Interrupt;
