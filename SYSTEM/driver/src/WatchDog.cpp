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

    // ʹ�� Ԥ��Ƶ�Ĵ���PR����װ�ؼĴ���RLR��д
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    // ����Ԥ��Ƶ��ֵ
    IWDG_SetPrescaler(prv);

    // ������װ�ؼĴ���ֵ
    IWDG_SetReload(rlv);

    // ����װ�ؼĴ�����ֵ�ŵ���������
    IWDG_ReloadCounter();

    // ʹ�� IWDG
    IWDG_Enable();
    return 0;
}

void CWatchDog::Feed()
{
    IWDG_ReloadCounter();
}
