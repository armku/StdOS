#pragma once

#include "Port.h"

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
