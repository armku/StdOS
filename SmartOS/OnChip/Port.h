#ifndef _Port_H_
#define _Port_H_

#include "Kernel\Sys.h"

/******** 端口打开关闭流程 ********/
/*
Port::Open
	#Port::Opening
		OutputPort::OnOpen
			#Port::OnOpen
		#OutputPort::OpenPin

Port::Close
	#Port::OnClose
*/


/******************************** Port ********************************/

// 端口基类
// 用于管理一个端口，通过PinBit标识该组的哪些引脚。
// 子类初始化时先通过SetPort设置端口，备份引脚状态，然后Config通过gpio结构体配置端口，端口销毁时恢复引脚状态
typedef enum
{
	GPIO_AIN = 0x0,//模拟输入模式
	GPIO_IN_FLOATING = 0x04,//浮空输入模式
	GPIO_IPD = 0x28,//下拉输入模式
	GPIO_IPU = 0x48, //上拉输入模式
	GPIO_Out_OD = 0x14,//开漏输出模式
	GPIO_Out_PP = 0x10,//通用推挽输出模式
	GPIO_AF_OD = 0x1C,//复用功能开漏输出
	GPIO_AF_PP = 0x18//复用功能推挽输出
}GPIOMode_T;

// 端口类
class Port
{
public:
	enum GPIO_AF
	{
		AF_0 = 0,
		AF_1 = 1,
		AF_2 = 2,
		AF_3 = 3,
		AF_4 = 4,
		AF_5 = 5,
		AF_6 = 6,
		AF_7 = 7
	};

    Pin		_Pin;		// 引脚
	bool	Opened;		// 是否已经打开
	byte    Index;		// 引脚自身次序编号，用于区分多引脚次序
	void*	State;		// 用户状态数据

	Port();
#ifndef TINY
	//virtual ~Port();
#endif

    Port& Set(Pin pin);	// 设置引脚
	bool Empty() const;

	bool Open();
	void Close();
	void Clear();

	void AFConfig(GPIO_AF GPIO_AF) const;
	static void RemapConfig(uint param, bool sta);

    virtual bool Read() const;

	String ToString() const;

protected:
    // 配置过程
    //virtual void OnOpen(void* param);
	//virtual void OnClose();

private:
	void Opening();












public:
	virtual Port& SetPin(Pin pin);	// 设置引脚
	void pinMode(GPIOMode_T mode);//设置管脚模式

	bool ReadInput() const;// Read/ReadInput 的区别在于，前者读输出后者读输入，在开漏输出的时候有很大区别
	void Write(bool value) const;
	Port& operator=(bool value) { Write(value); return *this; }
	Port& operator=(Port& port) { Write(port.Read()); return *this; }
	
	operator bool() const { return Read(); }
public:
	int Invert;//是否反向
};

#endif //_Port_H_
