#pragma once
#include "InputPort.h"

//回调函数定义 down指示按下还是弹起触发
typedef void(*BtnInCallback)(PinPort pin,bool down); 

class CExti: protected InputPort
{
    public:
        CExti(PinPort pin,byte invert=2);
        void Init(PinPort pin,byte invert=2); //初始化
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
