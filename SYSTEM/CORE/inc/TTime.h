#pragma once

#include "Type.h"

class TTime
{
	public:
		TTime();
		uint Ms();//ϵͳ���������ĺ�����
		uint Seconds();//ϵͳʱ��
		void SetTime(UInt64 seconds);//����ϵͳ��ǰʱ��,Unix��ʽ����
	public:
		uint64_t Current;//ϵͳ���������ĺ�����
		uint CurrentTicks;//�δ�������΢��̵ĳ��߾���ʱ��
	
};

extern TTime Time;//ϵͳʱ�䣬�������û�ֱ��ʹ��
