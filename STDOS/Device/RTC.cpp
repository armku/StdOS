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
