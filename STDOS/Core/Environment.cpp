#include "Environment.h"

TEnvironment::TEnvironment()
{

}

	
// 获取系统启动后经过的毫秒数
uint64_t TEnvironment::TickCount() const
{
	return 0;
}

// 获取系统启动后经过的毫秒数
uint64_t TEnvironment::Ms() const
{
	return 0;
}

// 获取当前计算机上的处理器数
int TEnvironment::ProcessorCount() const
{
	return 1;
}
