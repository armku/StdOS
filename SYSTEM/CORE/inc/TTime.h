#pragma once

#include "Type.h"

class TTime
{
	public:
		TTime();
		static UInt64 Ms();//ϵͳ���������ĺ�����
		uint Seconds();//ϵͳʱ��
		void SetTime(UInt64 seconds);//����ϵͳ��ǰʱ��,Unix��ʽ����
		void Sleep(uint ms);//��ʱms		
		
		static UInt64 Current;//ϵͳ���������ĺ�����	
		UInt64 CurrentTicks;//�δ�������΢��̵ĳ��߾���ʱ��		
		uint TicksToUs(uint ticks) const;
		uint UsToTicks(uint us) const;
	
};

extern TTime Time;//ϵͳʱ�䣬�������û�ֱ��ʹ��
