#include "Type.h"
#include "Environment.h"
#include "TTime.h"

TEnvironment::TEnvironment()
{
}

// ��ȡϵͳ�����󾭹��ĺ�����
UInt64 TEnvironment::TickCount()const
{
    return 0;
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
