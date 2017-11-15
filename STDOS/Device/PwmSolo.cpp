#include "PwmSolo.h"

PwmSolo::PwmSolo(uint frq, uint duty,bool invert)
{
    this->_freq = frq;
    this->_duty = duty;
	this->_invert=invert;
}
void PwmSolo::Open()
{
	AlternatePort::Open();
	this->OnOpen2();		
}
//ÉèÖÃÊä³ö±ÈÀı£¬0-100
void PwmSolo::SetOutPercent(float per)
{
    this->_duty = this->_freq *per / 100;
    this->SetOut(this->_duty);
}
