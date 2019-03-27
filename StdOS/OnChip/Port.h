#ifndef _Port_H_
#define _Port_H_

#include "Core\Type.h"
#include "Platform\Pin.h"

// 端口基类
class Port
{
public:	
    Pin		_Pin;		// 引脚  
	bool	Opened;		// 是否已经打开

	//InputPort
	typedef enum
	{
		NOPULL = 0x00,
		UP = 0x01,	// 上拉电阻
		DOWN = 0x02,	// 下拉电阻
	}PuPd;
	bool	Floating;	// 是否浮空输入
	PuPd	Pull;	// 上拉下拉电阻
	// END InputPort
	//OutputPort
	bool OpenDrain;	// 是否开漏输出
	//end OutputPort
	//OutputPort
	/*operator bool() const { return Read(); }*/
	bool ReadInput() const;// Read/ReadInput 的区别在于，前者读输出后者读输入，在开漏输出的时候有很大区别
	static void Write(Pin pin, bool value);

	//end OutputPort
	virtual void OpenPinAlternatePort(void* param);

	Port();

    Port& Set(Pin pin);	// 设置引脚
	bool Empty() const;

	bool Open();
	void Close();

	virtual bool Read() const;
	operator bool() const { return Read(); }	
protected:
    // 配置过程
    virtual void OnOpen(void* param);
	virtual void OnOpenInputPort(void* param);
	virtual void OnOpenOutputPort(void* param);
	virtual void OpenPinOutputPort(void* param);
};
// 输出口
class OutputPort : public Port
{
public:  
    OutputPort();
    OutputPort(Pin pin, uint8_t invert);
	
    void Write(bool value) const;
    OutputPort& operator=(bool value) { Write(value); return *this; }
    OutputPort& operator=(OutputPort& port) { Write(port.Read()); return *this; }
};

#endif //_Port_H_
