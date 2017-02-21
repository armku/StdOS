#pragma once

#include "Type.h"
#include "Port.h"
#include "Port.h"

#define MAXCH	8
#define CHDELAY	2	//ͨ���л���ʱʱ�䣬���������ȶ�

class CAD7689
{
    public:
        CAD7689(Pin pinsck, Pin pinsdi, Pin pinsdo, Pin pincnv);        
        void Routin(void); //��ȡADֵ,���л�ͨ��
        void Init(void); //
        float getVolt(ushort ch); //��ȡͨ����ѹֵ    
        
		byte FlagEnable[MAXCH]; //�Ƿ�����ͨ��ת�� 0����ֹ��1������ ����
    private:
        void Delay(uint nTime);
        ushort AD_Read(void);
        ushort AD_Write(ushort sdat);    
        byte getNextCH(); //��ȡ�´�ת��ͨ��
	
		OutputPort ppinsck;
        OutputPort ppinsdi;
        InputPort ppinsdo;
        OutputPort ppincnv;
        
		float volt[MAXCH]; //��ѹֵ
        ushort ADBuf[MAXCH]; //ԭʼADֵ
        byte chCurrent; //��ǰͨ��
        byte chDelayCnt; //ͨ����ʱʱ�䣬���������ȶ�        
};
