#include <stdio.h>
#include "InputPortNew.h"

// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _PORT(PIN) (1 << ((PIN) & (ushort)0x0F))
#define _PIN(PIN) (PIN & 0x0F)
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

void InputPortNew::OnConfig(GPIO_InitTypeDef &gpio)
{
    Port::OnConfig(gpio);

    #ifdef STM32F1
        if (Floating)
            gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        else if (Pull == UP)
            gpio.GPIO_Mode = GPIO_Mode_IPU;
        else if (Pull == DOWN)
            gpio.GPIO_Mode = GPIO_Mode_IPD;
        // 这里很不确定，需要根据实际进行调整
    #else 
        gpio.GPIO_Mode = GPIO_Mode_IN;
        //gpio.GPIO_OType = !Floating ? GPIO_OType_OD : GPIO_OType_PP;
    #endif 
}


/* 中断状态结构体 */
/* 一共16条中断线，意味着同一条线每一组只能有一个引脚使用中断 */
typedef struct TIntState
{
    Pin Pin;
    InputPortNew::IOReadHandler Handler; // 委托事件
    void *Param; // 事件参数，一般用来作为事件挂载者的对象，然后借助静态方法调用成员方法
    bool OldValue;

    uint ShakeTime; // 抖动时间
    int Used; // 被使用次数。对于前5行中断来说，这个只会是1，对于后面的中断线来说，可能多个
} IntState;

// 16条中断线
static IntState State[16];
static bool hasInitState = false;

void RegisterInput(int groupIndex, int pinIndex, InputPortNew::IOReadHandler handler);
void UnRegisterInput(int pinIndex);

InputPortNew::~InputPortNew()
{
    // 取消所有中断
    if (_Registed)
        Register(NULL);
}

ushort InputPortNew::ReadGroup() // 整组读取
{
    return GPIO_ReadInputData(Group);
}

// 读取本组所有引脚，任意脚为true则返回true，主要为单一引脚服务
bool InputPortNew::Read()
{
    // 转为bool时会转为0/1
    bool rs = GPIO_ReadInputData(Group) &PinBit;
    return rs ^ Invert;
}

bool InputPortNew::Read(Pin pin)
{
    GPIO_TypeDef *group = _GROUP(pin);
    return (group->IDR >> (pin &0xF)) &1;
}


// 注册回调  及中断使能
void InputPortNew::Register(IOReadHandler handler, void *param)
{
    if (!PinBit)
        return ;

    // 检查并初始化中断线数组
    if (!hasInitState)
    {
        for (int i = 0; i < 16; i++)
        {
            IntState *state = &State[i];
            state->Pin = P0;
            state->Handler = NULL;
            state->Used = 0;
        }
        hasInitState = true;
    }

    byte gi = _Pin >> 4;
    ushort n = PinBit;
    for (int i = 0; i < 16 && n != 0; i++)
    {
        // 如果设置了这一位，则注册事件
        if (n &0x01)
        {
            // 注册中断事件
            if (handler)
            {
                IntState *state = &State[i];
                state->ShakeTime = ShakeTime;
                #if 0
                    RegisterInput(gi, i, handler, param);
                #endif 
            }
            else
            {
                #if 0
                    UnRegisterInput(i);
                #endif 
            }
        }
        n >>= 1;
    }

    _Registed = handler != NULL;
}

#define IT 1
#ifdef IT
    #if 0
        void GPIO_ISR(int num) // 0 <= num <= 15
        {
            if (!hasInitState)
                return ;

            IntState *state = State + num;
            if (!state)
                return ;

            uint bit = 1 << num;
            bool value;
            //byte line = EXTI_Line0 << num;
            // 如果未指定委托，则不处理
            if (!state->Handler)
                return ;

            // 默认20us抖动时间
            uint shakeTime = state->ShakeTime;

            do
            {
                EXTI->PR = bit; // 重置挂起位
                value = InputPortNew::Read(state->Pin); // 获取引脚状态
                if (shakeTime > 0)
                {
                    // 值必须有变动才触发
                    if (value == state->OldValue)
                        return ;

                    Time.Sleep(shakeTime); // 避免抖动
                }
            }
            while (EXTI->PR &bit); // 如果再次挂起则重复
            //EXTI_ClearITPendingBit(line);
            // 值必须有变动才触发
            if (shakeTime > 0 && value == state->OldValue)
                return ;
            state->OldValue = value;
            if (state->Handler)
            {
                // 新值value为true，说明是上升，第二个参数是down，所以取非
                state->Handler(state->Pin, !value, state->Param);
            }
        }
    #endif 
    #if 0
        void EXTI_IRQHandler(ushort num, void *param)
        {
            #if defined(STM32F1) || defined(STM32F4)
                // EXTI0 - EXTI4
                if (num <= EXTI4_IRQn)
                    GPIO_ISR(num - EXTI0_IRQn);
                else if (num == EXTI9_5_IRQn)
                {
                    // EXTI5 - EXTI9
                    uint pending = EXTI->PR &EXTI->IMR &0x03E0; // pending bits 5..9
                    int num = 5;
                    pending >>= 5;
                    do
                    {
                        if (pending &1)
                            GPIO_ISR(num);
                        num++;
                        pending >>= 1;
                    }
                    while (pending);
                }
                else if (num == EXTI15_10_IRQn)
                {
                    // EXTI10 - EXTI15
                    uint pending = EXTI->PR &EXTI->IMR &0xFC00; // pending bits 10..15
                    int num = 10;
                    pending >>= 10;
                    do
                    {
                        if (pending &1)
                            GPIO_ISR(num);
                        num++;
                        pending >>= 1;
                    }
                    while (pending);
                }
            #elif defined(STM32F0)
                switch (num)
                {
                    case EXTI0_1_IRQn:
                        {
                            uint pending = EXTI->PR &EXTI->IMR &0x0003; // pending bits 0..1
                            int num = 0;
                            pending >>= 0;
                            do
                            {
                                if (pending &1)
                                    GPIO_ISR(num);
                                num++;
                                pending >>= 1;
                            }
                            while (pending);
                            break;
                        }
                    case EXTI2_3_IRQn:
                        {
                            uint pending = EXTI->PR &EXTI->IMR &0x000c; // pending bits 3..2
                            int num = 2;
                            pending >>= 2;
                            do
                            {
                                if (pending &1)
                                    GPIO_ISR(num);
                                num++;
                                pending >>= 1;
                            }
                            while (pending);
                        }
                    case EXTI4_15_IRQn:
                        {
                            uint pending = EXTI->PR &EXTI->IMR &0xFFF0; // pending bits 4..15
                            int num = 4;
                            pending >>= 4;
                            do
                            {
                                if (pending &1)
                                    GPIO_ISR(num);
                                num++;
                                pending >>= 1;
                            }
                            while (pending);
                        }
                }
            #endif 
        }
    #endif 
#endif 

void SetEXIT(int pinIndex, bool enable)
{
    /* 配置EXTI中断线 */
    EXTI_InitTypeDef ext;
    EXTI_StructInit(&ext);
    ext.EXTI_Line = EXTI_Line0 << pinIndex;
    ext.EXTI_Mode = EXTI_Mode_Interrupt;
    ext.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 上升沿下降沿触发
    ext.EXTI_LineCmd = enable ? ENABLE : DISABLE;
    EXTI_Init(&ext);
}

#if 0
    // 申请引脚中断托管
    void InputPortNew::RegisterInput(int groupIndex, int pinIndex, IOReadHandler handler, void *param)
    {
        IntState *state = &State[pinIndex];
        Pin pin = (Pin)((groupIndex << 4) + pinIndex);
        // 检查是否已经注册到别的引脚上
        if (state->Pin != pin && state->Pin != P0)
        {
            #if DEBUG
                debug_printf("EXTI%d can't register to P%c%d, it has register to P%c%d\r\n", groupIndex, _PIN_NAME(pin), _PIN_NAME(state->Pin));
            #endif 
            return ;
        }
        state->Pin = pin;
        state->Handler = handler;
        state->Param = param;
        state->OldValue = Read(pin); // 预先保存当前状态值，后面跳变时触发中断

        // 打开时钟，选择端口作为端口EXTI时钟线
        #if defined(STM32F0) || defined(STM32F4)
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
            SYSCFG_EXTILineConfig(groupIndex, pinIndex);
        #elif defined(STM32F1)
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
            GPIO_EXTILineConfig(groupIndex, pinIndex);
        #endif 

        SetEXIT(pinIndex, true);

        // 打开并设置EXTI中断为低优先级
        Interrupt.SetPriority(PORT_IRQns[pinIndex], 1);

        state->Used++;
        if (state->Used == 1)
        {
            Interrupt.Activate(PORT_IRQns[pinIndex], EXTI_IRQHandler, this);
        }
    }
#endif 
#if 0
    void InputPortNew::UnRegisterInput(int pinIndex)
    {
        IntState *state = &State[pinIndex];
        // 取消注册
        state->Pin = P0;
        state->Handler = 0;

        SetEXIT(pinIndex, false);

        state->Used--;
        if (state->Used == 0)
        {
            Interrupt.Deactivate(PORT_IRQns[pinIndex]);
        }
    }
#endif
// 端口引脚保护
#if DEBUG    
    bool InputPortNew::OnReserve(Pin pin, bool flag)
    {
        debug_printf("Input::");

        return Port::OnReserve(pin, flag);
    }
#endif
