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
            SoftI2C IIC; // I2C通信口
            byte Address; // 设备地址

            AT24CXX();
            virtual ~AT24CXX();

            bool Write(ushort addr, byte data);
            byte Read(ushort addr);
			
            virtual bool Write(uint addr, const Buffer &bs)const;
            virtual bool Read(uint addr, Buffer &bs)const;
        public:
            AT24CXX(Pin pinsck, Pin pinsda, EW24XXType devtype, byte devaddr = 0xA0, uint wnms = 5); //写延时时间       
            byte CheckOk();
            int Read(uint addr, void *pBuffer, int size, ushort bufpos = 0);
            int Write(uint addr, void *pBuffer, int size, ushort bufpos = 0);
            
            OutputPort *pinWP; //保护引脚
        private:
            byte checkDevice();
            ushort jsPageSize(uint type); //计算存储页大小
            int writePage(byte *buf, ushort bufpos, ushort addr, uint size); //页内写
            int readPage(byte *buf, ushort bufpos, ushort addr, uint size); //页内读
            int bufwr(ushort addr, byte *buf, uint size, ushort bufpos, byte wr); //读写集中操作1写 0读

            EW24XXType deviceType; //器件类型
            uint pageSize; //存储页大小
            uint writedelaynms; //写延时	  
    };

#endif
