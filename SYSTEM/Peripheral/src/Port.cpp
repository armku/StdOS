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

#if 0
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
#endif 

// 端口基本功能
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

// 单一引脚初始化
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
