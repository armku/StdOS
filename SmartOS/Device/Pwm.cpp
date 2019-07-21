﻿#include "Kernel\Sys.h"

#include "Port.h"
#include "Pwm.h"

Pwm::Pwm(TIMER index) : Timer(index)
{
	for(int i=0; i<4; i++)
	{
		Pulse[i]	= 0xFFFF;
		Ports[i]	= nullptr;
		Enabled[i]	= false;
	}

	//Remap		= 0;
}

Pwm::~Pwm()
{
	for(int i=0; i<4; i++)
	{
		delete Ports[i];
	}
}

void Pwm::SetPulse(int idx, ushort pulse)
{
	if(Pulse[idx] == pulse) return;

	Pulse[idx] = pulse;

	if(Opened)
		Config();
	else
		Open();
}

void Pwm::SetDuty(int idx, byte duty)
{
	if(idx <0 || idx >= 4) return;

	Pulse[idx]	= (((int)duty + 1) * Period) >> 8;
}

byte Pwm::GetDuty(int idx)
{
	if(idx <0 || idx >= 4)	return 0;
	if(Pulse[idx] == 0)		return 0;

	return (Pulse[idx] << 8) / Period - 1;
}

PwmData::PwmData(TIMER index) : Pwm(index)
{
	Pulses		= nullptr;
	PulseCount	= 0;
	Channel		= 0;
	PulseIndex	= 0xFF;
	Repeated	= false;
}
