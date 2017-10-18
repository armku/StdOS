#include "ADS1232.h"

void ADS1232::SetPin(Pin pdout, Pin psclk, Pin ppwdn)
{
    this->dout.Set(pdout);
    this->sclk.Set(psclk);
    this->pwdn.Set(ppwdn);
	
	this->dout.Invert=false;
	this->sclk.Invert=false;
	this->pwdn.Invert=false;
	
//	this->dout.OpenDrain=false;
	this->sclk.OpenDrain=false;
	this->pwdn.OpenDrain=false;
	
	this->dout.Open();
	this->sclk.Open();
	this->pwdn.Open();
	
//	this->dout = 1;
	this->sclk = 0;
	this->pwdn = 1;
}
void ADS1232::test()
{
	//this->dout=!this->dout;
}

void ADS1232::Init()
{
	//return;
	this->pwdn = 0; //复位1232，操作前先复位
	Sys.Delay(20);
    this->sclk = 0;
    Sys.Delay(20);
    Sys.Delay(20);
//    this->dout = 1; //初始化引脚
    this->pwdn = 1; //开启1232
    Sys.Delay(20);
	this->readCnt=0;
}


int ADS1232::Read()
{
	return;
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
    Sys.Delay(20);
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
