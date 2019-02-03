#ifndef _AD7689_H
#define _AD7689_H

#include "../../../../Bsp/Porting.h"

    #define MAXCH	8
    #define CHDELAY	2	//通道切换延时时间，用于数据稳定

    class CAD7689
    {
        public:
            uint8_t FlagEnable[MAXCH]; //是否允许通道转换 0：禁止，1：允许 备用
        public:
            CAD7689();
			void SetPin(Pin pinsck, Pin pinsdi, Pin pinsdo, Pin pincnv);
            void Routin(void); //读取AD值,并切换通道
            void Init(void); //
            float getVolt(uint16_t ch); //获取通道电压值    
		private:
			mcuGpio ppinsck;
			mcuGpio ppinsdi;
            mcuGpio ppinsdo;
			mcuGpio ppincnv;

            float volt[MAXCH]; //电压值
            uint16_t ADBuf[MAXCH]; //原始AD值
            uint8_t chCurrent; //当前通道
            uint8_t chDelayCnt; //通道延时时间，用于数据稳定        
		private:
            uint16_t AD_Read(void);
            uint16_t AD_Write(uint16_t sdat);
			uint8_t getNextCH(); //获取下次转换通道		
    };
#endif
