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
	return 1;
}
