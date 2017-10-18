#include "ADS1232.h"

void delay_short(int us)
{
	Sys.Delay(10);
}

void ADS1232::SetPin(Pin pdout, Pin psclk, Pin ppwdn)
{
    this->dout.Set(pdout);
    this->sclk.Set(psclk);
    this->pwdn.Set(ppwdn);
	
	this->dout.OpenDrain=false;
	this->sclk.OpenDrain=false;
	this->pwdn.OpenDrain=false;
	
	//this->dout.Open();
	//this->sclk.Open();
	this->pwdn.Open();
	
	this->dout = 1;
	this->sclk = 1;
	this->pwdn = 1;
}

void ADS1232::Init()
{
	this->pwdn = 0; //复位1232，操作前先复位
    delay_short(1000);
    this->sclk = 0;
    delay_short(1000);
    delay_short(1000);
    this->dout = 1; //初始化引脚
    this->pwdn = 1; //开启1232
    delay_short(1000);
	this->readCnt=0;
}


int ADS1232::Read()
{
	int temp = 0;
    byte i = 0;
    //        for(temp=0;temp<500;temp++)
    //        {
    //            if (0 == ADS1232_DOUT)
    //            {
    //                break;
    //            }
    //        }
    this->sclk = 0;
    for (temp = 0; temp < 1000; temp++)
    {
        if (!this->dout)
        {
            break;
        }
    }
    delay_short(33);
    if (temp < 1000)
    {
        temp = 0;
        //this->sclk=0;
        //this->sclk=1;
        for (i = 0; i < 24; i++)
        {
            this->sclk = 1;

            this->sclk = 0;

            temp <<= 1;
            if (1 == this->dout)
            {
                temp |= 0x01;
            }
            // this->sclk=1;
        }
        //            this->sclk=1; // The 25th SCLK to force DOUT high
        //            this->sclk=0;
        this->sclk = 1;

        this->sclk = 0;
        if (temp &0x800000)
        {
            temp ^= 0Xffffff;
            temp =  - (temp + 1);
        }
        else
        {
            temp &= 0X7fffff;
        }
        this->oldval = temp;
    }
	this->readCnt++;
    return this->oldval;
}
