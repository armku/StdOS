#pragma once 
#include <stdint.h>
#include "Port.h"

class CPWM
{
	public:
		CPWM(uint16_t pin,uint32_t frq=10000,uint16_t duty=50);
		~CPWM();
		uint8_t Init();
	private:
		CPort *pin;
		uint32_t freq;
		uint16_t duty;
};
