#include "Pwm.h"


CPWM::CPWM(PinPort pin,uint32_t frq,uint16_t duty)
{
	this->pin=new BasePort(pin);
	this->freq=frq;
	this->duty=duty;
}
CPWM::~CPWM()
{
}
uint8_t CPWM::Init()
{
	
	return 0;
}
