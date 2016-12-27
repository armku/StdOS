#pragma once 
#include <stdint.h>
#include "BasePort.h"

class CPWM
{
	public:
		CPWM(PinPort pin,uint32_t frq=10000,uint16_t duty=50);
		~CPWM();
		uint8_t Init();
	private:
		BasePort *pin;
		uint32_t freq;
		uint16_t duty;
};
