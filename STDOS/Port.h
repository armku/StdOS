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
// 输入口
class InputPort: public Port
{
    public:
        //以下为2017-01-07
        typedef enum
        {
                NOPULL = 0x00, UP = 0x01,  //上拉电阻 
                DOWN = 0x02,  //下拉电阻 
        } PuPd;
        //enum class Trigger	//强类型枚举
        typedef enum  //2017-01-07
        {
                Rising = 0x01,  //上升沿
                Falling = 0x02,  //下降沿
                Both = 0x03  //上升下降沿
        } Trigger;
        // 读取委托
        typedef void(*IOReadHandler)(Pin port, bool down, void *param);
        ushort ShakeTime; //	=	0; 		// 抖动时间.毫秒
        ushort PressTime; //	=	0;		//长按时间。毫秒
        bool Invert; //	=	2; 		// 是否倒置输入输出。默认2表示自动检测		
        PuPd Pull; //	=	PuPd;//上拉下拉电阻
        Trigger Mode; //	=	Both;	//触发模式，上升沿下降沿
        bool HardEvent; //	=	false;	//是否使用硬件事件，默认false
        //Delegate2<InputPortNew&,bool> Press;//按下事件
        //以上为2017-01-07
        bool Floating; // 是否浮空输入
        InputPort();
        InputPort(Pin pin, bool floating = true, PuPd pupd = UP);
        virtual ~InputPort();
        ushort ReadGroup(); // 整组读取
        bool Read(); // 读取状态
        static bool Read(Pin pin); // 读取某个引脚
        void Register(IOReadHandler handler, void *param = NULL); // 注册事件
        operator bool();
    protected:
        // 函数命名为Init，而不作为构造函数，主要是因为用构造函数会导致再实例化一个对象，然后这个函数在那个新对象里面执行
        void Init(bool floating = true, PuPd pupd = UP);
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
    private:
        bool _Registed;
        void RegisterInput(int groupIndex, int pinIndex, IOReadHandler handler, void *param);
        void UnRegisterInput(int pinIndex);
};
//中断线打开、关闭
void SetEXIT(int pinIndex, bool enable);
