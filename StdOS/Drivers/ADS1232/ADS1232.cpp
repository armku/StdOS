#include "ADS1232.h"
#include "Sys.h"

void ADS1232::SetPin(Pin pdout, Pin psclk, Pin ppwdn)
{
    this->dout.Set(pdout);
    this->sclk.Set(psclk);
    this->pwdn.Set(ppwdn);

    //	this->dout.OpenDrain=false;
    this->sclk.OpenDrain = false;
    this->pwdn.OpenDrain = false;

    this->dout.Open();
    this->sclk.Open();
    this->pwdn.Open();

    //this->dout = 1;
    this->sclk = 0;
    this->pwdn = 1;
}

void ADS1232::SetPinSpeed(Pin pin)
{
    this->pSpeed.Set(pin);
    this->pSpeed.OpenDrain = false;
    this->pSpeed.Open();
}

void ADS1232::SetPinGain(Pin gain0, Pin gain1)
{
    this->pGain0.Set(gain0);
    this->pGain1.Set(gain1);

    this->pGain0.OpenDrain = false;
    this->pGain1.OpenDrain = false;

    this->pGain0.Open();
    this->pGain1.Open();
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
    Sys.Delay(20);
    this->sclk = 0;
    Sys.Delay(20);
    Sys.Delay(20);
    //    this->dout = 1; //初始化引脚
    this->pwdn = 1; //开启1232
    Sys.Delay(20);
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

	msnew = Sys.Ms();
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
	//Sys.Delay(20);
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
