#include "Power.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

// ���õ�Դ�����Ӻ���
void Power::SetPower()
{
    return ;
}

// ��Դ�ȼ�����������͹���ģʽ��ʱ����
void Power::ChangePower(int level)
{
    return ;
}

// �͹������
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

// ��ģ����ϵͳע��͹��ľ������ϵͳ����͹���ǰ����
void Power::AddPower(Power *power)
{
    return ;
}

// ���ӵ�ϵͳʱ�ӣ�˯��ʱ����͹���
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
