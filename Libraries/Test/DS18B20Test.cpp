#include "Drivers\DS18B20.h"
#include "Sys.h"

//#define _DS18B20_TEST_CPP

#ifdef _DS18B20_TEST_CPP
	DS18B20 ds18;

	void ds18b20routin(void *param)
	{
	  debug_printf("温度 %.1f \r\n", ds18.GetTemp());
	}

	void ds18b20test()
	{
	#ifdef STM32F0
		ds18.SetPin(PE4);
	  #elif defined STM32F1
		#if 0
		  ds18.SetPin(PB10);
		#else 
		  ds18.SetPin(PD6);
		#endif 
	  #elif defined STM32F4
		ds18.SetPin(PG9);
	  #endif 
	  int i;
		ds18.Init();
	  for( i = 0; i < 100; i++)
	  {
		if (ds18.Presence())
		  break;
	  }
	  if(i>=100)
		  debug_printf("no ds18b20 exit \r\n");
	  ds18.ReadId();
	  debug_printf("\r\nDS18B20的序列号是： 0x");
	  for (int uc = 0; uc < 8; uc ++ )             // 打印 DS18B20 的序列号
		debug_printf ( "%.2X", ds18.id[ uc ] );
	  debug_printf("\r\n");
	  Sys.AddTask(ds18b20routin, 0, 100, 1000, "ds18b20routin");
	}
#endif
