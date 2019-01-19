#include "std_gpio.h"

bool Gpio::Empty()const
{
	return _Pin == P0;
}

void Gpio::Close()
{
	if (this->Opened)
	{
		if (this->_Pin != P0)
		{
			this->Opened = false;
		}
	}
}
