#pragma once

#include "Type.h"

class TimeCost
{
	public:
		TimeCost();
		void Show();//��ʾִ��ʱ��
		void Start();//��ʼ��ʱ
		uint Elapsed();//������ʱ�� us
	private:
		uint startTickCounts;//��ʼʱ��
};
