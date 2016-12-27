#pragma once

#include <stdint.h>
#include "BasePort.h"

#define MAXCH	8
#define CHDELAY	2	//通道切换延时时间，用于数据稳定

class CAD7689
{
    public:
        CAD7689(PinPort pinsck, PinPort pinsdi, PinPort pinsdo, PinPort pincnv);
        ~CAD7689();
    public:
        void Routin(void); //读取AD值,并切换通道
        void Init(void); //
        float getVolt(uint16_t ch); //获取通道电压值
    public:
        byte FlagEnable[MAXCH]; //是否允许通道转换 0：禁止，1：允许 备用
    private:
        void Delay(uint32_t nTime);
        uint16_t AD_Read(void);
        uint16_t AD_Write(uint16_t sdat);
    private:
        uint16_t ADBuf[MAXCH]; //原始AD值
        float volt[MAXCH]; //电压值
        byte chCurrent; //当前通道
        byte chDelayCnt; //通道延时时间，用于数据稳定
        BasePort *ppinsck;
        BasePort *ppinsdi;
        BasePort *ppinsdo;
        BasePort *ppincnv;
        byte getNextCH(); //获取下次转换通道
};
