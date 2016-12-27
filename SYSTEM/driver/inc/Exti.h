#pragma once
#include "BasePort.h"

class CExti
{
    public:
        CExti(PinPort pin);
        ~CExti();
    public:
        void Init(); //初始化
        void On(); //打开中断
        void Off(); //关闭中断
	private:
		BasePort *port;	//端口
	private:
		void Exti0_state(bool onoff);
		uint eXTI_Line;//中断线
		byte nVIC_IRQChannel;//中断向量
		byte gPIO_PinSource;//中断线
		byte gPIO_PortSourceGPIO;//端口
};
