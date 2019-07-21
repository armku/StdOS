#include "Buffer.h"

#include "Environment.h"

#if defined(__CC_ARM)
	#include <time.h>
#else
	#include <ctime>
#endif

const TEnvironment Environment;

/************************************************ TEnvironment ************************************************/

TEnvironment::TEnvironment()
{
	
}

// ��ȡϵͳ�����󾭹��ĺ�����
UInt64 TEnvironment::TickCount() const { return clock(); }

// ��ȡϵͳ�����󾭹��ĺ�����
UInt64 TEnvironment::Ms() const { return clock(); }

// ��ȡ��ǰ������ϵĴ�������
int TEnvironment::ProcessorCount() const { return 1; }
