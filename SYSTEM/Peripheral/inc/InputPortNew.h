#pragma once

#include "Port.h"

// 输入口
class InputPortNew: public Port
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

        InputPortNew()
        {
                Init();
        }
        InputPortNew(Pin pin, bool floating = true, PuPd_TypeDef pupd = PuPd_UP)
        {
                Init(floating, pupd);
                Set(pin);
        }

        virtual ~InputPortNew();

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
