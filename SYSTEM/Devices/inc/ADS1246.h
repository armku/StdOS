#pragma once 
#include "Type.h"
#include "SoftSpi.h"
#include "Port.h"
class CADS1246
{
    public:
        CADS1246(Pin pincs, Pin pinsck, Pin pindin, Pin pindout, Pin pinrd, Pin pinreset);       
        byte ReadReg(byte RegAddr);
        void WriteReg(byte RegAddr, byte da);
        float Read(void); //����-1,��ʾת��δ���
        void Init(void);
        byte Check(void); //AD��飬��������0
    private:
        CSoftSpi *pspi; //SPI�ӿ�
        OutputPort ppinrd; //����׼���ýӿ�
        OutputPort ppinreset; //��λ�ӿ�
    private:
        float decodead(byte *da);

};
