#include "stm32f10x.h"
#include "WatchDog.h"

CWatchDog::CWatchDog(uint ms)
{
    this->timeout = ms;
}

byte CWatchDog::Init()
{
    byte prv = IWDG_Prescaler_64;
    ushort rlv = 625;

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
