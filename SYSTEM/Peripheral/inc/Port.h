#pragma once 

#include "Sys.h"
#include "stm32f10x.h"

#define _force_inline 
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




#if 0

// 输入口
class InputPort: public Port
{
    public:
        typedef enum
        {
                PuPd_NOPULL = 0x00, PuPd_UP = 0x01, PuPd_DOWN = 0x02
        } PuPd_TypeDef;

        // 读取委托
        typedef void(*IOReadHandler)(Pin pin, bool down, void *param);

        uint ShakeTime; // 抖动时间
        PuPd_TypeDef PuPd; // 上拉下拉电阻
        bool Floating; // 是否浮空输入
        bool Invert; // 是否倒置输入输出

        InputPort()
        {
                Init();
        }
        InputPort(Pin pin, bool floating = true, PuPd_TypeDef pupd = PuPd_UP)
        {
                Init(floating, pupd);
                Set(pin);
        }

        virtual ~InputPort();

        ushort ReadGroup(); // 整组读取
        bool Read(); // 读取状态
        static bool Read(Pin pin); // 读取某个引脚

        void Register(IOReadHandler handler, void *param = NULL); // 注册事件

        operator bool()
        {
                return Read();
        }

    protected:
        // 函数命名为Init，而不作为构造函数，主要是因为用构造函数会导致再实例化一个对象，然后这个函数在那个新对象里面执行
        void Init(bool floating = true, PuPd_TypeDef pupd = PuPd_UP)
        {
                PuPd = pupd;
                Floating = floating;

                _Registed = false;
                //ShakeTime = 20;
                // 有些应用的输入口需要极高的灵敏度，这个时候不需要抖动检测
                ShakeTime = 0;
                Invert = false;
        }

        virtual void OnConfig(GPIO_InitTypeDef &gpio);

        #if DEBUG
            virtual bool OnReserve(Pin pin, bool flag);
        #endif 

    private:
        bool _Registed;

        void RegisterInput(int groupIndex, int pinIndex, IOReadHandler handler, void *param);
        void UnRegisterInput(int pinIndex);
};
#endif
// 模拟输入输出口
class AnalogInPort: public Port
{
    public:
        AnalogInPort(Pin pin)
        {
                Set(pin);
        }

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};



