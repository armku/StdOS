#ifndef _Sys_H_
#define _Sys_H_

#include "Core\Type.h"
#include "Core\List.h"
#include "Core\Delegate.h"
#include "Version.h"
#include "TTime.h"


#include <stdio.h>
#include <stddef.h>

#include "Core\Type.h"
#include "Core\Buffer.h"
#include "Core\Array.h"
#include "Core\ByteArray.h"
#include "Core\SString.h"
#include "Core\Stream.h"
#include "Core\DateTime.h"
#include "Core\Version.h"
#include "Core\List.h"
#include "Core\Dictionary.h"
#include "Core\Delegate.h"

/* 引脚定义 */
#include "Platform\Pin.h"



extern "C"
{
#ifdef DEBUG
	#define debug_printf SmartOS_printf	
#else
	#define debug_printf(format, ...)
#endif
}

#ifdef DEBUG
#define assert(expr, msg) ((expr) ? (void)0 : assert_failed2(msg, (const char*)__FILE__, __LINE__))

#else

#define assert(expr, msg) ((void)0)

#endif

// 系统类
class TSys
{
public:
    uint	Clock;  	// 系统时钟
    uint	CystalClock;// 晶振时钟
	uint	HeapSize;	// 堆大小
	uint	StackSize;	// 栈大小

	byte	ID[12];		// 芯片ID。
    ushort	FlashSize;	// 芯片Flash容量。
    //ushort	RAMSize;	// 芯片RAM容量
	Version OsVer;//系统版本
	Version AppVer;//软件版本

	TSys();

	void Init();     	// 初始化系统
	void ShowInfo() const;
	
	uint	Seconds() const;	// 系统绝对当前时间，秒

    void Sleep112233(int ms) const; // 毫秒级延迟，仅供系统调用
	
	// 延迟异步重启
	void Reboot(int msDelay = 0) const;
	// 打开全局中断
	void GlobalEnable();
	// 关闭全局中断
	void GlobalDisable();
		
private:
	void Reset() const;
	void OnInit();
	void OnShowInfo() const;

public:
	// 创建任务，返回任务编号。dueTime首次调度时间ms，period调度间隔ms，-1表示仅处理一次
	uint AddTask(Action func, void* param, int dueTime = 0, int period = 0, cstring name = nullptr) const;
	template<typename T>
	uint AddTask(void(T::*func)(), T* target, int dueTime = 0, int period = 0, cstring name = nullptr)
	{
		return AddTask(*(Action*)&func, target, dueTime, period, name);
	}
	void RemoveTask(uint& taskid) const;
	// 设置任务的开关状态，同时运行指定任务最近一次调度的时间，0表示马上调度
	bool SetTask(uint taskid, bool enable, int msNextTime = -1) const;
	// 改变任务周期
	bool SetTaskPeriod(uint taskid, int period) const;

	bool Started;
	void Start();	// 开始系统大循环
};

extern TSys Sys;		// 创建一个全局的Sys对象  会在main函数之前执行构造函数（！！！！！）

#define STDOS_VERSION "0.8.2019.0408" //系统版本号
extern char* AppVersion;//需要定义软件版本号实现 如：char *AppVersion = "0.1.2018.1114";

#endif //_Sys_H_

/*

v0.1.2018.0820  使用C++11实现，实现F1 F4 F1的GPIO和串口、IIC、SPI、中断管理
v0.2.2018.		串口发送、接收采用DMA方式实现。
v0.3.2018       更换串口实现方式
v0.4.2018.1024  启动过程优化，启动时关闭全局中断，启动完成开启，防止启动异常
v0.5.2018.1030  系统统计信息精简，去除不必要内容
v0.9.2019.0408	SPIsoft正常
*/
