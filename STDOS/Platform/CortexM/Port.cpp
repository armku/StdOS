#include "Port.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif
// 获取组和针脚
#define _GROUP(PIN) ((GPIO_TypeDef *) (GPIOA_BASE + (((PIN) & (ushort)0xF0) << 6)))
#define _RCC_APB2(PIN) (RCC_APB2Periph_GPIOA << (PIN >> 4))

signed int Bits2Index(int a1)
{
//  signed int v1; // r1@1
//  signed int result; // r0@1

//  v1 = a1;
//  for ( result = 0; result < 16; ++result )
//  {
//    if ( v1 & 1 )
//      return result;
//    v1 >>= 1;
//  }
  return -1;
}

void Port_OnOpen()
{
  
}
int IndexToGroup(int a1)
{
  return (a1 << 10) + 1073809408;
}
//bool InputPort_HasEXTI(int a1, int a2)
//{
//  return States[a1] && States[a1] != a2;
//}
//int SetEXIT(char a1, char a2, int a3, int a4)
//{
//  char v4; // r5@1
//  char v5; // r6@1
//  int v6; // r4@1
//  int v8; // [sp+0h] [bp-18h]@1
//  int v9; // [sp+4h] [bp-14h]@1

//  v8 = a3;
//  v9 = a4;
//  v4 = a1;
//  v5 = a2;
//  v6 = a3;
//  EXTI_StructInit(&v8);
//  v8 = 1 << v4;
//  LOBYTE(v9) = 0;
//  if ( v6 == 1 )
//  {
//    BYTE1(v9) = 8;
//  }
//  else if ( v6 == 2 )
//  {
//    BYTE1(v9) = 12;
//  }
//  else
//  {
//    BYTE1(v9) = 16;
//  }
//  BYTE2(v9) = v5;
//  return EXTI_Init(&v8);
//}
//InputPort * GPIO_ISR(int a1)
//{
//  int v1; // r5@1
//  InputPort *result; // r0@1
//  InputPort **v3; // r4@3
//  char v4; // r7@6

//  v1 = a1;
//  result = (InputPort *)(unsigned __int8)hasInitState;
//  if ( hasInitState )
//  {
//    result = (InputPort *)States;
//    v3 = (InputPort **)&States[v1];
//    if ( v3 )
//    {
//      result = *v3;
//      if ( *v3 )
//      {
//        do
//        {
//          v40010414 = 1 << v1;
//          v4 = (*(int (**)(void))(*(_DWORD *)*v3 + 8))();
//        }
//        while ( v40010414 & (1 << v1) );
//        result = (InputPort *)InputPort::OnPress(*v3, v4);
//      }
//    }
//  }
//  return result;
//}


void OutputPort::Write(bool value)const
{
    if (this->Invert)
    {
        if (value)
        {
            GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
        }
        else
        {
            GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
        }
    }
    else
    {
        if (value)
        {
            GPIO_SetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
        }
        else
        {
            GPIO_ResetBits(_GROUP(this->_Pin), _PORT(this->_Pin));
        }
    }
}

//int _INTERNAL_8_Port_cpp_41434ca6::OpenClock(signed int a1, int a2)
//{
//  signed int v2; // r5@1
//  int v3; // r6@1
//  signed int v4; // r4@1
//  int v5; // r1@2
//  int result; // r0@2
//  int v7; // r1@4

//  v2 = a1;
//  v3 = a2;
//  v4 = a1 >> 4;
//  if ( a2 )
//  {
//    v5 = GroupClock[v4];
//    result = v5 + 1;
//    GroupClock[v4] = result;
//    if ( v5 )
//      return result;
//  }
//  else
//  {
//    v7 = GroupClock[v4];
//    result = v7 - 1;
//    GroupClock[v4] = result;
//    if ( v7 > 1 )
//      return result;
//  }
//  return OpenPeriphClock(v2, v3);
//}

void Port::OnOpen(void *param)
{
    GPIO_InitTypeDef *gpio = (GPIO_InitTypeDef*)param;
    #ifdef STM32F0
		gpio->GPIO_Speed = GPIO_Speed_50MHz;
    #elif defined STM32F1
        gpio->GPIO_Speed = GPIO_Speed_50MHz;
    #elif defined STM32F4
        gpio->GPIO_Speed = GPIO_Speed_100MHz;
    #endif 
}

void Port::OnClose(){}

void Port::Opening()
{
}
bool InputPort::OnRegister()
{
}
void InputPort::ClosePin()
{
	
}
bool OutputPort::Read()const
{
    return this->Invert ? !Port::Read(): Port::Read();
}

bool Port::Read()const
{
    GPIO_TypeDef *group = _GROUP(this->_Pin);
    return (group->IDR >> (this->_Pin &0xF)) &1;
}
