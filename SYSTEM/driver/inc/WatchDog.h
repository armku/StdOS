#pragma once 
#include "BasePort.h"

// ���Ź�
class CWatchDog
{
    public:
        CWatchDog(uint32_t ms = 3000);
        ~CWatchDog();
    public:
        byte Init();
        void Feed(); // ι��
    private:
        uint32_t timeout; // ��ǰ��ʱʱ��
};
