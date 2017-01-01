#pragma once
#include "BasePort.h"


// 读取委托
typedef void (*IOReadHandler)(PinPort , bool );
/* 中断状态结构体 */
/* 一共16条中断线，意味着同一条线每一组只能有一个引脚使用中断 */
typedef struct TIntState
{
    PinPort Pin;
    IOReadHandler Handler;
    bool OldValue;
} IntState;

class CExti:public BasePort
{
    public:
        CExti(PinPort pin);
        void Init(); //初始化
        void On(); //打开中断
        void Off(); //关闭中断		
		void TIO_Register(PinPort pin, IOReadHandler handler);// 注册回调  及中断使能
	private:
		BasePort *port;	//端口
	private:
		void Exti0_state(bool onoff);
		uint eXTI_Line;//中断线
		byte nVIC_IRQChannel;//中断向量
		byte gPIO_PinSource;//中断线
		byte gPIO_PortSourceGPIO;//端口
};
