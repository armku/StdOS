#pragma once 
#include <stdint.h>
#include "SpiSoft.h"
class CADS1246
{
    public:
        CADS1246(uint16_t pincs, uint16_t pinsck, uint16_t pindin, uint16_t pindout, uint16_t pinrd, uint16_t pinreset);
        ~CADS1246();
    public:
        uint8_t ReadReg(uint8_t RegAddr);
        void WriteReg(uint8_t RegAddr, uint8_t da);
        float Read(void); //����-1,��ʾת��δ���
        void Init(void);
        uint8_t Check(void); //AD��飬��������0
    public:
        CSpiSoft *pspi; //SPI�ӿ�
        CPort *ppinrd; //����׼���ýӿ�
        CPort *ppinreset; //��λ�ӿ�
    private:
        float decodead(uint8_t *da);

};
