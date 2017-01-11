#pragma once 

#include "Sys.h"
#include "stm32f10x.h"

#define _force_inline __inline
#ifdef STM32F4
    #define GPIO_MAX_SPEED 100
#else 
    #define GPIO_MAX_SPEED 50
#endif 

// 端口基类
// 用于管理一个端口，通过PinBit标识该组的哪些引脚。
// 子类初始化时先通过SetPort设置端口，备份引脚状态，然后Config通过gpio结构体配置端口，端口销毁时恢复引脚状态
class Port
{
    public:
        GPIO_TypeDef *Group; // 针脚组
        Pin _Pin; // 针脚
        ushort PinBit; // 组内引脚位。每个引脚一个位

        Port &Set(Pin pin); // 设置引脚，并应用配置。
        bool Empty()const
        {
                return _Pin == P0;
        }

        virtual void Config(); // 确定配置,确认用对象内部的参数进行初始化

        // 辅助函数
        _force_inline static GPIO_TypeDef* IndexToGroup(byte index);
        _force_inline static byte GroupToIndex(GPIO_TypeDef* group);

        #if DEBUG
            static bool Reserve(Pin pin, bool flag); // 保护引脚，别的功能要使用时将会报错。返回是否保护成功
            static bool IsBusy(Pin pin); // 引脚是否被保护
        #endif 		
    protected:
        Port();
        virtual ~Port();

        // 配置过程，由Config调用，最后GPIO_Init
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
        #if DEBUG
            virtual bool OnReserve(Pin pin, bool flag);
        #endif 

    private:
        #if defined(STM32F1)
            ulong InitState; // 备份引脚初始状态，在析构时还原
        #endif 
};
