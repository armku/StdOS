#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
#include "Type.h"
// 中断委托（中断号，参数）
typedef void (*InterruptCallback)(ushort num, void* param);

//VectorySize 64 未考证
// 中断管理类
class TInterrupt
{
public:
    void Init() const;    // 初始化中断向量表
	
    // 注册中断函数（中断号，函数，参数）
    bool Activate(short irq, InterruptCallback isr, void* param = nullptr);

    // 设置优先级
    void SetPriority(short irq, uint32_t priority = 1) const;

    static void GlobalEnable();	// 打开全局中断
    static void GlobalDisable();	// 关闭全局中断
private:
	void OnInit() const;
};

// 全局中断类对象
extern TInterrupt Interrupt;
// 会在maim（）  之前进行初始化   没有构造函数...
// 初始化函数  Interrupt.Init（）  在 Sys.cpp 内 TSys::TSys() 中被调用
// <TSys::TSys()也是构造函数   Sys.TSys()函数 在main（）函数之前被执行>

extern "C"
{
	void FaultHandler(void);
	void UserHandler(void);

#if defined(BOOT) || defined(APP)
	void RealHandler(void);
#endif
}

#endif

/*
完全接管中断，在RAM中开辟中断向量表，做到随时可换。
由于中断向量表要求128对齐，这里多分配128字节，找到对齐点后给向量表使用

为了增强中断函数处理，我们使用_Vectors作为真正的中断向量表，全部使用OnHandler作为中断处理函数。
然后在OnHandler内部获取中断号，再调用Vectors中保存的用户委托，并向它传递中断号和参数。
*/
