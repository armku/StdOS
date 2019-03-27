#include "ADS1232.h"
#include "Sys.h"

void ADS1232::SetPin(Pin pdout, Pin psclk, Pin ppwdn)
{
    this->dout.SetPin(pdout);
    this->sclk.SetPin(psclk);
    this->pwdn.SetPin(ppwdn);

    this->dout.pinMode(GPIO_Out_PP);
	this->sclk.pinMode(GPIO_Out_PP);
    this->pwdn.pinMode(GPIO_Out_PP);

    this->dout = 1;
    this->sclk = 0;
    this->pwdn = 1;
}

void ADS1232::SetPinSpeed(Pin pin)
{
    this->pSpeed.SetPin(pin);
}

void ADS1232::SetPinGain(Pin gain0, Pin gain1)
{
    this->pGain0.SetPin(gain0);
    this->pGain1.SetPin(gain1);
}

void ADS1232::SetSpeed(Speed speed)
{
	switch (speed)
	{
	case SPEED10:
		this->pSpeed = 0;
		break;
	case SPEED80:
		this->pSpeed = 1;
		break;
	default:
		break;
	}
}

void ADS1232::SetGain(Gain gain)
{
	switch (gain)
	{
	case GAIN1:
		this->pGain0 = 0;
		this->pGain1 = 0;
		break;
	case GAIN2:
		this->pGain0 = 1;
		this->pGain1 = 0;
		break;
	case GAIN64:
		this->pGain0 = 0;
		this->pGain1 = 1;
		break;
	case GAIN128:
		this->pGain0 = 1;
		this->pGain1 = 1;
		break;
	default:
		break;
	}
}

void ADS1232::Init()
{   
    this->pwdn = 0; //复位1232，操作前先复位
    delayMicroseconds(20);
    this->sclk = 0;
    delayMicroseconds(20);
    delayMicroseconds(20);
    //    this->dout = 1; //初始化引脚
    this->pwdn = 1; //开启1232
    delayMicroseconds(20);
    this->readCnt = 0;
	this->CalCnt = 0;
}


int ADS1232::Read(bool cal)
{
	static uint64_t msold = 0;
	static uint64_t msnew = 0;
	int temp = 0;
	uint32_t valOrigin = 0;
	int32_t ret = 0;

	msnew = millis();
	this->Status = 0;
	if (msnew - msold > 5000)
	{
		cal = true;
		msold = msnew;
	}
	if (readCnt < 3)
	{
		cal = true;
	}
	
	for (temp = 0; temp < 1000; temp++)
	{
		if (!this->dout)
		{
			break;
		}
	}
	this->Status = temp;
	//delayMicroseconds(20);
	if (temp < 1000)
	{
		valOrigin = 0;
		Sys.GlobalDisable();
		for (int i = 0; i < 24; i++)
		{
			valOrigin <<= 1;
			this->sclk = 1;
			if (this->dout)
			{
				valOrigin |= 0x01;
			}
			this->sclk = 0;
		}
		if (cal)
		{
			this->Status |= 1 << 8;
			this->sclk = 1;
			this->sclk = 0;
			this->sclk = 1;
			this->sclk = 0;
			this->CalCnt++;
		}
		else
		{
			this->sclk = 1;
			this->sclk = 0;
		}
		Sys.GlobalEnable();
		if (valOrigin & 0x800000)
		{
			ret = -((0XFFFFFF ^ valOrigin) + 1);
			this->Status |= 1 << 11;
		}
		else
		{
			ret = 0X7FFFFF & valOrigin;
		}
		this->oldval = ret;
	}
	this->readCnt++;
	return this->oldval;
}
