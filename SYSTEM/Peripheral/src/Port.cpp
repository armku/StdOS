#include <stdio.h>
#include "stm32f10x.h"
#include "Port.h"

#define NULL 0
#define GPIO_Mode_IN GPIO_Mode_IN_FLOATING
#define GPIO_Mode_AF GPIO_Mode_AF_OD
#define GPIO_OType_OD GPIO_Mode_Out_OD
#define GPIO_OType_PP GPIO_Mode_Out_PP
#define GPIO_Mode_OUT GPIO_Mode_Out_OD


// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _PORT(PIN) (1 << ((PIN) & (ushort)0x0F))
#define _PIN(PIN) (PIN & 0x0F)
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))


#if defined(STM32F1) || defined(STM32F4)
    static const int PORT_IRQns[] = 
    {
        EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5个基础的
        EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
        EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
    };
#elif defined(STM32F0)
    static const int PORT_IRQns[] = 
    {
        EXTI0_1_IRQn, EXTI0_1_IRQn,  // 基础
        EXTI2_3_IRQn, EXTI2_3_IRQn,  // 基础
        EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn,
            EXTI4_15_IRQn  // EXTI15_10
    };
#endif 
/*
端口基本功能
 */
Port::Port()
{
    _Pin = P0;
    Group = NULL;
    PinBit = 0;
}

Port::~Port()
{
    #if defined(STM32F1)
        // 恢复为初始化状态
        ushort bits = PinBit;
        int config = InitState &0xFFFFFFFF;
        for (int i = 0; i < 16 && bits; i++, bits >>= 1)
        {
            if (i == 7)
                config = InitState >> 32;
            if (bits &1)
            {
                uint shift = (i &7) << 2; // 每引脚4位
                uint mask = 0xF << shift; // 屏蔽掉其它位

                GPIO_TypeDef *port = Group;
                if (i &0x08)
                {
                    // bit 8 - 15
                    port->CRH = port->CRH &~mask | (config &mask);
                }
                else
                {
                    // bit 0-7
                    port->CRL = port->CRL &~mask | (config &mask);
                }
            }
        }
    #endif 
}

/*
单一引脚初始化
 */
Port &Port::Set(Pin pin)
{
    //assert_param(pin != P0);

    _Pin = pin;
    if (_Pin != P0)
    {
        Group = IndexToGroup(pin >> 4);
        PinBit = 1 << (pin &0x0F);
    }
    else
    {
        Group = NULL;
        PinBit = 0;
    }

    #if defined(STM32F1)
        // 整组引脚的初始状态，析构时有选择恢复
        if (_Pin != P0)
            InitState = ((ulong)Group->CRH << 32) + Group->CRL;
    #endif 

    if (_Pin != P0)
        Config();

    return  *this;
}

bool Port::Empty()const
{
    return _Pin == P0;
}

void Port::Config()
{
    GPIO_InitTypeDef gpio;
    // 特别要慎重，有些结构体成员可能因为没有初始化而酿成大错
    GPIO_StructInit(&gpio);

    OnConfig(gpio);
    GPIO_Init(Group, &gpio);
}

void Port::OnConfig(GPIO_InitTypeDef &gpio)
{
    // 打开时钟
    int gi = _Pin >> 4;
    #ifdef STM32F0
        RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOAEN << gi, ENABLE);
    #elif defined(STM32F1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << gi, ENABLE);
    #elif defined(STM32F4)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA << gi, ENABLE);
    #endif 

    gpio.GPIO_Pin = PinBit;

    #ifdef STM32F1
        // PA15/PB3/PB4 需要关闭JTAG
        switch (_Pin)
        {
            case PA15:
            case PB3:
            case PB4:
                {
                    debug_printf("Close JTAG for P%c%d\r\n", _PIN_NAME(_Pin));

                    // PA15是jtag接口中的一员 想要使用 必须开启remap
                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
                    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
                    break;
                }
        }
    #endif 
}

GPIO_TypeDef *Port::IndexToGroup(byte index)
{
    return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
}

byte Port::GroupToIndex(GPIO_TypeDef *group)
{
    return (byte)(((int)group - GPIOA_BASE) >> 10);
}

#define GPIO_Mode_IN GPIO_Mode_IN_FLOATING
#define GPIO_Mode_AF GPIO_Mode_AF_OD
#define GPIO_OType_OD GPIO_Mode_Out_OD
#define GPIO_OType_PP GPIO_Mode_Out_PP
#define GPIO_Mode_OUT GPIO_Mode_Out_OD


// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _PORT(PIN) (1 << ((PIN) & (ushort)0x0F))
#define _PIN(PIN) (PIN & 0x0F)
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

OutputPort::OutputPort()
{
    Init();
}

OutputPort::OutputPort(Pin pin, bool invert, bool openDrain, uint speed)
{
    Init(invert, openDrain, speed);
    Set(pin);
}

void OutputPort::OnConfig(GPIO_InitTypeDef &gpio)
{
    #ifndef STM32F4
        assert_param(Speed == 2 || Speed == 10 || Speed == 50);
    #else 
        assert_param(Speed == 2 || Speed == 25 || Speed == 50 || Speed == 100);
    #endif 

    Port::OnConfig(gpio);

    switch (Speed)
    {
        case 2:
            gpio.GPIO_Speed = GPIO_Speed_2MHz;
            break;
            #ifndef STM32F4
            case 10:
                gpio.GPIO_Speed = GPIO_Speed_10MHz;
                break;
            #else 
            case 25:
                gpio.GPIO_Speed = GPIO_Speed_25MHz;
                break;
            case 100:
                gpio.GPIO_Speed = GPIO_Speed_100MHz;
                break;
            #endif 
        case 50:
            gpio.GPIO_Speed = GPIO_Speed_50MHz;
            break;
    }

    #ifdef STM32F1
        gpio.GPIO_Mode = OpenDrain ? GPIO_Mode_Out_OD : GPIO_Mode_Out_PP;
    #else 
        gpio.GPIO_Mode = GPIO_Mode_OUT;
        gpio.GPIO_OType = OpenDrain ? GPIO_OType_OD : GPIO_OType_PP;
    #endif 

    // 配置之前，需要根据倒置情况来设定初始状态，也就是在打开端口之前必须明确端口高低状态
    ushort dat = GPIO_ReadOutputData(Group);
    if (!Invert)
        dat &= ~PinBit;
    else
        dat |= PinBit;
    GPIO_Write(Group, dat);
}

/*
整组读取
 */
ushort OutputPort::ReadGroup()
{
    return GPIO_ReadOutputData(Group);
}

bool OutputPort::Read()
{
    // 转为bool时会转为0/1
    bool rs = GPIO_ReadOutputData(Group) &PinBit;
    return rs ^ Invert;
}

bool OutputPort::ReadInput()
{
    bool rs = GPIO_ReadInputData(Group) &PinBit;
    return rs ^ Invert;
}

bool OutputPort::Read(Pin pin)
{
    GPIO_TypeDef *group = _GROUP(pin);
    return (group->IDR >> (pin &0xF)) &1;
}

void OutputPort::Write(bool value)
{
    if (value ^ Invert)
        GPIO_SetBits(Group, PinBit);
    else
        GPIO_ResetBits(Group, PinBit);
}

OutputPort &OutputPort::operator = (bool value)
{
    Write(value);
    return  *this;
}

OutputPort &OutputPort::operator = (OutputPort &port)
{
    Write(port.Read());
    return  *this;
}

OutputPort::operator bool()
{
    return Read();
}

void OutputPort::WriteGroup(ushort value)
{
    GPIO_Write(Group, value);
}

void OutputPort::Up(uint ms)
{
    Write(true);
    Sys.Sleep(ms);
    Write(false);
}

void OutputPort::Blink(uint times, uint ms)
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

/*
设置端口状态
 */
void OutputPort::Write(Pin pin, bool value)
{
    if (value)
        GPIO_SetBits(_GROUP(pin), _PORT(pin));
    else
        GPIO_ResetBits(_GROUP(pin), _PORT(pin));
}

void OutputPort::Init(bool invert, bool openDrain, uint speed)
{
    OpenDrain = openDrain;
    Speed = speed;
    Invert = invert;
}

PortScope::PortScope(OutputPort *port, bool value)
{
    _port = port;
    if (_port)
    {
        // 备份数值，析构的时候需要还原
        _value = port->Read();
        *_port = value;
    }
}

PortScope::~PortScope()
{
    if (_port)
        *_port = _value;
}

AlternatePort::AlternatePort(): OutputPort()
{
    Init(false, false);
}

AlternatePort::AlternatePort(Pin pin, bool invert, bool openDrain, uint speed): OutputPort(pin, invert, openDrain, speed)
{
    Init(invert, openDrain, speed);
    Set(pin);
}

void AlternatePort::OnConfig(GPIO_InitTypeDef &gpio)
{
    OutputPort::OnConfig(gpio);

    #ifdef STM32F1
        gpio.GPIO_Mode = OpenDrain ? GPIO_Mode_AF_OD : GPIO_Mode_AF_PP;
    #else 
        gpio.GPIO_Mode = GPIO_Mode_AF;
        gpio.GPIO_OType = OpenDrain ? GPIO_OType_OD : GPIO_OType_PP;
    #endif 
}

void AnalogInPort::OnConfig(GPIO_InitTypeDef &gpio)
{
    Port::OnConfig(gpio);

    #ifdef STM32F1
        gpio.GPIO_Mode = GPIO_Mode_AIN; //
    #else 
        gpio.GPIO_Mode = GPIO_Mode_AN;
        //gpio.GPIO_OType = !Floating ? GPIO_OType_OD : GPIO_OType_PP;
    #endif 
}

AnalogInPort::AnalogInPort(Pin pin)
{
    Set(pin);
}
