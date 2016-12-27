#pragma once

#include <stdint.h>
#include "BasePort.h"

#define MAXCH	8
#define CHDELAY	2	//ͨ���л���ʱʱ�䣬���������ȶ�

class CAD7689
{
    public:
        CAD7689(PinPort pinsck, PinPort pinsdi, PinPort pinsdo, PinPort pincnv);
        ~CAD7689();
    public:
        void Routin(void); //��ȡADֵ,���л�ͨ��
        void Init(void); //
        float getVolt(uint16_t ch); //��ȡͨ����ѹֵ
    public:
        byte FlagEnable[MAXCH]; //�Ƿ�����ͨ��ת�� 0����ֹ��1������ ����
    private:
        void Delay(uint32_t nTime);
        uint16_t AD_Read(void);
        uint16_t AD_Write(uint16_t sdat);
    private:
        uint16_t ADBuf[MAXCH]; //ԭʼADֵ
        float volt[MAXCH]; //��ѹֵ
        byte chCurrent; //��ǰͨ��
        byte chDelayCnt; //ͨ����ʱʱ�䣬���������ȶ�
        BasePort *ppinsck;
        BasePort *ppinsdi;
        BasePort *ppinsdo;
        BasePort *ppincnv;
        byte getNextCH(); //��ȡ�´�ת��ͨ��
};
