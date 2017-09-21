#include "Port.h"
#include "stm32f4xx.h"


GPIO_TypeDef *IndexToGroup(byte index);

bool Port::Open()
{
    if (this->Opened == false)
    {
        if (_Pin != P0)
        {
            // 打开时钟
            int gi = _Pin >> 4;
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA << gi, ENABLE);
            
            GPIO_InitTypeDef gpio;
            // 特别要慎重，有些结构体成员可能因为没有初始化而酿成大错
            GPIO_StructInit(&gpio);
            gpio.GPIO_Pin = 1 << (this->_Pin &0x0F);
            this->OnOpen(&gpio);

            GPIO_Init(IndexToGroup(this->_Pin >> 4), &gpio);
        }

        this->Opened = true;
    }
    return true;
}
int EXTI_IRQHandler(int a1, void *a2)
{
	return 0;
}
//int OpenPeriphClock(signed int a1)
//{
//  return RCC_APB2PeriphClockCmd(4 << (a1 >> 4));
//}
int InputPort_OpenEXTI(int a1)
{
//  void *v1; // r5@1
//  signed int v2; // r6@1
//  int v3; // r4@1

//  v1 = (void *)a1;
//  v2 = *(_BYTE *)(a1 + 4);
//  v3 = Bits2Index(1 << (v2 & 0xF));
//  RCC_APB2PeriphClockCmd(1);
//  GPIO_EXTILineConfig(v2 >> 4, (unsigned __int8)v3);
//  SetEXIT(v3, 1, 3);
//  TInterrupt::SetPriority((TInterrupt *)&Interrupt, PORT_IRQns[v3], 1u);
//  return TInterrupt::Activate(
//           (TInterrupt *)&Interrupt,
//           PORT_IRQns[v3],
//           (void (__cdecl *)(unsigned __int16, void *))EXTI_IRQHandler,
//           v1);
}
int InputPort_CloseEXTI(const InputPort *a1)
{
//  _INTERNAL_8_Port_cpp_4e0cfc97 *v1; // r5@1
//  const InputPort *v2; // r4@1
//  int v3; // r2@1
//  int result; // r0@1

//  v1 = a1;
//  v2 = (const InputPort *)Bits2Index(1 << (*((_BYTE *)a1 + 4) & 0xF));
//  SetEXIT(v2, 0, 3);
//  result = _INTERNAL_8_Port_cpp_4e0cfc97::IsOnlyExOfInt(v1, v2, v3);
//  if ( result )
//    result = TInterrupt::Deactivate((TInterrupt *)&Interrupt, *((_BYTE *)v2 + (_DWORD)PORT_IRQns));
//  return result;
	return 0;
}
void OutputPort::OpenPin(void *param)
{
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;

        gpio->GPIO_Mode = GPIO_Mode_OUT; //普通输出模式	
        if (this->OpenDrain)
        {
            gpio->GPIO_OType = GPIO_OType_OD; //推挽输出
            gpio->GPIO_PuPd = GPIO_PuPd_NOPULL; //            
        }
        else
        {
            gpio->GPIO_OType = GPIO_OType_PP; //推挽输出
            gpio->GPIO_PuPd = GPIO_PuPd_UP; //上拉
        } 
}
void AnalogInPort::OpenPin(void* param)
{
	
}
void AlternatePort::OpenPin(void *param)
{
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
         gpio->GPIO_Mode = GPIO_Mode_AF;
		gpio->GPIO_Speed = GPIO_Speed_50MHz;
        gpio->GPIO_OType = OpenDrain ? GPIO_OType_OD : GPIO_OType_PP;
		if(!this->OpenDrain)
		{
			gpio->GPIO_PuPd = GPIO_PuPd_UP;
		}
		else
		{
			gpio->GPIO_PuPd = GPIO_PuPd_NOPULL;
		}
    int i = 0;
    i++;
}
void Port::RemapConfig(uint param, bool sta)
{
	
}
void InputPort::OpenPin(void* param)
{
}
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////以下为添加///////////////////////////////////////
// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

void SetEXIT(int pinIndex, bool enable);

    #if 0
        static const int PORT_IRQns[] = 
        {
            EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,  // 5个基础的
            EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,  // EXTI9_5
            EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn  // EXTI15_10
        };
    #endif 
GPIO_TypeDef *IndexToGroup(byte index)
{
    return ((GPIO_TypeDef*)(GPIOA_BASE + (index << 10)));
}

byte GroupToIndex(GPIO_TypeDef *group)
{
    return (byte)(((int)group - GPIOA_BASE) >> 10);
}

void OutputPort::Write(Pin pin, bool value)
{
    if (value)
    {
        GPIO_SetBits(_GROUP(pin), _PORT(pin));
    }
    else
    {
        GPIO_ResetBits(_GROUP(pin), _PORT(pin));
    }
}
void AnalogInPort::OnOpen(void *param)
{
    Port::OnOpen(param);
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
        gpio->GPIO_Mode = GPIO_Mode_AN;
        //gpio->GPIO_OType = !Floating ? GPIO_OType_OD : GPIO_OType_PP;
}
void InputPort::OnOpen(void *param)
{
    Port::OnOpen(param);
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
        gpio->GPIO_Mode = GPIO_Mode_IN;
        if (this->Floating)
        {
            gpio->GPIO_OType = GPIO_OType_OD;
        }
        else
        {
            gpio->GPIO_OType = GPIO_OType_PP;
        }
        switch (this->Pull)
        {
            case NOPULL:
                gpio->GPIO_PuPd = GPIO_PuPd_NOPULL;
                break;
            case UP:
                gpio->GPIO_PuPd = GPIO_PuPd_UP;
                break;
            case DOWN:
                gpio->GPIO_PuPd = GPIO_PuPd_DOWN;
                break;
            default:
                break;
        }
}
void GPIO_ISR(int num);
//所有中断线处理
void EXTI_IRQHandler(ushort num, void *param)
{
        // EXTI0 - EXTI4
        if (num <= EXTI4_IRQn)
        {
            GPIO_ISR(num - EXTI0_IRQn);
        }
        else if (num == EXTI9_5_IRQn)
        {
            // EXTI5 - EXTI9
            uint pending = EXTI->PR &EXTI->IMR &0x03E0; // pending bits 5..9
            int num = 5;
            pending >>= 5;
            do
            {
                if (pending &1)
                {
                    GPIO_ISR(num);
                }
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
                {
                    GPIO_ISR(num);
                }
                num++;
                pending >>= 1;
            }
            while (pending);
        }
}

//中断线打开、关闭
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
/*
整组读取
 */
//ushort OutputPort::ReadGroup()
//{
//    return GPIO_ReadOutputData(((GPIO_TypeDef *)this->State));
//}
#if 0   
    // 配置之前，需要根据倒置情况来设定初始状态，也就是在打开端口之前必须明确端口高低状态
    ushort dat = GPIO_ReadOutputData(((GPIO_TypeDef*)this->State));
    //    if (!Invert)
    //        dat &= ~PinBit;
    //    else
    //        dat |= PinBit;
    GPIO_Write(((GPIO_TypeDef*)this->State), dat);
#endif 

//void OutputPort::WriteGroup(ushort value)
//{
//    GPIO_Write(((GPIO_TypeDef *)this->State), value);
//}
#if 0
    void InputPort::Init(bool floating, PuPd pupd)
    {
        Pull = pupd;
        Floating = floating;

        _Registed = false;
        //ShakeTime = 20;
        // 有些应用的输入口需要极高的灵敏度，这个时候不需要抖动检测
        ShakeTime = 0;
        Invert = false;
    }
#endif 

#if 0
    ushort InputPort::ReadGroup() // 整组读取
    {
        return GPIO_ReadInputData(((GPIO_TypeDef*)this->State));
    }
#endif 


#if 0
    // 注册回调  及中断使能
    void InputPort::Register(IOReadHandler handler, void *param)
    {
        //    if (!PinBit)
        //        return ;

        // 检查并初始化中断线数组
        if (!hasInitState)
        {
            for (int i = 0; i < 16; i++)
            {
                IntState *state1 = &InterruptState[i];
                state1->Pin = P0;
                state1->Handler = NULL;
                state1->Used = 0;
            }
            hasInitState = true;
        }

        byte gi = _Pin >> 4;
        gi = gi;
        //    ushort n = PinBit;
        ushort n = 0;
        for (int i = 0; i < 16 && n != 0; i++)
        {
            // 如果设置了这一位，则注册事件
            if (n &0x01)
            {
                // 注册中断事件
                if (handler)
                {
                    IntState *state2 = &InterruptState[i];
                    state2->ShakeTime = ShakeTime;
                    RegisterInput(gi, i, handler, param);
                }
                else
                {
                    UnRegisterInput(i);
                }
            }
            n >>= 1;
        }

        //    _Registed = handler != NULL;
    }
#endif 

#if 0
    // 申请引脚中断托管
    void InputPort::RegisterInput(int groupIndex, int pinIndex, IOReadHandler handler, void *param)
    {
        IntState *state4 = &InterruptState[pinIndex];
        Pin pin = (Pin)((groupIndex << 4) + pinIndex);
        // 检查是否已经注册到别的引脚上
        if (state4->Pin != pin && state4->Pin != P0)
        {
            return ;
        }
        state4->Pin = pin;
        state4->Handler = handler;
        state4->Param = param;
        //    state4->OldValue = Read(pin); // 预先保存当前状态值，后面跳变时触发中断

        // 打开时钟，选择端口作为端口EXTI时钟线
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
            SYSCFG_EXTILineConfig(groupIndex, pinIndex);
        
        SetEXIT(pinIndex, true);
        // 打开并设置EXTI中断为低优先级
        Interrupt.SetPriority(PORT_IRQns[pinIndex]);
        state4->Used++;
        if (state4->Used == 1)
        {
            Interrupt.Activate(PORT_IRQns[pinIndex], EXTI_IRQHandler, this);
        }
    }
#endif 
#if 0
    void InputPort::UnRegisterInput(int pinIndex)
    {
        IntState *state5 = &InterruptState[pinIndex];
        // 取消注册
        state5->Pin = P0;
        state5->Handler = 0;

        SetEXIT(pinIndex, false);

        state5->Used--;
        if (state5->Used == 0)
        {
            Interrupt.Deactivate(PORT_IRQns[pinIndex]);
        }
    }
#endif 
#if 0
    //测试代码
    InputPort exti(PC13); //PA1 PB3     PA0 PC13
    InputPort exti1(PA0);
    void OnKeyPress(InputPort *pin, bool down, void *param)
    {
        //led1.Write(onoff);
        led2 = !led2;
        //    printf("中断引脚：P%c%d 值%d \r\n", _PIN_NAME(pin), down);
    }


    InputPort key0(PA0);
    //按键事件
    void OnPress(InputPort &port, bool down)
    {
        debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
    }
    InputPort key(PC13);
    key.Press = OnPress;
    key.UsePress();
    key.Open();
    exti.Register(OnKeyPress);
    exti1.Register(OnKeyPress);
#endif
