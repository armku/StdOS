#ifndef _Port_H_
#define _Port_H_

#include "Core\Type.h"
#include "Platform\Pin.h"

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

// 端口基类
class Port
{
public:	
    Pin		_Pin;		// 引脚  
	bool	Opened;		// 是否已经打开

	void SetPinMode(GPIOMode_T mode);//设置管脚模式

	bool ReadInput() const;// Read/ReadInput 的区别在于，前者读输出后者读输入，在开漏输出的时候有很大区别
	static void Write(Pin pin, bool value);
	void Write(bool value) const;
	Port& operator=(bool value) { Write(value); return *this; }
	Port& operator=(Port& port) { Write(port.Read()); return *this; }
	Port();

    Port& Set(Pin pin);	// 设置引脚
	bool Empty() const;

	bool Open();
	void Close();

	virtual bool Read() const;
	operator bool() const { return Read(); }
};

#endif //_Port_H_
