/*
���ļ���Ҫ�Ż���
 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "Array.h"
#include "TInterrupt.h"
#include "SerialPort.h"
#include "stm32f0xx.h"

extern "C"
{
    extern uint __heap_base;
    extern uint __heap_limit;
    extern uint __initial_sp;
}

extern String *CPUName;
void TSys::OnInit()
{
    this->Clock = 72000000;
    this->CystalClock = HSE_VALUE;
    this->MessagePort = COM1;
    Buffer::Copy(this->ID, (void*)0x1FFFF7AC, ArrayLength(this->ID));

    this->CPUID = SCB->CPUID;
    uint MCUID = DBGMCU->IDCODE; // MCU���롣�����豸�汾�������Ӱ汾
    this->RevID = MCUID >> 16;
    this->DevID = MCUID &0x0FFF;

    this->FlashSize = *(__IO ushort*)(0x1FFFF7CC); // ����
    switch (this->DevID)
    {
        case 0X0440:
            CPUName = new String("STM32F030C8");
            this->RAMSize = 8;
            break;
        case 0X0448:
            CPUName = new String("STM32F072VB");
            this->RAMSize = 64;
            break;
        default:
            CPUName = new String("δ֪");
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
    SmartOS_printf("CPUID:%p", this->CPUID);
    SmartOS_printf(" ARMv7-M");
    SmartOS_printf(" Cortex-M%d:", 3);
    SmartOS_printf(" R%dp%d", Rx, Px);
    SmartOS_printf("\r\n");
    SmartOS_printf("Heap :(%p, %p) = 0x%x (%dk)\r\n", (uint) &__heap_base, (uint) &__heap_limit, HeapSize, HeapSize / 1024);
    SmartOS_printf("Stack:(%p, %p) = 0x%x (%dk)\r\n", (uint) &__heap_limit, (uint) &__initial_sp, StackSize, StackSize / 1024);

    printf("ChipType:0x42455633 3\r\n");
}

// ����ϵͳ
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