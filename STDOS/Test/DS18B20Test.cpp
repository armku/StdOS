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
		ds18.SetPin(PB10);
        while (ds18.Init())
        {
            printf("no ds18b20 exit \r\n");
        }
        Sys.AddTask(ds18b20routin, 0, 100, 1000, "ds18b20routin");
    }

#endif
