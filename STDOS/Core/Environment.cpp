#include "Environment.h"

TEnvironment::TEnvironment()
{

}

	
// ��ȡϵͳ�����󾭹��ĺ�����
uint64_t TEnvironment::TickCount() const
{
	return 0;
}

// ��ȡϵͳ�����󾭹��ĺ�����
uint64_t TEnvironment::Ms() const
{
	return 0;
}

// ��ȡ��ǰ������ϵĴ�������
int TEnvironment::ProcessorCount() const
{
	return 1;
}
