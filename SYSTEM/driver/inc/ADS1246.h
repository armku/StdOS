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
        float Read(void); //返回-1,表示转换未完成
        void Init(void);
        uint8_t Check(void); //AD检查，正常返回0
    public:
        CSpiSoft *pspi; //SPI接口
        CPort *ppinrd; //数据准备好接口
        CPort *ppinreset; //复位接口
    private:
        float decodead(uint8_t *da);

};
