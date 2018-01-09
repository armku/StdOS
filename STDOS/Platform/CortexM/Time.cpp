#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"

Timer *timerTick;
Delegate < Timer & > abc;
void timTickrefesh(void *param)
{
    Time.Milliseconds += 1000;
    Time.Seconds++;
}
int gTicks = 0; //每个us需要的systick时钟数 	

void TTime::OnHandler(ushort num, void *param)
{
//	if(num && TIM_GetITStatus(TIM3,TIM_IT_Update))
//	{
//		++Time.Seconds;
//		Time.Milliseconds +=1000;
//		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
//		if(Time.OnSave)
//			Time.OnSave();
//	}
}

uint TTime::TicksToUs(uint ticks)const
{    	
	 uint result;

  if ( ticks )
    result = ticks / gTicks;
  else
    result = 0;
  return result;
	
}

uint TTime::UsToTicks(uint us)const
{
    uint result;

  if ( us )
    result = us * gTicks;
  else
    result = 0;
  return result;
}
