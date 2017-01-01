#pragma once

#include "BasePort.h"

//回调函数定义 down指示按下还是弹起触发
typedef void(*BtnInCallback)(PinPort pin, bool down);

class InputPort: protected BasePort
{
    public:
        InputPort(PinPort pin, byte invert = 2);
        void Init();
        //void Open();	
        //void UsePress();//启用事件处理
    public:
        //BtnInCallback callBack;//回掉函数
        //uint PressTime;//弹起世间
    private:
        bool binvert; //端口反转
    public:
        void On(); //打开中断
        void Off(); //关闭中断		
        void Register(BtnInCallback handler); // 注册回调
    private:
        void Exti0_state(bool onoff);
        uint eXTI_Line; //中断线
        byte nVIC_IRQChannel; //中断向量
        byte gPIO_PinSource; //中断线
        byte gPIO_PortSourceGPIO; //端口
};
