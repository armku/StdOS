#include "SString.h"
#include "Environment.h"

TEnvironment::TEnvironment()
{

}

	
// ��ȡϵͳ�����󾭹��ĺ�����
UInt64 TEnvironment::TickCount() const
{
	return 0;
}

// ��ȡϵͳ�����󾭹��ĺ�����
UInt64 TEnvironment::Ms() const
{
	return 0;
}

// ��ȡ��ǰ������ϵĴ�������
int TEnvironment::ProcessorCount() const
{
	return 1;
}
