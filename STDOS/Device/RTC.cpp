#include "RTC.h"
#include "TTime.h"

HardRTC::HardRTC()
{
    this->LowPower = true;
    this->External = false;
}

HardRTC *HardRTC::Instance()
{
    return new HardRTC();
}

void HardRTC::Start(bool lowpower, bool external)
{
    lowpower = lowpower;
    external = external;
}
//设置时间-北京时间格式
void HardRTC::SetTime(DateTime & dt)
{
	this->SetTime(dt.TotalSeconds());
}
