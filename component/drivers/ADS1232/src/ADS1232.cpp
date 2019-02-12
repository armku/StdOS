#include "ADS1232.h"
#include "Sys.h"
#include "arduino.h"

void ADS1232::SetPin(Pin pdout, Pin psclk, Pin ppwdn)
{
    this->dout.Set(pdout);
    this->sclk.Set(psclk);
    this->pwdn.Set(ppwdn);

    this->dout.Invert = false;
    this->sclk.Invert = false;
    this->pwdn.Invert = false;
	
    this->dout.mode(PIN_MODE::INPUT);
    this->sclk.mode(PIN_MODE::OUTPUT_OD);
    this->pwdn.mode(PIN_MODE::OUTPUT_OD);

    //this->dout = 1;
    this->sclk = 0;
    this->pwdn = 1;
}

void ADS1232::SetPinSpeed(Pin pin)
{
    this->pSpeed.Set(pin);
    this->pSpeed.Invert = false;
    this->pSpeed.mode(PIN_MODE::OUTPUT_OD_PU);
}

void ADS1232::SetPinGain(Pin gain0, Pin gain1)
{
    this->pGain0.Set(gain0);
    this->pGain1.Set(gain1);

    this->pGain0.Invert = false;
    this->pGain1.Invert = false;
	    
    this->pGain0.mode(PIN_MODE::OUTPUT_OD_PU);
    this->pGain1.mode(PIN_MODE::OUTPUT_OD_PU);
}

void ADS1232::SetSpeed(Speed speed)
{
    if (this->pSpeed._Pin != P0)
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
}

void ADS1232::SetGain(Gain gain)
{
    if ((this->pGain0._Pin != P0) && (this->pGain1._Pin != P0))
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
}

void ADS1232::Init()
{   
    this->pwdn = 0; //复位1232，操作前先复位
	delayMicroseconds(20);
    this->sclk = 0;
	delayMicroseconds(40);
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

	msnew = sys.Ms();
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
		sys.GlobalDisable();
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
		sys.GlobalEnable();
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
