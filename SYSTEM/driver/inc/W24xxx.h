#pragma once 
#include "SoftI2C.h"

#define EEPTEST 1
typedef enum
{
    AT24C01 = 127, AT24C02 = 255, AT24C04 = 511, AT24C08 = 1023, AT24C16 = 2047, AT24C32 = 4095, AT24C64 = 8191, AT24C128 = 16383, AT24C256 = 32767, AT24C512 = (32767 *2), AT24C1024 = (32767 *4), AT24C2048 = (32767 *8)
} EW24XXType;


class CW24xxx
{
    public:
        CW24xxx(PinPort pinsck, PinPort pinsda, EW24XXType devtype, byte devaddr = 0xA0, uint32_t wnms = 5); //д��ʱʱ��
        ~CW24xxx();
        byte CheckOk();
    public:
        #if EEPTEST
            void Test(void);
        #endif 
        byte ReadBytes(byte *_pReadBuf, uint16_t bufpos, uint16_t _usAddress, uint32_t _usSize);
        byte WriteBytes(byte *_pWriteBuf, uint16_t bufpos, uint16_t _usAddress, uint32_t _usSize);
        byte ReadByte(uint16_t address);
        byte WriteByte(uint16_t address, byte da);
    private:
        byte checkDevice();
        EW24XXType deviceType; //��������
        byte devAddr; //�豸��ַ
        uint32_t pageSize; //�洢ҳ��С
        CSoftI2C *pi2c;
        uint32_t writedelaynms; //д��ʱ	
    private:
        uint16_t jsPageSize(uint32_t type); //����洢ҳ��С
        byte writePage(byte *buf, uint16_t bufpos, uint16_t addr, uint32_t len); //ҳ��д
        byte readPage(byte *buf, uint16_t bufpos, uint16_t addr, uint32_t len); //ҳ�ڶ�
        byte bufwr(byte *buf, uint16_t bufpos, uint16_t addr, uint32_t len, byte wr); //��д���в���1д 0��
};
