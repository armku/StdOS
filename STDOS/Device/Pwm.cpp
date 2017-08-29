#include "Pwm.h"

// index 定时器编号
Pwm::Pwm(TIMER index):Timer(index)
{
	
}
Pwm::~Pwm()
{
	
}

// 设置占空比，0~255
void Pwm::SetDuty(int idx, byte duty)
{
}
byte Pwm::GetDuty(int idx)
{
	return 0;
}
PwmData::PwmData(TIMER index):Pwm(index)
{
	
}
