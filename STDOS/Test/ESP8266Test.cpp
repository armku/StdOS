#include "Drivers\Esp8266.h"
#include <string.h>  
#include <stdbool.h>
#include "stm32f10x.h" 
#include "SerialPort.h"
#include "Sys.h"

//#define ESP8266TEST

#ifdef ESP8266TEST   

    Esp8266 esp;
       
    /**
     * @brief  ESP8266 £¨Sta Tcp Client£©Í¸´«
     * @param  ÎÞ
     * @retval ÎÞ
     */
    void ESP8266Test()
    {
        esp.SetPin(PG13, PG14);
		esp.Init(COM3,115200);
    }
#endif
