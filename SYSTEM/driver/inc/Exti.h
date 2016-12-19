#pragma once
#include "Port.h"

class CExti
{
    public:
        CExti(uint16_t pin);
        ~CExti();
    public:
        void Init(); //初始化
        void On(); //打开中断
        void Off(); //关闭中断
	private:
		CPort *port;	//端口
	private:
		void Exti0_state(bool onoff);
};
