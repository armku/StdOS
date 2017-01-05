#pragma once

#include "Type.h"
#include "OutputPort.h"
#include "InputPortNew.h"

#define MAXCH	8
#define CHDELAY	2	//通道切换延时时间，用于数据稳定

class CAD7689
{
    public:
        CAD7689(Pin pinsck, Pin pinsdi, Pin pinsdo, Pin pincnv);
        ~CAD7689();  
        void Routin(void); //读取AD值,并切换通道
        void Init(void); //
        float getVolt(ushort ch); //获取通道电压值    
        byte FlagEnable[MAXCH]; //是否允许通道转换 0：禁止，1：允许 备用
    private:
        void Delay(uint nTime);
        ushort AD_Read(void);
        ushort AD_Write(ushort sdat);    
        ushort ADBuf[MAXCH]; //原始AD值
        float volt[MAXCH]; //电压值
        byte chCurrent; //当前通道
        byte chDelayCnt; //通道延时时间，用于数据稳定
        OutputPort ppinsck;
        OutputPort ppinsdi;
        InputPortNew ppinsdo;
        OutputPort ppincnv;
        byte getNextCH(); //获取下次转换通道
};
