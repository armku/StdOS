#ifndef _AD7689_H
#define _AD7689_H

    #include "Device\Port.h"

    #define MAXCH	8
    #define CHDELAY	2	//通道切换延时时间，用于数据稳定

    class CAD7689
    {
        public:
            byte FlagEnable[MAXCH]; //是否允许通道转换 0：禁止，1：允许 备用
        public:
            CAD7689();
			CAD7689(Pin pinsck, Pin pinsdi, Pin pinsdo, Pin pincnv);
			void SetPin(Pin pinsck, Pin pinsdi, Pin pinsdo, Pin pincnv);
            void Routin(void); //读取AD值,并切换通道
            void Init(void); //
            float getVolt(ushort ch); //获取通道电压值    
		private:
            OutputPort ppinsck;
            OutputPort ppinsdi;
            InputPort ppinsdo;
            OutputPort ppincnv;

            float volt[MAXCH]; //电压值
            ushort ADBuf[MAXCH]; //原始AD值
            byte chCurrent; //当前通道
            byte chDelayCnt; //通道延时时间，用于数据稳定        
		private:
            void Delay(uint nTime);
            ushort AD_Read(void);
            ushort AD_Write(ushort sdat);
			byte getNextCH(); //获取下次转换通道		
    };
#endif
