/*
SmartOS v2.9的看门狗驱动，兼容STM32F0/F1/F4/GD32F10x/GD32F1x0
 */
#pragma once 

#include "Type.h"

// 看门狗
class WatchDog
{
    public:
        WatchDog(uint ms = 3000);
        ~WatchDog();
        bool Config(uint ms); // 配置看门狗喂狗重置时间，超过该时间讲重启MCU
        void ConfigMax(); // 看门狗无法关闭，只能设置一个最大值
        void Feed(); // 喂狗
	public:
		uint Timeout; // 当前超时时间
};

// 看门狗
class CWatchDog
{
    public:
        CWatchDog(uint ms = 3000);
        ~CWatchDog();
    public:
        byte Init();
        void Feed(); // 喂狗
    private:
        uint timeout; // 当前超时时间
};
