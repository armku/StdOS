#include "Power.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

// 设置电源管理钩子函数
void Power::SetPower()
{
    return ;
}

// 电源等级变更（如进入低功耗模式）时调用
void Power::ChangePower(int level)
{
    return ;
}

// 低功耗相关
void Power::Sleep(int msTime)
{
    return ;
}

void Power::Stop(int msTime)
{
    return ;
}

void Power::Standby(int msTime)
{
    return ;
}

// 各模块向系统注册低功耗句柄，供系统进入低功耗前调用
void Power::AddPower(Power *power)
{
    return ;
}

// 附加到系统时钟，睡眠时进入低功耗
bool Power::AttachTimeSleep()
{
    return false;
}


void Power::OnSleep(int msTime)
{
    return ;
}

void Power::OnStop(int msTime)
{
    return ;
}

void Power::OnStandby(int msTime)
{
    return ;
}
