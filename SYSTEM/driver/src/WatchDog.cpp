#include "stm32f10x.h"
#include "WatchDog.h"

CWatchDog::CWatchDog(uint32_t ms)
{
    this->timeout = ms;
}

CWatchDog::~CWatchDog(){

}
uint8_t CWatchDog::Init()
{
    uint8_t prv = IWDG_Prescaler_64;
    uint16_t rlv = 625;

    // 使能 预分频寄存器PR和重装载寄存器RLR可写
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    // 设置预分频器值
    IWDG_SetPrescaler(prv);

    // 设置重装载寄存器值
    IWDG_SetReload(rlv);

    // 把重装载寄存器的值放到计数器中
    IWDG_ReloadCounter();

    // 使能 IWDG
    IWDG_Enable();
    return 0;
}

void CWatchDog::Feed()
{
    IWDG_ReloadCounter();
}
