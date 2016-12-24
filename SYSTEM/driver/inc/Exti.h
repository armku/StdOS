#pragma once
#include "Port.h"

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
		CPort *port;	//端口
	private:
		void Exti0_state(bool onoff);
		uint32_t eXTI_Line;//中断线
		uint8_t nVIC_IRQChannel;//中断向量
		uint8_t gPIO_PinSource;//中断线
		uint8_t gPIO_PortSourceGPIO;//端口
};
