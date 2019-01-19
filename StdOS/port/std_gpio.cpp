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

/*
��һ���ų�ʼ��
 */
Gpio &Gpio::Set(Pin pin)
{
	if (this->_Pin != pin)
	{
		if (this->_Pin != P0)
			this->Close();
		this->_Pin = pin;

	}
	return  *this;
}
