#include "PwmSolo.h"
#include "stm32f10x.h"


PwmSolo::PwmSolo(Pin pin, uint frq, uint duty): AlternatePort(pin)
{
    this->freq = frq;
    this->duty = duty;
}
void PwmSolo::Init()
{
	this->OnInit();
}
//ÉèÖÃÊä³ö±ÈÀı£¬0-100
void PwmSolo::SetOutPercent(float per)
{
    this->duty = this->freq *per / 100;
    this->SetOut(this->duty);
}
