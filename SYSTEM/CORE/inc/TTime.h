#pragma once

#include "Type.h"

class TTime
{
	public:
		TTime();
		uint Ms();//ϵͳ���������ĺ�����
		uint Seconds();//ϵͳʱ��
		void SetTime(UInt64 seconds);//����ϵͳ��ǰʱ��,Unix��ʽ����
		void Sleep(uint ms);//��ʱms		
		
		uint64_t Current;//ϵͳ���������ĺ�����	
		uint64_t CurrentTicks;//�δ�������΢��̵ĳ��߾���ʱ��		
		uint TicksToUs(uint ticks) const;
		uint UsToTicks(uint us) const;
	
};

extern TTime Time;//ϵͳʱ�䣬�������û�ֱ��ʹ��
