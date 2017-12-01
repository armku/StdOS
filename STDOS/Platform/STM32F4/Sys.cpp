/*
此文件需要优化掉
 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "Array.h"
#include "TInterrupt.h"
#include "SerialPort.h"
#include "stm32f4xx.h"

extern "C"
{
    extern uint __heap_base;
    extern uint __heap_limit;
    extern uint __initial_sp;
}

extern String *CPUName;
void TSys::OnInit()
{
    this->Clock = 168000000;
    this->CystalClock = HSE_VALUE;
    this->MessagePort = COM1;
    Buffer::Copy(this->ID, (void*)0x1fff7a10, ArrayLength(this->ID));

    this->CPUID = SCB->CPUID;
    uint MCUID = DBGMCU->IDCODE; // MCU编码。低字设备版本，高字子版本
    this->RevID = MCUID >> 16;
    this->DevID = MCUID &0x0FFF;

    this->FlashSize = *(__IO ushort*)(0X1FFF7a22); // 容量
    switch (this->DevID)
    {
        case 0X0413:
            CPUName = new String("STM32F407ZG");
            this->RAMSize = 192;
            break;
        default:
            CPUName = new String("未知");
            this->RAMSize = 0;
            break;
    }
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
    printf("%s %dMHz Flash:%dk RAM:%dk\r\n", CPUName->GetBuffer(), this->Clock, this->FlashSize, this->RAMSize);
    printf("DevID:0x%04X RevID:0x%04X \r\n", this->DevID, this->RevID);
    debug_printf("CPUID:%p", this->CPUID);
    debug_printf(" ARMv7-M");
    debug_printf(" Cortex-M%d:", 3);
    debug_printf(" R%dp%d", Rx, Px);
    debug_printf("\r\n");
    debug_printf("Heap :(%p, %p) = 0x%x (%dk)\r\n", (uint) &__heap_base, (uint) &__heap_limit, HeapSize, HeapSize / 1024);
    debug_printf("Stack:(%p, %p) = 0x%x (%dk)\r\n", (uint) &__heap_limit, (uint) &__initial_sp, StackSize, StackSize / 1024);

    printf("ChipType:0x42455633 3\r\n");
}

// 重启系统
void TSys::Reset()const
{
    NVIC_SystemReset();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
SmartIRQ::SmartIRQ(bool enable)
{
    _state = __get_PRIMASK();
    if (enable)
        __enable_irq();
    else
        __disable_irq();
}

SmartIRQ::~SmartIRQ()
{
    __set_PRIMASK(_state);
}
