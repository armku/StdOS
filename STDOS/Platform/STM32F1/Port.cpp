#include "Port.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

GPIO_TypeDef *IndexToGroup(byte index);

bool Port::Open()
{
    if (this->Opened == false)
    {
        if (_Pin != P0)
        {
            // 打开时钟
            int gi = _Pin >> 4;
            #ifdef STM32F0
                RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA << gi, ENABLE);
            #elif defined STM32F1
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << gi, ENABLE);
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
            #elif defined STM32F4
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA << gi, ENABLE);
            #endif 

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

    #ifdef STM32F0
		gpio->GPIO_Mode = GPIO_Mode_OUT; //普通输出模式	
        if (this->OpenDrain)
        {
            gpio->GPIO_OType = GPIO_OType_OD;
			gpio->GPIO_PuPd = GPIO_PuPd_NOPULL;/*设置引脚模式为无上拉*/
        }
        else
        {
            gpio->GPIO_OType = GPIO_OType_PP;//通用推挽输出			
			gpio->GPIO_PuPd = GPIO_PuPd_UP;/*设置引脚模式为上拉*/
        }
    #elif defined STM32F1
        if (this->OpenDrain)
        {
            gpio->GPIO_Mode = GPIO_Mode_Out_OD;
        }
        else
        {
            gpio->GPIO_Mode = GPIO_Mode_Out_PP;
        }
    #elif defined STM32F4
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
    #endif 
}
void AnalogInPort::OpenPin(void* param)
{
	
}
void AlternatePort::OpenPin(void *param)
{
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
    #ifdef STM32F0
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
    #elif defined STM32F1
        gpio->GPIO_Mode = this->OpenDrain ? GPIO_Mode_AF_OD : GPIO_Mode_AF_PP;
    #elif defined STM32F4
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
    #endif 
    int i = 0;
    i++;
}
void Port::RemapConfig(uint param, bool sta)
{
	
}
void InputPort::OpenPin(void* param)
{
}












