#include "Gpio.h"
#include "wiring_digital.h"

Gpio::Gpio()
{ 
	this->_Pin = P0; 
	this->Opened = false; 
	this->Invert = 0;
}


bool Gpio::Empty()const
{
	return _Pin == P0;
}

/*
单一引脚初始化
 */
Gpio &Gpio::Set(Pin pin)
{
	if (this->_Pin != pin)
	{
		this->_Pin = pin;

	}
	return  *this;
}
