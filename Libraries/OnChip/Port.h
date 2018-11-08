#ifndef _Port_H_
#define _Port_H_

#include "Core\Type.h"
#include "Platform\Pin.h"

/******************************** Port ********************************/

// 端口基类
class Port
{
public:	
    Pin		_Pin;		// 引脚  
	bool	Opened;		// 是否已经打开

	Port();

    Port& Set(Pin pin);	// 设置引脚
	bool Empty() const;

	bool Open();
	void Close();

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
    uint8_t Invert		= 2;		// 是否倒置输入输出。默认2表示自动检测
    bool OpenDrain	= false;	// 是否开漏输出
    uint8_t Speed		= 50;		// 速度

    OutputPort();
    OutputPort(Pin pin);
    OutputPort(Pin pin, uint8_t invert, bool openDrain = false, uint8_t speed = 50);
	
    void Write(bool value) const;
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
	    
    uint8_t	Invert		= 2;	// 是否倒置输入输出。默认2表示自动检测
    bool	Floating	= true;	// 是否浮空输入
    PuPd	Pull		= UP;	// 上拉下拉电阻
	
	InputPort(){}
    InputPort(Pin pin, bool floating = true, PuPd pull = UP);

	// 读取状态
    virtual bool Read() const;
    operator bool() const { return Read(); }

protected:
    virtual void OnOpen(void* param);
};

#endif //_Port_H_
