#pragma once 
#include "Port.h"

// ���Ź�
class CWatchDog
{
    public:
        CWatchDog(uint ms = 3000);       
    public:
        byte Init();
        void Feed(); // ι��
    private:
        uint timeout; // ��ǰ��ʱʱ��
};
