#pragma once 
#include <stdint.h>
#include "SoftSpi.h"
class CADS1246
{
    public:
        CADS1246(PinPort pincs, PinPort pinsck, PinPort pindin, PinPort pindout, PinPort pinrd, PinPort pinreset);
        ~CADS1246();
    public:
        uint8_t ReadReg(uint8_t RegAddr);
        void WriteReg(uint8_t RegAddr, uint8_t da);
        float Read(void); //����-1,��ʾת��δ���
        void Init(void);
        uint8_t Check(void); //AD��飬��������0
    private:
        CSoftSpi *pspi; //SPI�ӿ�
        BasePort *ppinrd; //����׼���ýӿ�
        BasePort *ppinreset; //��λ�ӿ�
    private:
        float decodead(uint8_t *da);

};
