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
class Port
{
public:	
    Pin		_Pin;		// 引脚   4
	bool	Opened;		// 是否已经打开 8 5
	uint8_t    Index;		// 引脚自身次序编号，用于区分多引脚次序
	void*	State;		// 用户状态数据

	Port();

    Port& Set(Pin pin);	// 设置引脚
	bool Empty() const;

	bool Open();
	void Close();
	void Clear();

	static void RemapConfig(uint32_t param, bool sta);

    virtual bool Read() const;

protected:
    // 配置过程
    virtual void OnOpen(void* param);
};

/******************************** OutputPort ********************************/

// 输出口
class OutputPort : public Port
{
public:
    uint8_t Invert		= 2;		// 是否倒置输入输出。默认2表示自动检测  12
    bool OpenDrain	= false;	// 是否开漏输出 13
    uint8_t Speed		= 50;		// 速度 14

    OutputPort();
    OutputPort(Pin pin);
    OutputPort(Pin pin, uint8_t invert, bool openDrain = false, uint8_t speed = 50);

	OutputPort& Init(Pin pin, bool invert);

    void Write(bool value) const;
	// 拉高一段时间后拉低
	void Up(int ms) const;
	void Down(int ms) const;
	// 闪烁多次
	void Blink(int times, int ms) const;

	// Read/ReadInput 的区别在于，前者读输出后者读输入，在开漏输出的时候有很大区别
    virtual bool Read() const;
	bool ReadInput() const;

    static void Write(Pin pin, bool value);

    OutputPort& operator=(bool value) { Write(value); return *this; }
    OutputPort& operator=(OutputPort& port) { Write(port.Read()); return *this; }
    operator bool() const { return Read(); }

protected:
    virtual void OnOpen(void* param);
	virtual void OpenPin(void* param);
};

/******************************** AlternatePort ********************************/

// 复用输出口
class AlternatePort : public OutputPort
{
public:
	AlternatePort();
    AlternatePort(Pin pin);
    AlternatePort(Pin pin, uint8_t invert, bool openDrain = false, uint8_t speed = 50);

protected:
    virtual void OpenPin(void* param);
};

/******************************** InputPort ********************************/

// 输入口
class InputPort : public Port
{
public:
    typedef enum
    {
        NOPULL	= 0x00,
        UP		= 0x01,	// 上拉电阻
        DOWN	= 0x02,	// 下拉电阻
    }PuPd;

    // 读取委托
    typedef void (*IOReadHandler)(InputPort* port, bool down, void* param);

    uint8_t	Invert		= 2;	// 是否倒置输入输出。默认2表示自动检测 16
    bool	Floating	= true;	// 是否浮空输入
    PuPd	Pull		= UP;	// 上拉下拉电阻
	
	InputPort(){}
    InputPort(Pin pin, bool floating = true, PuPd pull = UP);

	InputPort& Init(Pin pin, bool invert);

	// 读取状态
    virtual bool Read() const;
    operator bool() const { return Read(); }

protected:
    virtual void OnOpen(void* param);
};

#endif //_Port_H_
