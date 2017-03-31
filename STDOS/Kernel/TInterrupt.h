#pragma once

#include "Type.h"

typedef void (*OnUsartReceive)(ushort num, void *param);

class TInterrupt
{
	public:
		void SetPriority(byte irq,byte level);//�����ж����ȼ�
		void Activate(byte irq,  OnUsartReceive onrcv, void * param);
		void Deactivate(byte irq);//�ر��ж�	
};
extern TInterrupt Interrupt;
