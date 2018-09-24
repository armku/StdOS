#ifndef __Environment_H__
#define __Environment_H__
#include <stdint.h>
// 环境变量
class TEnvironment
{
public:
	TEnvironment();
	
	// 获取系统启动后经过的毫秒数
	uint64_t TickCount() const;

	// 获取系统启动后经过的毫秒数
	uint64_t Ms() const;

	// 获取当前计算机上的处理器数
	int ProcessorCount() const;
};

// 全局环境变量
extern const TEnvironment Environment;

#endif
