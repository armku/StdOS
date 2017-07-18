#include "Kernel\TTime.h"
#include "Environment.h"

TEnvironment::TEnvironment()
{
	
}

// 获取系统启动后经过的毫秒数
UInt64 TEnvironment::TickCount()const
{
    return Time.Milliseconds;
}


// 获取系统启动后经过的毫秒数
UInt64 TEnvironment::Ms()const
{
    return Time.Milliseconds;
}

// 获取当前计算机上的处理器数
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
