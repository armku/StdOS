/*
此文件需要优化掉
 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "TInterrupt.h"
#include "SerialPort.h"
#include "Platform\stm32.h"

extern "C"
{
    extern uint32_t __heap_base;
    extern uint32_t __heap_limit;
    extern uint32_t __initial_sp;
}

static char *CPUName;
void TSys::OnInit()
{
    this->Clock = 168000000;
    this->CystalClock = HSE_VALUE;
    this->MessagePort = COM1;
    Buffer::Copy(this->ID, (void*)0x1fff7a10, ArrayLength(this->ID));

    this->CPUID = SCB->CPUID;
    uint32_t MCUID = DBGMCU->IDCODE; // MCU编码。低字设备版本，高字子版本
    this->RevID = MCUID >> 16;
    this->DevID = MCUID &0x0FFF;

    this->FlashSize = *(__IO uint16_t*)(0X1FFF7a22); // 容量
    switch (this->DevID)
    {
        case 0X0413:
            CPUName = "STM32F407ZG";
            this->RAMSize = 192;
            break;
        default:
            CPUName = "未知";
            this->RAMSize = 0;
            break;
    }
}

void TSys::OnShowInfo()const
{
    uint32_t Rx = 0;
    uint32_t Px = 0;

    uint32_t HeapSize = 0;
    uint32_t StackSize = 0;


    HeapSize = ((uint32_t) &__heap_limit - (uint32_t) &__heap_base);
    StackSize = ((uint32_t) &__initial_sp - (uint32_t) &__heap_limit);

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


    debug_printf("STDOS::");
    debug_printf("STM32");
    debug_printf("F103");
    debug_printf("%s %dMHz Flash:%dk RAM:%dk\r\n", CPUName, this->Clock, this->FlashSize, this->RAMSize);
    debug_printf("DevID:0x%04X RevID:0x%04X \r\n", this->DevID, this->RevID);
    debug_printf("CPUID:%p", this->CPUID);
    debug_printf(" ARMv7-M");
    debug_printf(" Cortex-M%d:", 3);
    debug_printf(" R%dp%d", Rx, Px);
    debug_printf("\r\n");
    debug_printf("Heap :(%p, %p) = 0x%x (%dk)\r\n", (uint32_t) &__heap_base, (uint32_t) &__heap_limit, HeapSize, HeapSize / 1024);
    debug_printf("Stack:(%p, %p) = 0x%x (%dk)\r\n", (uint32_t) &__heap_limit, (uint32_t) &__initial_sp, StackSize, StackSize / 1024);

    debug_printf("ChipType:0x42455633 3\r\n");
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
