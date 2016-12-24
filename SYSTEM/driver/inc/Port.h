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
        void Set(); //��������
        void Reset(); //��λ����
        void Toggle(); //��ת����
        uint8_t Read(void);
        void Write(uint8_t da);
    private:
        uint16_t pin; //���ź�
        uint8_t pinbit;
        void SetMode(PIN_MODE mode); //����ģʽ
};
