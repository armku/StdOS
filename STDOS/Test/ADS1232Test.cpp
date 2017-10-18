#include "Drivers\ADS1232.h"

//≤‚ ‘1232
#ifdef DEBUG
ADS1232 ads1232;
void ADS1232Test()
{
	ads1232.SetPin(PB15,PB13,PD5);
	ads1232.Init();
}
#endif
