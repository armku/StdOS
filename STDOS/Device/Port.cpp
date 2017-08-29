#include "Port.h"

Port::Port()
{
    this->_Pin = P0;
    this->State = NULL;
    //this->PinBit = 0;
}

#ifndef TINY	
    Port::~Port()
    {
        #ifdef STM32F0
        #elif defined STM32F1
            // 恢复为初始化状态
            //            ushort bits = PinBit;
            //            int config = InitState &0xFFFFFFFF;
            //            for (int i = 0; i < 16 && bits; i++, bits >>= 1)
            //            {
            //                if (i == 7)
            //                    config = InitState >> 32;
            //                if (bits &1)
            //                {
            //                    uint shift = (i &7) << 2; // 每引脚4位
            //                    uint mask = 0xF << shift; // 屏蔽掉其它位

            //                    GPIO_TypeDef *port = (GPIO_TypeDef *)this->State;
            //                    if (i &0x08)
            //                    {
            //                        // bit 8 - 15
            //                        port->CRH = port->CRH &~mask | (config &mask);
            //                    }
            //                    else
            //                    {
            //                        // bit 0-7
            //                        port->CRL = port->CRL &~mask | (config &mask);
            //                    }
            //                }
            //            }
        #elif defined STM32F4
        #endif 
    }
#endif 

/*
单一引脚初始化
 */
Port &Port::Set(Pin pin)
{
    //assert_param(pin != P0);

    this->_Pin = pin;
    //    if (_Pin != P0)
    //    {
    //        this->State = IndexToGroup(pin >> 4);
    //        //        PinBit = 1 << (pin &0x0F);
    //    }
    //    else
    //    {
    //        this->State = NULL;
    //        //        PinBit = 0;
    //    }
    #ifdef STM32F0
    #elif defined STM32F1
        // 整组引脚的初始状态，析构时有选择恢复
        //        if (_Pin != P0)
        //            InitState = ((UInt64)((GPIO_TypeDef *)this->State)->CRH << 32) + ((GPIO_TypeDef *)this->State)->CRL;
    #elif defined STM32F4
    #endif 


    this->Open();
    return  *this;
}

bool Port::Empty()const
{
    return _Pin == P0;
}

void Port::Close()
{
    this->OnClose();
    this->Opened = false;
}

void Port::Clear(){}

OutputPort::OutputPort(){}
OutputPort::OutputPort(Pin pin)
{
    this->Opened = false;
    Set(pin);
    this->Write(0);
}

OutputPort::OutputPort(Pin pin, byte invert, bool openDrain, byte speed)
{
    this->Opened = false;
    this->Invert = invert;
    this->OpenDrain = openDrain;
    Set(pin);
    this->Write(0);
}

bool OutputPort::ReadInput()const
{
    return this->Invert ? !Port::Read(): Port::Read();
}

void OutputPort::Up(int ms)const
{
    Write(true);
    Sys.Sleep(ms);
    Write(false);
}

void OutputPort::Down(int ms)const
{
    Write(false);
    Sys.Sleep(ms);
    Write(true);
}

void OutputPort::Blink(int times, int ms)const
{
    bool flag = true;
    for (int i = 0; i < times; i++)
    {
        Write(flag);
        flag = !flag;
        Sys.Sleep(ms);
    }
    Write(false);
}

void OutputPort::OnOpen(void *param)
{
    Port::OnOpen(param);
    this->OpenPin(param);
}

AlternatePort::AlternatePort(): OutputPort()
{
    this->Opened = false;
}

AlternatePort::AlternatePort(Pin pin): OutputPort(pin)
{
    this->Opened = false;
    this->OpenDrain = false;
}

AlternatePort::AlternatePort(Pin pin, byte invert, bool openDrain, byte speed)
{
    //局部变量中，数据值不确定
    this->Opened = false;
    this->Invert = invert;
    this->OpenDrain = openDrain;
    Set(pin);
}

InputPort::InputPort(Pin pin, bool floating, PuPd pupd)
{
    this->Opened = false;
    this->Floating = floating;
    this->Pull = pupd;
    Set(pin);
}

InputPort::InputPort(){

}

bool InputPort::UsePress()
{
    return false;
}

void InputPort::OnClose(){

}

/* 中断状态结构体 */
/* 一共16条中断线，意味着同一条线每一组只能有一个引脚使用中断 */
typedef struct TIntState
{
    Pin Pin;
    InputPort inputport;
    InputPort::IOReadHandler Handler; // 委托事件
    void *Param; // 事件参数，一般用来作为事件挂载者的对象，然后借助静态方法调用成员方法	
    bool OldValue;

    uint ShakeTime; // 抖动时间	
    int Used; // 被使用次数。对于前5行中断来说，这个只会是1，对于后面的中断线来说，可能多个
} IntState;

// 16条中断线
static IntState InterruptState[16];
static bool hasInitState = false;

InputPort::~InputPort()
{
    // 取消所有中断
    //    if (_Registed)
    {
        //       Register(NULL);
    }
}

// 读取本组所有引脚，任意脚为true则返回true，主要为单一引脚服务
bool InputPort::Read()const
{
    return this->Invert ? !Port::Read(): Port::Read();
}

void GPIO_ISR(int num) // 0 <= num <= 15
{
    if (!hasInitState)
    {
        return ;
    }
    IntState *state3 = InterruptState + num;
    if (!state3)
    {
        return ;
    }
    #if 0
        uint bit = 1 << num;
    #endif 
    bool value;
    //    value = InputPort::Read(state3->Pin);
    //byte line = EXTI_Line0 << num;
    // 如果未指定委托，则不处理
    if (!state3->Handler)
    {
        return ;
    }
    #if 0
        // 默认20us抖动时间
        uint shakeTime = state->ShakeTime;
    #endif 
    #if 0
        do
        {
            EXTI->PR = bit; // 重置挂起位
            value = InputPort::Read(state->Pin); // 获取引脚状态
            if (shakeTime > 0)
            {
                // 值必须有变动才触发
                if (value == state->OldValue)
                {
                    return ;
                }
                Time.Sleep(shakeTime); // 避免抖动
            }
        }
        while (EXTI->PR &bit); // 如果再次挂起则重复
    #endif 
    //EXTI_ClearITPendingBit(line);
    #if 0
        // 值必须有变动才触发
        if (shakeTime > 0 && value == state->OldValue)
            return ;
        state->OldValue = value;
    #endif 
    if (state3->Handler)
    {
        // 新值value为true，说明是上升，第二个参数是down，所以取非
        state3->Handler(&(state3->inputport), value, state3->Param);
    }
}
