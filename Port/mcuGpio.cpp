#include "mcuGpio.h"
#include "wiring_digital.h"

//���ùܽ�
void mcuGpio::SetPin(Pin pin)
{
	this->_Pin = pin;
	this->_pin = 1 << (pin % 16);// GPIO_Pin_0;
}

/**
  *@brief    GPIOģʽ����
  *@param    mode:PIN_MODEö�ٱ�������
  *@retval   None
  */
void mcuGpio::mode(PIN_MODE mode)
{
	pinMode(this->_Pin, mode);
}

/**
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::read(uint8_t *val)
{
	*val = this->read();
}


/**
 *@brief    ��ȡGPIO����״̬
 *@param    NONE
 *@retval   �������ŵ�ǰ�ĵ�ƽ״̬
*/
uint8_t mcuGpio::read(void)
{
	return digitalRead(this->_Pin);
}

/**
 *@brief    GPIO�����ƽ��ת
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::toggle()
{
	digitalToggle(this->_Pin);
}

// �൱��read()
int mcuGpio::operator =(mcuGpio &)
{
	return this->read();
}
// �൱��read()
mcuGpio::operator int()
{
	return this->read();
}

// ��ȡ�����������ţ������Ϊtrue�򷵻�true����ҪΪ��һ���ŷ���
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
