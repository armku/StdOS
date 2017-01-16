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
        CW24xxx(Pin pinsck, Pin pinsda, EW24XXType devtype, byte devaddr = 0xA0, uint wnms = 5); //д��ʱʱ��       
        byte CheckOk();
        #if EEPTEST
            void Test(void);
        #endif 
        byte ReadBytes(byte *_pReadBuf, ushort bufpos, ushort _usAddress, uint _usSize);
        byte WriteBytes(byte *_pWriteBuf, ushort bufpos, ushort _usAddress, uint _usSize);
        byte ReadByte(ushort address);
        byte WriteByte(ushort address, byte da);
		OutputPort *pinWP;//��������
    private:
        byte checkDevice();
        EW24XXType deviceType; //��������
        byte devAddr; //�豸��ַ
        uint pageSize; //�洢ҳ��С
        CSoftI2C *pi2c;
        uint writedelaynms; //д��ʱ	    
        ushort jsPageSize(uint type); //����洢ҳ��С
        byte writePage(byte *buf, ushort bufpos, ushort addr, uint len); //ҳ��д
        byte readPage(byte *buf, ushort bufpos, ushort addr, uint len); //ҳ�ڶ�
        byte bufwr(byte *buf, ushort bufpos, ushort addr, uint len, byte wr); //��д���в���1д 0��
};
