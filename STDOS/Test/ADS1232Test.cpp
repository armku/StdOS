#include "Drivers\ADS1232.h"

//测试1232
#ifdef DEBUG
    ADS1232 ads1232;
    int ads1232doutcnt;
    float ads1232value;
    void InputPort_OpenEXTI(Pin pin, InputPort::Trigger trigger);
    void SetEXIT(int pinIndex, bool enable, InputPort::Trigger trigger = InputPort::Both);
    void OnPress1232(InputPort &port, bool down)
    {
        SetEXIT(PB14, true, InputPort::Falling);
        //debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
        ads1232doutcnt++;
        ads1232value = ads1232.Read();
        InputPort_OpenEXTI(PB14, InputPort::Falling);
    }
    InputPort key1232dout(PB14, true, InputPort::DOWN);

    void Show1232cnt(void *param)
    {
        debug_printf("\r\n ADS1232CNT:%d value:%f", ads1232doutcnt);
        ads1232.test();
    }
    void ADS1232Test()
    {
        ads1232.SetPin(PB14, PB13, PD5);
        ads1232.Init();

        key1232dout.Press = OnPress1232;
        key1232dout.UsePress();
        key1232dout.Open();

        Sys.AddTask(Show1232cnt, 0, 1000, 1000, "Show1232cnt");
    }
#endif 

#if 0
    #define AD_Dout  P20
    #define AD_SCLK  P37
    #define AD_PDWN  P36
    #define AD_SPEED P35
    #define AD_GAIN1 P34
    #define AD_GAIN0 P33

    //1.ADC 速度
    extern unsigned char ADC_SPS; //默认80sps
    #define _10_SPS				0
    #define _80_SPS				1

    //2.ADC 增益
    extern unsigned char ADC_GAIN; //默认1

    #define ADC_GX_1			1 
    #define ADC_GX_2			2 
    #define ADC_GX_64			64
    #define ADC_GX_128			128

    //3.ADC 上电校准读取次数
    #define ADC_OFFSET_No		10					//连续读取10次

    //4.ADC 通道定义
    #define ADC_CH0				0				 	//通道0
    #define ADC_CH1				1					//通道1

    void ADS123X_SPEED_CONFIG(unsigned char speed);
    void ADS123X_GAIN_CONFIG(unsigned char gain);
    void ADS123x_Init(void);
    unsigned long int Ads123X_Read(void);

    //1.ADC 速度
    unsigned char ADC_SPS = _10_SPS; //默认80sps
    //2.ADC 增益
    unsigned char ADC_GAIN = ADC_GX_1; //默认1


    //配置速度
    void ADS123X_SPEED_CONFIG(unsigned char speed)
    {
        switch (speed)
        {
            case _10_SPS:
                AD_SPEED = _10_SPS;
                break;
            case _80_SPS:
                AD_SPEED = _80_SPS;
                break;

            default:
                AD_SPEED = _10_SPS;
        }
    }
    //配置放大倍数
    void ADS123X_GAIN_CONFIG(unsigned char gain)
    {
        switch (gain)
        {
            case ADC_GX_1:
                AD_GAIN0 = 0;
                AD_GAIN1 = 0; //GAIN = 1
                break;
            case ADC_GX_2:
                AD_GAIN0 = 1;
                AD_GAIN1 = 0; //GAIN = 2
                break;
            case ADC_GX_64:
                AD_GAIN0 = 0;
                AD_GAIN1 = 1; //GAIN = 64
                break;
            case ADC_GX_128:
                AD_GAIN0 = 1;
                AD_GAIN1 = 1; //GAIN = 128
                break;

            default:
                AD_GAIN0 = 0;
                AD_GAIN1 = 0; //GAIN = 1
        }
    }
    void ADS123x_Init(void)
    {
        unsigned char i = 0;
        AD_PDWN = 0;
        DelayUs(20); //使其复位 		 
        ADS123X_SPEED_CONFIG(_80_SPS);
        ADS123X_GAIN_CONFIG(ADC_GX_128);
        AD_SCLK = 1;
        DelayUs(20);
        AD_SCLK = 0;
        DelayUs(20); //拉高RDY
        AD_PDWN = 1;
        DelayUs(20); //拉高RST
        DelayMs(10); //等待10MS
        AD_Dout = 1;
        DelayMs(10);

        while (AD_Dout)
            ;
        for (i = 0; i < 26; i++)

        //发送26个脉冲，校准                                                                                               //?26?????ADS1232
        {
            AD_SCLK = 1;
            _nop_();
            AD_SCLK = 0;
        }
        while (AD_Dout)
            ;
        for (i = 0; i < 25; i++)

        //25个脉冲接收数据                                                                                       //?25???????
        {
            AD_SCLK = 1;
            _nop_();
            AD_SCLK = 0;
        }
    }
    //读取ADC
    unsigned long int Ads123X_Read(void)
    {
        unsigned char i;
        unsigned long Ads_ReadData;
        while (AD_Dout)
            ;
        for (i = 0; i < 24; i++)
        {
            AD_SCLK = 1; //上升沿读取数据
            _nop_();
            Ads_ReadData <<= 1;
            if (AD_Dout)
            {
                Ads_ReadData |= 0x01;
            }
            AD_SCLK = 0;
            _nop_();
        }
        AD_SCLK = 1;
        _nop_();
        AD_SCLK = 0;
        _nop_();
        AD_SCLK = 1;
        _nop_(); //消除累计误差       
        AD_SCLK = 0;
        _nop_();

        Ads_ReadData &= 0x00ffffffL; //24位有效位变成32位

        if (Ads_ReadData >= 0x800000)
        // 
        {
            Ads_ReadData |= 0xff000000;
        }
        return (Ads_ReadData &0x00FFFFFF); //返回数据
    }
    ////校准 读取ADC
    //unsigned long int Ads123X_Calibrat_Read_3Byte(void)
    ////{
    //   	unsigned char i;
    //		unsigned long Ads_ReadData;
    //		while(AD_Dout)                 
    //		for(i = 0; i < 24; i++)
    //			{
    //				AD_SCLK=1;//上升沿读取数据
    //				_nop_();
    //				Ads_ReadData<<=1;
    //				if(AD_Dout)
    //				{
    //					Ads_ReadData |= 0x01;
    //				}
    //				AD_SCLK=0;
    //				_nop_();
    //			}
    //		AD_SCLK=1;  _nop_();   
    //		AD_SCLK=0;  _nop_();
    //		AD_SCLK=1;  _nop_();  //消除累计误差       
    //		AD_SCLK=0;  _nop_();                  

    //		Ads_ReadData&=0x00ffffffL; //24位有效位变成32位

    //		if(Ads_ReadData>= 0x800000) // 
    //		{  
    //			Ads_ReadData |= 0xff000000;
    //		}
    //					  
    //		return(Data & 0x00FFFFFF);	//返回数据
    //}

#endif
