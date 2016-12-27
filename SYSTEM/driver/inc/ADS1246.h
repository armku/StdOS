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
        float Read(void); //返回-1,表示转换未完成
        void Init(void);
        uint8_t Check(void); //AD检查，正常返回0
    private:
        CSoftSpi *pspi; //SPI接口
        BasePort *ppinrd; //数据准备好接口
        BasePort *ppinreset; //复位接口
    private:
        float decodead(uint8_t *da);

};
