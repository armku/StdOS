#pragma once

#include "BasePort.h"

class CLed:public BasePort
{
    public:
        CLed(PinPort pin, ushort period1 = 1000, ushort onTime1 = 500);       
	
        byte Init();
        void On();
        void Off(); 
    private:
        ushort period; //周期
        ushort onTime; //灯亮时间
};
