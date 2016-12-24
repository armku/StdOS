#pragma once 

#include <stdint.h>
#include "common.h"

class CPort
{
    public:
        CPort(PinPort pin);
        ~CPort();
    public:
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
        void Toggle(); //翻转引脚
        uint8_t Read(void);
        void Write(uint8_t da);
    private:
        uint16_t pin; //引脚号
        uint8_t pinbit;
        void SetMode(PIN_MODE mode); //引脚模式
};
