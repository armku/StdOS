#pragma once 
#include "Type.h"
#include "SoftSpi.h"
class CADS1246
{
    public:
        CADS1246(PinPort pincs, PinPort pinsck, PinPort pindin, PinPort pindout, PinPort pinrd, PinPort pinreset);
        ~CADS1246();
    public:
        byte ReadReg(byte RegAddr);
        void WriteReg(byte RegAddr, byte da);
        float Read(void); //����-1,��ʾת��δ���
        void Init(void);
        byte Check(void); //AD��飬��������0
    private:
        CSoftSpi *pspi; //SPI�ӿ�
        BasePort *ppinrd; //����׼���ýӿ�
        BasePort *ppinreset; //��λ�ӿ�
    private:
        float decodead(byte *da);

};
