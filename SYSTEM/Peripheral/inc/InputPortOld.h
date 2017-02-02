#pragma once

#include "Sys.h"

//回调函数定义 down指示按下还是弹起触发
typedef void(*BtnInCallbackOld)(Pin pin, bool down);

class InputPortOld
{
    public:
		InputPortOld();
        InputPortOld(Pin pin);
        void InitOld();
         
		Pin _Pin; // 针脚
   
        void On(); //打开中断
        void Off(); //关闭中断		
        void RegisterOld(BtnInCallbackOld handler); // 注册回调		
    private:
        void Exti0_state(bool onoff);
        uint eXTI_Line; //中断线
        byte nVIC_IRQChannel; //中断向量
        byte gPIO_PinSource; //中断线
        byte gPIO_PortSourceGPIO; //端口	
	public:        	
		void Write(const bool value);//写入值，true：高电平，false：低电平			
        void SetModeAIN();
        void SetModeIN_FLOATING();
        void SetModeINPUT_IPD();
        void SetModeINPUT_IPU();
        void SetModeOut_OD();
        void SetModeOut_PP();
        void SetModeAF_OD();
        void SetModeAF_PP();
        void Set(); //设置引脚
        void Reset(); //复位引脚        
        byte Read(void); 
		static bool ReadPinPort(Pin pin);//读取端口状态
    protected:         
        void SetMode(PIN_MODE mode); //引脚模式		
};
