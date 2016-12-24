#pragma once 
#include "I2CSoft.h"

#define EEPTEST 1
typedef enum
{
    AT24C01 = 127, AT24C02 = 255, AT24C04 = 511, AT24C08 = 1023, AT24C16 = 2047, AT24C32 = 4095, AT24C64 = 8191, AT24C128 = 16383, AT24C256 = 32767, AT24C512 = (32767 *2), AT24C1024 = (32767 *4), AT24C2048 = (32767 *8)
} EW24XXType;


class CW24xxx
{
    public:
        CW24xxx(PinPort pinsck, PinPort pinsda, EW24XXType devtype, uint8_t devaddr = 0xA0, uint32_t wnms = 5); //写延时时间
        ~CW24xxx();
        uint8_t CheckOk();
    public:
        #if EEPTEST
            void Test(void);
        #endif 
        uint8_t ReadBytes(uint8_t *_pReadBuf, uint16_t bufpos, uint16_t _usAddress, uint32_t _usSize);
        uint8_t WriteBytes(uint8_t *_pWriteBuf, uint16_t bufpos, uint16_t _usAddress, uint32_t _usSize);
        uint8_t ReadByte(uint16_t address);
        uint8_t WriteByte(uint16_t address, uint8_t da);
    private:
        uint8_t checkDevice();
        EW24XXType deviceType; //器件类型
        uint8_t devAddr; //设备地址
        uint32_t pageSize; //存储页大小
        CI2CSoft *pi2c;
        uint32_t writedelaynms; //写延时	
    private:
        uint16_t jsPageSize(uint32_t type); //计算存储页大小
        uint8_t writePage(uint8_t *buf, uint16_t bufpos, uint16_t addr, uint32_t len); //页内写
        uint8_t readPage(uint8_t *buf, uint16_t bufpos, uint16_t addr, uint32_t len); //页内读
        uint8_t bufwr(uint8_t *buf, uint16_t bufpos, uint16_t addr, uint32_t len, uint8_t wr); //读写集中操作1写 0读
};
