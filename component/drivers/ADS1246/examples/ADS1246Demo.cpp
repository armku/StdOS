#include "../src/ADS1246.h"

//#define _ADS1246_TEST_CPP

#ifdef _ADS1246_TEST_CPP
	void SetEXIT(int pinIndex, bool enable,InputPort::Trigger trigger);
    InputPort exti_1(PD5); //1246中断接口
    #if ADS1246SPISOFT
        SpiSoft spi1;
    #else 
        Spi spi1(Spi2, CPOL_High, CPHA_1Edge);
    #endif 
    ADS1246 ads1246_1(&spi1, exti_1, PE1, ADC_SPS_80);
    uint32_t adsCnt1 = 0;
    void adRead_1()
    {
        SetEXIT(5, false,InputPort::Both);
        int adtmp = ads1246_1.Read();
        SetEXIT(5, true,InputPort::Both);
    }

    void adRead_1();
    volatile bool flaginad;
    void adRead_1test(Pin pin, bool down, void *param)
    {
        if (down)
        {
            return ;
        }
        if (!flaginad)
        {
            flaginad = true;
            adRead_1();
            flaginad = false;
        }
    }

    void adResetChk(void *param)
    {
        adsCnt1 = ads1246_1.CheckSpeed();
        if (adsCnt1 < 30)
        {
            if (ads1246_1.Online())
            {
                ads1246_1.Reset();
            }
        }
    }

    void ads1246test()
    {
        #if ADS1246SPISOFT
            spi1.SetPin(PB13, PB15, PB14);
        #endif 
        spi1.SetNss(PE0);
        ads1246_1.Init();
//        exti_1.Register(adRead_1test);
        sys.AddTask(adResetChk, 0, 20000, 1000, "adSpeedchk");
    }
#endif
