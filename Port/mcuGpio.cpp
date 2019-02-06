#include "mcuGpio.h"
#include "wiring_digital.h"

//设置管脚
void mcuGpio::SetPin(Pin pin)
{
	this->_Pin = pin;
	this->_pin = 1 << (pin % 16);// GPIO_Pin_0;
}

/**
  *@brief    GPIO模式设置
  *@param    mode:PIN_MODE枚举变量类型
  *@retval   None
  */
void mcuGpio::mode(PIN_MODE mode)
{
	pinMode(this->_Pin, mode);
}

/**
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::read(uint8_t *val)
{
	*val = this->read();
}


/**
 *@brief    读取GPIO引脚状态
 *@param    NONE
 *@retval   返回引脚当前的电平状态
*/
uint8_t mcuGpio::read(void)
{
	return digitalRead(this->_Pin);
}

/**
 *@brief    GPIO输出电平翻转
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::toggle()
{
	digitalToggle(this->_Pin);
}

// 相当与read()
int mcuGpio::operator =(mcuGpio &)
{
	return this->read();
}
// 相当与read()
mcuGpio::operator int()
{
	return this->read();
}

// 读取本组所有引脚，任意脚为true则返回true，主要为单一引脚服务
bool mcuGpio::Read()
{
	return this->Invert ? !read() : read();
}

void mcuGpio::Write(Pin pin, bool value)
{
	digitalWrite(pin, value);
}
void mcuGpio::Write(bool value)
{
	if (this->_Pin == P0)
		return;
	this->Write(this->_Pin, this->Invert ? !value : value);
}

bool mcuGpio::Open()
{
	return true;
}
////////////////////////////////////////////////////////////////////

mcuGpio::mcuGpio()
{
	this->OpenDrain = 0;
}

mcuGpio::mcuGpio(Pin pin, uint8_t invert, bool openDrain)
{
	this->Opened = false;

	this->Invert = 2;
	this->OpenDrain = false;
	this->OpenDrain = openDrain;
	this->Invert = invert;
	if (pin != P0)
	{
		this->Set(pin);
		this->Open();
	}
}
