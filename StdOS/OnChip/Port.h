#ifndef _Port_H_
#define _Port_H_

#include "Core\Type.h"
#include "Platform\Pin.h"
#include "port/std_port_gpio.h"

// 端口基类
class Port:public mcuGpio
{
public:	
    bool Open();
};
// 输出口
class OutputPort : public Port
{
public:
    bool OpenDrain	= false;	// 是否开漏输出

    OutputPort();
    OutputPort(Pin pin, uint8_t invert, bool openDrain = false);
	
    void Write(bool value) ;
	// Read/ReadInput 的区别在于，前者读输出后者读输入，在开漏输出的时候有很大区别
    uint8_t ReadInput();

    static void Write(Pin pin, bool value);

    OutputPort& operator=(bool value) { Write(value); return *this; }
    OutputPort& operator=(OutputPort& port) { Write(port.Read()); return *this; }
    operator bool()  { return Read(); }

protected:
    virtual void OnOpen(void* param);
	virtual void OpenPin(void* param);
};

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
	    
    bool	Floating	= true;	// 是否浮空输入
    PuPd	Pull		= UP;	// 上拉下拉电阻
	
	InputPort(){}

protected:
    virtual void OnOpen(void* param);
};

#endif //_Port_H_
