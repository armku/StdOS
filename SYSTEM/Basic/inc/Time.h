/*
ϵͳʱ�� Time
Time ��ϵͳʱ�� TTime ���ȫ�ֶ��󣬲������û���ֱ��ʹ�á�
Time.Current ��ǰ��������ͬ Sys.Ms��
Time.CurrentTicks ���������΢�뻹�̵ĳ��߾���ʱ�䡣
Time.SetTime(UInt64 seconds) ����ϵͳ��ǰʱ�䣬Unix ��ʽ������
*/
#pragma once

#include "Type.h"

class CTime
{
	public:
		uint64_t Current;
		uint64_t CurrentTicks;
		void SetTime(UInt64 seconds);
};
