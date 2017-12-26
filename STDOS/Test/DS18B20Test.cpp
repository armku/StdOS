#include "Drivers\DS18B20.h"
#include "Sys.h"

#define DEBUGds18b20test

#ifdef DEBUGds18b20test
  DS18B20 ds18;

  #if 0
    void ds18b20routin(void *param)
    {
      debug_printf("温度 %.1f \r\n", ds18.GetTemp());
    }
    void ds18b20test()
    {
      #ifdef STM32F1
        #if 0
          ds18.SetPin(PB10);
        #else 
          ds18.SetPin(PD6);
        #endif 
      #elif defined STM32F4
        ds18.SetPin(PB1);
      #endif 
      for (int i = 0; i < 100; i++)
      {
        if (ds18.Init())
          debug_printf("no ds18b20 exit \r\n");
      }
      Sys.AddTask(ds18b20routin, 0, 100, 1000, "ds18b20routin");
    }
  #else 
    uint8_t uc, ucDs18b20Id[8];
    void ds18b20routin(void *param)
    {
      debug_printf("温度 %.1f \r\n", ds18.DS18B20_GetTemp_MatchRom(ucDs18b20Id))
        ;
    }
    void ds18b20test()
    {
      debug_printf("\r\n this is a ds18b20 test demo \r\n");
		return;
      while (ds18.DS18B20_Init())
        debug_printf("\r\n no ds18b20 exit \r\n");

      debug_printf("\r\n ds18b20 exit \r\n");


      ds18.DS18B20_ReadId(ucDs18b20Id); // 读取 DS18B20 的序列号

      debug_printf("\r\nDS18B20的序列号是： 0x");

      for (uc = 0; uc < 8; uc++)
      // 打印 DS18B20 的序列号
        debug_printf("%.2x", ucDs18b20Id[uc]);
      Sys.AddTask(ds18b20routin, 0, 100, 1000, "ds18b20routin");
    }
  #endif 

#endif
