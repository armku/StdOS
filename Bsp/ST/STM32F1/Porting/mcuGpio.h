#ifndef _STD_PORT_GPIO_H
#define _STD_PORT_GPIO_H

#include "../../../../StdOS/Port/Gpio.h"

class mcuGpio : public Gpio
{
public:
	mcuGpio();
	/*mcuGpio(GPIO_TypeDef *port, uint16_t pin);*/
	virtual void SetPin(Pin pin);//设置管脚
	virtual void mode(PIN_MODE mode);//设置管脚模式
	virtual void toggle();//端口翻转
	virtual void read(uint8_t *val);//读取端口
	virtual uint8_t read(void);//读

	operator int();
	int operator =(mcuGpio &);
	/*mcuGpio operator = (int value);*/
	// Read/ReadInput 的区别在于，前者读输出后者读输入，在开漏输出的时候有很大区别
	virtual bool Read();
	//virtual uint8_t ReadInput()=0;
	void Write(bool value);

	static void Write(Pin pin, bool value);
	operator bool() { return Read(); }
	bool Open();
	mcuGpio(Pin pin, uint8_t invert, bool openDrain = false);
	virtual mcuGpio& operator=(bool value) { Write(value); return *this; }
	//virtual mcuGpio& operator=(mcuGpio& port) { Write(port.Read()); return *this; }
public:
	uint16_t      _pin; /**< 引脚的序号 */	
};

#endif // !_STD_PORT_GPIO_H
