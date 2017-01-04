#pragma once 

#include "Sys.h"
#include "stm32f10x.h"

#ifdef STM32F4
    #define GPIO_MAX_SPEED 100
#else 
    #define GPIO_MAX_SPEED 50
#endif 



class Port
{
	public:
		Pin _Pin; //引脚
		ushort  PinBit; // 组内引脚位。每个引脚一个位
		Port& Set(Pin pin); // 设置引脚，并应用配置。
		bool Empty() const { return _Pin == P0; }
		virtual void Config(); // 确定配置,确认用对象内部的参数进行初始化
		// 配置过程，由Config调用，最后GPIO_Init
		//virtual void OnConfig(GPIO_InitTypeDef& gpio);
	protected:
        Port();
        virtual ~Port();
};
// 输出口
class OutputPort: public Port
{
    public:
        bool OpenDrain; // 是否开漏输出
        bool Invert; // 是否倒置输入输出
        uint Speed; // 速度

        OutputPort()
        {
                Init();
        }
        // 普通输出一般采用开漏输出，需要倒置
        OutputPort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED)
        {
                Init(invert, openDrain, speed);
                Set(pin);
        }

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

        OutputPort &operator = (bool value)
        {
                Write(value);
                return  *this;
        }
        OutputPort &operator = (OutputPort &port)
        {
                Write(port.Read());
                return  *this;
        }
        operator bool()
        {
                return Read();
        }

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);

        void Init(bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED)
        {
                OpenDrain = openDrain;
                Speed = speed;
                Invert = invert;
        }

        #if DEBUG
            virtual bool OnReserve(Pin pin, bool flag);
        #endif 
};
