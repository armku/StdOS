#pragma once

#include "Type.h"

#ifdef __cplusplus
    extern "C"
    {
    #endif 
	#define delay_ostickspersec 1000			//Ê±ÖÓÆµÂÊ
		
    void delay_init(void);
    void delay_ms(ushort nms);
    void delay_us(uint nus);
    #ifdef __cplusplus
    }
#endif
