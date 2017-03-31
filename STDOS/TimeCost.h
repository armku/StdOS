#pragma once

#include "Type.h"

class TimeCost
{
	public:
		TimeCost();
		void Show();//显示执行时间
		void Start();//开始计时
		uint Elapsed();//经过的时间 us
	private:
		uint startTickCounts;//开始时间
};
