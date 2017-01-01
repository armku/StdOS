#pragma once
#include "InputPort.h"

// 读取委托 引脚号 引脚电平状态
typedef void(*IOReadHandler)(PinPort, bool);

class CExti: protected InputPort
{
    public:
        CExti(PinPort pin);
        void Init(); //初始化
        void On(); //打开中断
        void Off(); //关闭中断		
        void Register(IOReadHandler handler); // 注册回调
    private:
        void Exti0_state(bool onoff);
        uint eXTI_Line; //中断线
        byte nVIC_IRQChannel; //中断向量
        byte gPIO_PinSource; //中断线
        byte gPIO_PortSourceGPIO; //端口
};
