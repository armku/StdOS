#include "Pwm.h"

// index ��ʱ�����
Pwm::Pwm(TIMER index):Timer(index)
{
	
}
Pwm::~Pwm()
{
	
}

// ����ռ�ձȣ�0~255
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
