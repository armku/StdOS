#ifndef _AT24CXX_H_
#define _AT24CXX_H_

    #include "Device\I2C.h"
    #include "Storage\Storage.h"
    typedef enum
    {
        AT24C01 = 127,  //
        AT24C02 = 255,  //
        AT24C04 = 511,  //
        AT24C08 = 1023,  //
        AT24C16 = 2047,  //
        AT24C32 = 4095,  //
        AT24C64 = 8191,  //
        AT24C128 = 16383,  //
        AT24C256 = 32767,  //
        AT24C512 = (32767 *2),  //
        AT24C1024 = (32767 *4),  // 
        AT24C2048 = (32767 *8) //
    } EW24XXType;
    // EEPROM
    class AT24CXX: public CharStorage
    {
        public:
            SoftI2C IIC; // I2Cͨ�ſ�
            byte Address; // �豸��ַ

            AT24CXX();
            virtual ~AT24CXX();

            bool Write(ushort addr, byte data);
            byte Read(ushort addr);
			
            virtual bool Write(uint addr, const Buffer &bs)const;
            virtual bool Read(uint addr, Buffer &bs)const;
        public:
            AT24CXX(Pin pinsck, Pin pinsda, EW24XXType devtype, byte devaddr = 0xA0, uint wnms = 5); //д��ʱʱ��       
            byte CheckOk();
            int Read(uint addr, void *pBuffer, int size, ushort bufpos = 0);
            int Write(uint addr, void *pBuffer, int size, ushort bufpos = 0);
            
            OutputPort *pinWP; //��������
        private:
            byte checkDevice();
            ushort jsPageSize(uint type); //����洢ҳ��С
            int writePage(byte *buf, ushort bufpos, ushort addr, uint size); //ҳ��д
            int readPage(byte *buf, ushort bufpos, ushort addr, uint size); //ҳ�ڶ�
            int bufwr(ushort addr, byte *buf, uint size, ushort bufpos, byte wr); //��д���в���1д 0��

            EW24XXType deviceType; //��������
            uint pageSize; //�洢ҳ��С
            uint writedelaynms; //д��ʱ	  
    };

#endif
