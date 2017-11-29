#include "Drivers\Esp8266.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "stm32f10x.h" 
#include "SerialPort.h"
#include "Sys.h"

#define ESP8266TEST

#ifdef ESP8266TEST
   

    Esp8266 esp;

    void espRoutin(void *param)
    {
		esp.Routin(param);        
    }
    uint OnUsart3Read(ITransport *transport, Buffer &bs, void *para, void *para2)
    {
        //transport->Write(bs);
//        debug_printf("收到数据:\r\n");
//        bs.Show(true);
		
		SerialPort *sp=(SerialPort*)transport;
		esp.OnReceive(bs);
		
		
		
        return 0;
    }
	
    /**
     * @brief  ESP8266 （Sta Tcp Client）透传
     * @param  无
     * @retval 无
     */
    void ESP8266Test()
    {
        esp.SetPin(PG13, PG14);
		esp.Init(COM3,115200);
        
        esp.Port->Register(OnUsart3Read);
        esp.Port->Open();

        //esp.SetSerialPort(&sp3);
		
        esp.Init();

        Sys.AddTask(espRoutin, 0, 0, 500, "espRoutin");

    }
#endif
