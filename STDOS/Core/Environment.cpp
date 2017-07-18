#include "Kernel\TTime.h"
#include "Environment.h"

TEnvironment::TEnvironment()
{
	
}

// ��ȡϵͳ�����󾭹��ĺ�����
UInt64 TEnvironment::TickCount()const
{
    return Time.Milliseconds;
}


// ��ȡϵͳ�����󾭹��ĺ�����
UInt64 TEnvironment::Ms()const
{
    return Time.Milliseconds;
}

// ��ȡ��ǰ������ϵĴ�������
int TEnvironment::ProcessorCount()const
{
	#ifdef STM32F0	
		return 1;
	#elif defined STM32F1
		return 1;
	#elif defined STM32F4
		return 1;
	#endif
}
