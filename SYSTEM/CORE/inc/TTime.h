#pragma once

#include "Type.h"

class TTime
{
	public:
		TTime();
		uint Ms();//���ص�ǰ������
		void SetTime(UInt64 seconds);//����ϵͳ��ǰʱ��,Unix��ʽ����
	public:
		uint Current;//��ǰ������
		uint CurrentTicks;//�δ�������΢��̵ĳ��߾���ʱ��
	
};

extern TTime Time;//ϵͳʱ�䣬�������û�ֱ��ʹ��
