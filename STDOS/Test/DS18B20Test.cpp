#include "Drivers\DS18B20.h"
#include "Sys.h"

#ifdef DEBUG
	DS18B20 ds18;
    void ds18b20routin(void *param)
    {
        printf("ÎÂ¶È %.1f \r\n", ds18.GetTemp());
    }
    void ds18b20test()
    {	
		#ifdef STM32F1
		#if 1
			ds18.SetPin(PB10);
		#else
			ds18.SetPin(PD6);
		#endif
		#elif defined STM32F4
			ds18.SetPin(PB1);
		#endif
        for(int i=0;i<100;i++)
		{
			if(ds18.Init())        
            printf("no ds18b20 exit \r\n");
        }
        Sys.AddTask(ds18b20routin, 0, 100, 1000, "ds18b20routin");
    }

#endif
