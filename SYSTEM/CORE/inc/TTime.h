#pragma once

#include "Type.h"

class TTime
{
	public:
		TTime();
		UInt64 Ms() const;//ϵͳ���������ĺ�����
		UInt64 Current() const;//ϵͳ���������ĺ���
		uint Seconds();//ϵͳʱ��
		void SetTime(UInt64 seconds);//����ϵͳ��ǰʱ��,Unix��ʽ����
		void Sleep(uint ms);//��ʱms		
					
		uint TicksToUs(uint ticks) const;
		uint UsToTicks(uint us) const;
	
		UInt64 mCurrent;//ϵͳ���������ĺ�����	
		UInt64 CurrentTicks;//�δ�������΢��̵ĳ��߾���ʱ��		
};
