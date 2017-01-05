#include "OutputPort.h"

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

ushort OutputPort::ReadGroup() // 整组读取
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

// 设置端口状态
void OutputPort::Write(Pin pin, bool value)
{
    if (value)
        GPIO_SetBits(_GROUP(pin), _PORT(pin));
    else
        GPIO_ResetBits(_GROUP(pin), _PORT(pin));
}
// 端口引脚保护
#if DEBUG    
    bool OutputPort::OnReserve(Pin pin, bool flag)
    {
        debug_printf("Output::");

        return Port::OnReserve(pin, flag);
    }
#endif
