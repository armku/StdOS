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
        this->Close();
    }
#endif 

/*
单一引脚初始化
 */
Port &Port::Set(Pin pin)
{
    this->_Pin = pin;    

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
String Port::ToString()const
{
    String ret;
    if (this->_Pin == P0)
        ret += "P0";
    else
    {
        ret += this->_Pin >> 4+'A';
        ret += ret.Concat(this->_Pin &0x0F, 10);
    }

    return ret;
}

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
	if(this->Empty())
		return false;
	else
		return this->Invert ? !Port::Read(): Port::Read();
}

void OutputPort::Up(int ms)const
{
	if(!this->Empty())
	{
		this->Write(true);
		Sys.Sleep(ms);
		this->Write(false);
	}
}

void OutputPort::Down(int ms)const
{
	if(!this->Empty())
	{
		this->Write(false);
		Sys.Sleep(ms);
		this->Write(true);
	}
}

void OutputPort::Blink(int times, int ms)const
{
	if(!this->Empty())
	{
		bool flag = true;
		for (int i = 0; i < times; i++)
		{
			this->Write(flag);
			flag = !flag;
			Sys.Sleep(ms);
		}
		Write(false);
	}
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
	this->Open();
}

InputPort::InputPort(){

}
void InputPort::InputNoIRQTask(void* param)
{
	InputPort *ip=(InputPort*)param;
	ip->OnPress(ip->Opened);
}
bool InputPort::UsePress()
{
	if(this->_Pin==P0)
	{
		//assert_failed2((const char *)"%s,%d", __FILE__, 0x12);
	}
	this->HardEvent=this->OnRegister();
	if(!this->Opened &&!this->Floating)
	{
		if(this->HardEvent)
			this->Opened	=	Sys.AddTask(InputPort::InputTask,this,-1,-1,"InputTask");
		else
			this->Opened	=	Sys.AddTask(InputPort::InputNoIRQTask,this,100,100,"InputNoIRQTask");
	}
	
	
    return true;
}

void InputPort::OnClose()
{
	this->OnClose();
	this->ClosePin();
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
	//Sys.RemoveTask(InputPort::InputTask);
}

// 读取本组所有引脚，任意脚为true则返回true，主要为单一引脚服务
bool InputPort::Read()const
{
    return this->Invert ? !Port::Read(): Port::Read();
}
void InputPort::OnPress(bool down)
{
	
}
void InputPort::InputTask(void* param)
{
	
}
InputPort& InputPort::Init(Pin pin, bool invert)
{
	this->Set(pin);
	this->Invert=invert;
	return *this;
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
