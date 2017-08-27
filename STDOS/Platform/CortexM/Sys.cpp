/*
系统能自动识别常见芯片类型、主频、Flash大小、Ram大小
Sys.ID 是12字节芯片唯一标识、也就是ChipID，同一批芯片仅前面几个字节不同
毫秒级睡眠期间，系统将会安排执行其他耗时较短的任务。如果没有可用任务，系统将会进入低功耗模式，以节省能耗

 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "Array.h"
#include "TInterrupt.h"
#include "SerialPort.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

extern "C"
{
    extern uint __heap_base;
    extern uint __heap_limit;
    extern uint __initial_sp;
}
String *CPUName;
//unsigned int Get_JTAG_ID(void)
//{
//  unsigned int result; // r0@2

//  if ( vE00FFFE8 & 8 )
//    result = (vE00FFFD0 << 8) & 0xFFF | ((unsigned int)vE00FFFE4 >> 3) | (32 * (vE00FFFE8 & 7) + 1);
//  else
//    result = 0;
//  return result;
//}
//int SmartOS_Log(const String *a1)
//{
//  const Buffer *v1; // r5@1
//  int result; // r0@3
//  int v3; // r4@5

//  v1 = (const Buffer *)a1;
//  if ( *((_DWORD *)&Sys + 1) && Sys != 255 )
//  {
//    v3 = SerialPort::GetMessagePort((SerialPort *)Sys);
//    if ( v3 && *(_BYTE *)(v3 + 13) )
//      result = ITransport::Write((ITransport *)v3, v1);
//    else
//      result = 0;
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
//}
//void ExitCritical(void)
//{
//  __enable_irq();
//}
//void EnterCritical(void)
//{
//  __disable_irq();
//}
//unsigned int  _REV(unsigned int a1)
//{
//  return __rev(a1);
//}
//int  _REV16(unsigned __int16 a1)
//{
//  return (unsigned __int16)_REV16();
//}
void TSys::SetStackTop(uint addr)
{
//	return _set_MSP(addr);
}	
void TSys::OnInit()
{
	this->Clock=72000000;
	this->CystalClock=HSE_VALUE;
	this->MessagePort=COM1;
	#ifdef STM32F0
        Buffer::Copy(this->ID,(void *)0x1FFFF7AC,12);
    #elif defined STM32F1 
        Buffer::Copy(this->ID,(void *)0x1FFFF7E8,12);
    #elif defined STM32F4 
        Buffer::Copy(this->ID,(void *)0x1fff7a10,12);
    #endif 	
	
	
	
	this->CPUID = SCB->CPUID;
    uint MCUID = DBGMCU->IDCODE; // MCU编码。低字设备版本，高字子版本
    this->RevID = MCUID >> 16;
    this->DevID = MCUID &0x0FFF;

    #ifdef STM32F0
        this->FlashSize = *(__IO ushort*)(0x1FFFF7CC); // 容量
    #elif defined STM32F1 
        this->FlashSize = *(__IO ushort*)(0x1FFFF7E0); // 容量
    #elif defined STM32F4 
        this->FlashSize = *(__IO ushort*)(0X1FFF7a22); // 容量
    #endif 
    switch (this->DevID)
    {
		#ifdef STM32F0
			case 0X0448:
				CPUName = new String("STM32F072VB");
                this->RAMSize = 64;
                break;
        #elif defined STM32F1
            case 0X0307:
                CPUName = new String("STM32F103RD");
                this->RAMSize = 64;
                break;
            case 0x0410:
                CPUName = new String("STM32F103C8");
                this->RAMSize = 20;
                break;
            case 0X0414:
                CPUName = new String("STM32F103ZE");
                this->RAMSize = 64;
                break;
            case 0X0418:
                CPUName = new String("STM32F105VC");
                this->RAMSize = 64;
                break;
            case 0X0430:
                CPUName = new String("STM32F103VG");
                this->RAMSize = 768;
                break;
         #elif defined STM32F4
            case 0X0413:
                CPUName = new String("STM32F407ZG");
                this->RAMSize = 192;
                break;
            #endif 
        default:
            CPUName = new String("未知");
            this->RAMSize = 0;
            break;
    }
}	
void TSys::OnStart()
{
	
}	
// 初始化系统时钟
void TSys::InitClock()
{	    
    //    this->Inited = 1;
}	
void TSys::OnShowInfo()const
{
    uint Rx = 0;
    uint Px = 0;

    uint HeapSize = 0;
    uint StackSize = 0;


    HeapSize = ((uint) &__heap_limit - (uint) &__heap_base);
    StackSize = ((uint) &__initial_sp - (uint) &__heap_limit);

    switch (this->CPUID &SCB_CPUID_VARIANT_Msk)
    {
        case 0:
            if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 1)
            {
                Rx = 1;
                Px = 0;
            }
            else
            {
                Rx = 0;
                Px = 0;
            }
            break;
        case 0X00100000:
            Rx = 1;
            if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 1)
            {
                Px = 1;
            }
            break;
        case 0X00200000:
            Rx = 2;
            if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 0)
            {
                Px = 0;
            }
            break;
        default:
            Px = 9;
            Rx = 9;
            break;
    }


    printf("STDOS::");
	printf("STM32");
	printf("F103");
	printf(" %dMHz Flash:%dk RAM:%dk\r\n", CPUName->GetBuffer(), this->Clock, this->FlashSize, this->RAMSize);
	printf("DevID:0x%04X RevID:0x%04X \r\n", this->DevID, this->RevID);
	SmartOS_printf("CPUID:%p",this->CPUID);
	SmartOS_printf(" ARMv7-M");
	SmartOS_printf(" Cortex-M%d:",3);
	SmartOS_printf(" R%dp%d",Rx,Px);
	SmartOS_printf("\r\n");
	SmartOS_printf("Heap :(%p, %p) = 0x%x (%dk)\r\n",(uint) &__heap_base, (uint) &__heap_limit, HeapSize, HeapSize / 1024);
	SmartOS_printf("Stack:(%p, %p) = 0x%x (%dk)\r\n", (uint) &__heap_limit, (uint) &__initial_sp, StackSize, StackSize / 1024);
	
	
    printf("ChipType:0x42455633 3\r\n");
    
}
bool TSys::CheckMemory() const
{
	return true;
}		
// 重启系统
void TSys::Reset() const
{
	NVIC_SystemReset();
}
uint TSys::HeapBase() const	// 堆起始地址，前面是静态分配内存
{
//	return &_heap_base;
}	
uint TSys::StackTop() const	// 栈顶，后面是初始化不清零区域
{
	//return (*((_WORD *)this + 31) << 10) + 536870656;
}	
