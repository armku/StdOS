#include "Port.h"

int IndexToGroup(int a1)
{
  return (a1 << 10) + 1073809408;
}

/* 中断状态结构体 */
/* 一共16条中断线，意味着同一条线每一组只能有一个引脚使用中断 */
typedef struct TIntState
{
    Pin Pin;
    InputPort inputport;
    //InputPort::IOReadHandler Handler; // 委托事件
	Delegate2<InputPort&, bool>	Press;	// 按下事件
    void *Param; // 事件参数，一般用来作为事件挂载者的对象，然后借助静态方法调用成员方法	
    bool OldValue;

    uint ShakeTime; // 抖动时间	
    int Used; // 被使用次数。对于前5行中断来说，这个只会是1，对于后面的中断线来说，可能多个
} IntState;


// 16条中断线
static IntState InterruptState[16];
static bool hasInitState = false;
//InputPort *
void GPIO_ISR(int num) // 0 <= num <= 15
{
	
	if (!hasInitState)
    {
//        return ;
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
    if (!state3->Press)
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
    
	if (state3->Press)
    {
        // 新值value为true，说明是上升，第二个参数是down，所以取非
        state3->Press((state3->inputport), value);
    }
}

void Port::OnClose(){}

void Port::Opening()
{
	//GPIO_StructInit();
}
bool InputPort::OnRegister()
{
	if(this->Press)
	{
		IntState *state=new IntState();
		state->Press=this->Press;
		state->inputport._Pin=this->_Pin;
		InterruptState[this->_Pin&0x0f]=*state;
	}
	return true;
}
void InputPort::ClosePin()
{
	
}
bool OutputPort::Read()const
{
    return this->Invert ? !Port::Read(): Port::Read();
}
