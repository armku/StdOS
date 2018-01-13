#include "RTC.h"
#include "TTime.h"

static HardRTC ihardRtc;
HardRTC::HardRTC()
{
    this->LowPower = true;
    this->External = false;
}

HardRTC *HardRTC::Instance()
{
    return &ihardRtc;
}

void HardRTC::Start(bool lowpower, bool external)
{
    lowpower = lowpower;
    external = external;
}
//����ʱ��-����ʱ���ʽ
void HardRTC::SetTime(DateTime & dt)
{
	this->SetTime(dt.TotalSeconds());
}
