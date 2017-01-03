#include "stm32f10x.h"
#include "Port.h"

// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _PORT(PIN) (1 << ((PIN) & (ushort)0x0F))
#define _PIN(PIN) (PIN & 0x0F)
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

#if defined(STM32F1) || defined(STM32F4)
static const int PORT_IRQns[] = {
    EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn, // 5个基础的
    EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,    // EXTI9_5
    EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn   // EXTI15_10
};
#elif defined(STM32F0)
static const int PORT_IRQns[] = {
    EXTI0_1_IRQn, EXTI0_1_IRQn, // 基础
    EXTI2_3_IRQn, EXTI2_3_IRQn, // 基础
    EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn,
    EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn   // EXTI15_10
};
#endif

// 端口基本功能
#define REGION_Port 1
#define NULL 0
#ifdef REGION_Port
Port::Port()
{
        _Pin = P0;
        Group = NULL;
        PinBit = 0;
}
#else
Port:Port()
{
#if defined(STM32F1)
        // 恢复为初始化状态
        ushort bits = PinBit;
        int config = InitState & 0xFFFFFFFF;
        for(int i=0; i<16 && bits; i++, bits>>=1)
        {
                if(i == 7) config = InitState >> 32;
                if(bits & 1)
                {
                        uint shift = (i & 7) << 2; // 每引脚4位
                        uint mask = 0xF << shift;  // 屏蔽掉其它位

                        GPIO_TypeDef* port = Group;
                        if (i & 0x08) { // bit 8 - 15
                                port->CRH = port->CRH & ~mask | (config & mask);
                        } else { // bit 0-7
                                port->CRL = port->CRL & ~mask | (config & mask);
                        }
                }
        }
#endif

#if DEBUG
        // 解除保护引脚
        OnReserve(_Pin, false);
#endif
}

// 单一引脚初始化
Port& Port::Set(Pin pin)
{
        //assert_param(pin != P0);

#if DEBUG
        if(_Pin != P0) OnReserve(_Pin, false);
#endif

    _Pin = pin;
        if(_Pin != P0)
        {
                Group = IndexToGroup(pin >> 4);
                PinBit = 1 << (pin & 0x0F);
        }
        else
        {
                Group = NULL;
                PinBit = 0;
        }

#if defined(STM32F1)
        // 整组引脚的初始状态，析构时有选择恢复
        if(_Pin != P0) InitState = ((ulong)Group->CRH << 32) + Group->CRL;
#endif

#if DEBUG
        // 保护引脚
        if(_Pin != P0) OnReserve(_Pin, true);
#endif

        if(_Pin != P0) Config();

        return *this;
}

void Port::Config()
{
        GPIO_InitTypeDef gpio;
        // 特别要慎重，有些结构体成员可能因为没有初始化而酿成大错
        GPIO_StructInit(&gpio);

    OnConfig(gpio);
    GPIO_Init(Group, &gpio);
}

void Port::OnConfig(GPIO_InitTypeDef& gpio)
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
        switch(_Pin)
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

GPIO_TypeDef* Port::IndexToGroup(byte index) { return ((GPIO_TypeDef *) (GPIOA_BASE + (index << 10))); }
byte Port::GroupToIndex(GPIO_TypeDef* group) { return (byte)(((int)group - GPIOA_BASE) >> 10); }
#endif

// 端口引脚保护
#if DEBUG
static ushort Reserved[8];                // 引脚保留位，记录每个引脚是否已经被保留，禁止别的模块使用

// 保护引脚，别的功能要使用时将会报错。返回是否保护成功
bool Port::Reserve(Pin pin, bool flag)
{
    int port = pin >> 4, bit = 1 << (pin & 0x0F);
    if (flag) {
        if (Reserved[port] & bit) {
                        // 增加针脚已经被保护的提示，很多地方调用ReservePin而不写日志，得到False后直接抛异常
                        debug_printf("ReservePin P%c%d already reserved\r\n", _PIN_NAME(pin));
                        return false; // already reserved
                }
        Reserved[port] |= bit;

                debug_printf("ReservePin P%c%d\r\n", _PIN_NAME(pin));
    } else {
        Reserved[port] &= ~bit;

#if defined(STM32F1)
                int config = 0;
                uint shift = (pin & 7) << 2; // 4 bits / pin
                uint mask = 0xF << shift; // 屏蔽掉其它位
                GPIO_TypeDef* port2 = IndexToGroup(port); // pointer to the actual port registers
                if (pin & 0x08) { // bit 8 - 15
                        config = port2->CRH & mask;
                } else { // bit 0-7
                        config = port2->CRL & mask;
                }

                config >>= shift;        // 移位到最右边
                config &= 0xF;
                debug_printf("UnReservePin P%c%d Config=0x%02x\r\n", _PIN_NAME(pin), config);
#else
                debug_printf("UnReservePin P%c%d\r\n", _PIN_NAME(pin));
#endif
        }

    return true;
}

bool Port::OnReserve(Pin pin, bool flag)
{
        return Reserve(pin, flag);
}

// 引脚是否被保护
bool Port::IsBusy(Pin pin)
{
    int port = pin >> 4, sh = pin & 0x0F;
    return (Reserved[port] >> sh) & 1;
}
#endif





























//以上为新版

Port::Port(Pin pin)
{
    this->pin = pin;
}

//写入值，true：高电平，false：低电平
void Port::Write(const bool value)
{
	if(value)
	{
		this->Set();
	}
	else
	{
		this->Reset();
	}
}
//写入值，true:高电平 false:低电平
void Port::operator = (const bool value)
{
	this->Write(value);
}
//写入值，其他:高电平 0:低电平
void Port::operator = (const byte value)
{
	this->Write((bool)value);
}
////引脚模式
void Port::SetMode(PIN_MODE mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(_RCC_APB2(pin), ENABLE);

    switch (mode)
    {
        case AIN:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
            break;
        case INPUT:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            break;
        case INPUT_PD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
            break;
        case INPUT_PU:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
            break;
        case OUTPUT_OD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
            break;
        case OUTPUT_PP:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            break;
        case AF_OD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
            break;
        case AF_PP:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
            break;
        default:
            break;
    }
    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = _PORT(this->pin);

    GPIO_Init(_GROUP(pin), &GPIO_InitStructure);
}

void Port::SetModeAIN()
{
    this->SetMode(AIN);
}

void Port::SetModeIN_FLOATING()
{
    this->SetMode(INPUT);
}

void Port::SetModeINPUT_IPD()
{
    this->SetMode(INPUT_PD);
}

void Port::SetModeINPUT_IPU()
{
    this->SetMode(INPUT_PU);
}

void Port::SetModeOut_OD()
{
    this->SetMode(OUTPUT_OD);
}

void Port::SetModeOut_PP()
{
    this->SetMode(OUTPUT_PP);
}

void Port::SetModeAF_OD()
{
    this->SetMode(AF_OD);
}

void Port::SetModeAF_PP()
{
    this->SetMode(AF_PP);
}

void Port::Set()
{
    this->pinbit = 1;
    GPIO_SetBits(_GROUP(pin), _PORT(pin));
}

void Port::Reset()
{
    this->pinbit = 0;
    GPIO_ResetBits(_GROUP(pin), _PORT(pin));
}

byte Port::Read(void)
{
    return ReadPinPort(this->pin);
}
//读取端口状态
bool Port::ReadPinPort(Pin pin)
{
	 return GPIO_ReadInputDataBit(_GROUP(pin), _PORT(pin));
}
