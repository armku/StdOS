#include "TTime.h"

TTime::TTime()
{
}
//���ص�ǰ������
uint TTime::Ms()
{
	return this->Current;
}
//����ϵͳ��ǰʱ��,Unix��ʽ����
void TTime::SetTime(UInt64 seconds)
{
	
}
//ϵͳʱ��
uint TTime::Seconds()
{
	return 0;
}
//��ʱms
void TTime::Sleep(uint ms)
{
	
}
uint TTime::TicksToUs(uint ticks) const
{
	if(!ticks)		
	{
		return 0;
	}
	//1000000 = 64 * 15625 = 2^6 * 15625
	//(0xFFFFFFFF / (1000000 >> 6)) = 274877.90688
	//���TICKS<274877,��ô�˷��������
	if(ticks <= (0xffffffff / (1000000 >> 6)))
	{
		return (ticks * (1000000 >> 6)) >> (15-6);
	}
	else
	{
		return ((UInt64)ticks * (1000000 >> 6)) >> (16-6);
	}
}
uint TTime::UsToTicks(uint us) const
{
	if(!us) 
	{
		return 0;
	}
	
	//1000000 = 64 * 15625 = 2^6 * 15625
	//((0xffffffff +1) >> (15-6)) = 8388608 = 0x800000 = 1 << (32 - 9)
	//���ticks<274877,��ô�˷��������
	if(us < (1 << (32 - (15 - 6))))
	{
		return (us << (15 - 6)) / (1000000 >> 6);
	}
	else
	{
		return ((UInt64)us << (15 - 6)) / (1000000 >> 6);
	}
}

