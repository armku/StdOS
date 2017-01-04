/*
SmartOS v2.9�Ŀ��Ź�����������STM32F0/F1/F4/GD32F10x/GD32F1x0
 */
#pragma once 

#include "Type.h"

// ���Ź�
class WatchDog
{
    public:
        WatchDog(uint ms = 3000);
        ~WatchDog();
        bool Config(uint ms); // ���ÿ��Ź�ι������ʱ�䣬������ʱ�佲����MCU
        void ConfigMax(); // ���Ź��޷��رգ�ֻ������һ�����ֵ
        void Feed(); // ι��
	public:
		uint Timeout; // ��ǰ��ʱʱ��
};

// ���Ź�
class CWatchDog
{
    public:
        CWatchDog(uint ms = 3000);
        ~CWatchDog();
    public:
        byte Init();
        void Feed(); // ι��
    private:
        uint timeout; // ��ǰ��ʱʱ��
};
