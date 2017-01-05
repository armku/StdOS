#pragma once

#include "Type.h"
#include "OutputPort.h"
#include "InputPortNew.h"

#define MAXCH	8
#define CHDELAY	2	//ͨ���л���ʱʱ�䣬���������ȶ�

class CAD7689
{
    public:
        CAD7689(Pin pinsck, Pin pinsdi, Pin pinsdo, Pin pincnv);
        ~CAD7689();  
        void Routin(void); //��ȡADֵ,���л�ͨ��
        void Init(void); //
        float getVolt(ushort ch); //��ȡͨ����ѹֵ    
        byte FlagEnable[MAXCH]; //�Ƿ�����ͨ��ת�� 0����ֹ��1������ ����
    private:
        void Delay(uint nTime);
        ushort AD_Read(void);
        ushort AD_Write(ushort sdat);    
        ushort ADBuf[MAXCH]; //ԭʼADֵ
        float volt[MAXCH]; //��ѹֵ
        byte chCurrent; //��ǰͨ��
        byte chDelayCnt; //ͨ����ʱʱ�䣬���������ȶ�
        OutputPort ppinsck;
        OutputPort ppinsdi;
        InputPortNew ppinsdo;
        OutputPort ppincnv;
        byte getNextCH(); //��ȡ�´�ת��ͨ��
};
