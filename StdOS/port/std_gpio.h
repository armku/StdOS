#ifndef _STD_GPIO_H
#define _STD_GPIO_H

#include "Type.h"
#include "Platform/Pin.h"

typedef enum
{
	OUTPUT = 0x00,

	OUTPUT_PP,
	OUTPUT_PP_PU,
	OUTPUT_PP_PD,

	OUTPUT_OD,
	OUTPUT_OD_PU,
	OUTPUT_OD_PD,

	AIN,

	INPUT,
	INPUT_PD,
	INPUT_PU,

	AF_OD,
	AF_OD_PU,
	AF_OD_PD,

	AF_PP,
	AF_PP_PU,
	AF_PP_PD,
} PIN_MODE;

class Gpio
{
public:
	Gpio() { this->_Pin = P0; };
	virtual void SetPin(Pin pin)=0;//设置管脚
	virtual void mode(PIN_MODE mode) = 0;
	virtual void mode(PIN_MODE mode, uint8_t af_configration) = 0;
	virtual void set() = 0;
	virtual void reset() = 0;

	virtual void write(uint8_t val) = 0;
	virtual void read(uint8_t *val) = 0;
	virtual uint8_t read(void) = 0;
	virtual void toggle() = 0;

public:
	bool Empty() const;
	void Close();
	Gpio& Set(Pin pin);	// 设置引脚
public:
	//Pin id;
	Pin _Pin;
	bool	Opened;		// 是否已经打开
};

#endif // !_STD_GPIO_H
