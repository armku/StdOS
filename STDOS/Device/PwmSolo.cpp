#include "PwmSolo.h"
#include "stm32f10x.h"

PwmSolo::PwmSolo(uint frq, uint duty)
{
    this->freq = frq;
    this->duty = duty;
}
void PwmSolo::Open()
{
	this->OnOpen();
	AlternatePort::Open();	
}
//ÉèÖÃÊä³ö±ÈÀı£¬0-100
void PwmSolo::SetOutPercent(float per)
{
    this->duty = this->freq *per / 100;
    this->SetOut(this->duty);
}
