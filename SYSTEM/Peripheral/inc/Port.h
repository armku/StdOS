/*
端口基类
用于管理一个端口，通过PinBit标识该组的哪些引脚。
子类初始化时先通过SetPort设置端口，备份引脚状态，然后Config通过gpio结构体配置端口，端口销毁时恢复引脚状态
 */
#pragma once 

#include "Sys.h"
#include "stm32f10x.h"

#define _force_inline __inline
#ifdef STM32F4
    #define GPIO_MAX_SPEED 100
#else 
    #define GPIO_MAX_SPEED 50
#endif 

class Port
{
    public:
        GPIO_TypeDef *Group; // 针脚组
        Pin _Pin; // 针脚
        ushort PinBit; // 组内引脚位。每个引脚一个位

        Port &Set(Pin pin); // 设置引脚，并应用配置。
        bool Empty()const;
        virtual void Config(); // 确定配置,确认用对象内部的参数进行初始化
        // 辅助函数
        _force_inline static GPIO_TypeDef *IndexToGroup(byte index);
        _force_inline static byte GroupToIndex(GPIO_TypeDef *group);

    protected:
        Port();
        virtual ~Port();
        // 配置过程，由Config调用，最后GPIO_Init
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
    private:
        #if defined(STM32F1)
            ulong InitState; // 备份引脚初始状态，在析构时还原
        #endif 
};
/*
输出口
 */
class OutputPort: public Port
{
    public:
        bool OpenDrain; // 是否开漏输出
        bool Invert; // 是否倒置输入输出
        uint Speed; // 速度

        OutputPort();
        // 普通输出一般采用开漏输出，需要倒置
        OutputPort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED);

        // 整体写入所有包含的引脚
        void Write(bool value);
        void WriteGroup(ushort value); // 整组写入
        void Up(uint ms); // 拉高一段时间后拉低
        void Blink(uint times, uint ms); // 闪烁多次

        ushort ReadGroup(); // 整组读取
        // 读取指定索引引脚。索引按照从小到大，0xFF表示任意脚为true则返回true
        bool Read(byte index);
        bool Read(); // Read() ReadReal() 的区别在  前者读输出  一个读输入    在开漏输出的时候有很大区别
        bool ReadInput();

        static bool Read(Pin pin);
        static void Write(Pin pin, bool value);

        OutputPort &operator = (bool value);
        OutputPort &operator = (OutputPort &port);
        operator bool();

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
        void Init(bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED);
};
/*
输出端口会话类。初始化时打开端口，超出作用域析构时关闭。反向操作可配置端口为倒置
 */
class PortScope
{
    private:
        OutputPort *_port;
        bool _value;
    public:
        PortScope(OutputPort *port, bool value = true);

        ~PortScope();
};
/*
复用输出口
 */
class AlternatePort: public OutputPort
{
    public:
        AlternatePort();
        // 复用输出一般采用推挽输出，不需要倒置
        AlternatePort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED);
    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};
/*
模拟输入输出口
 */
class AnalogInPort: public Port
{
    public:
        AnalogInPort(Pin pin);

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};
