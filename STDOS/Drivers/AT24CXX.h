#ifndef _AT24CXX_H_
#define _AT24CXX_H_

#include "Device\I2C.h"
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
	AT24C512 = (32767 * 2),  //
	AT24C1024 = (32767 * 4),  // 
	AT24C2048 = (32767 * 8) //
} EW24XXType;
// EEPROM
class AT24CXX
{
public:
	SoftI2C IIC; // I2C通信口
	byte Address; // 设备地址

	bool Write(uint16_t addr, byte data);
	byte Read(uint16_t addr);

	virtual bool Write(uint32_t addr, Buffer &bs);
	virtual bool Read(uint32_t addr, Buffer &bs);
	int Read(uint32_t addr, void *pBuffer, int size, uint16_t bufpos = 0);
	int Write(uint32_t addr, void *pBuffer, int size, uint16_t bufpos = 0);
public:
	AT24CXX(EW24XXType devtype, byte devaddr = 0xA0, uint32_t wnms = 5); //写延时时间
	void SetPin(Pin pinscl, Pin pinsda, Pin pinwriteprotect = P0);
	byte CheckOk();

	OutputPort pinWP; //保护引脚
private:
	byte checkDevice();
	uint16_t jsPageSize(uint32_t type); //计算存储页大小
	int writePage(byte *buf, uint16_t bufpos, uint16_t addr, uint32_t size); //页内写
	int writePage(uint16_t addr, Buffer &bs); //页内写
	int readPage(byte *buf, uint16_t bufpos, uint16_t addr, uint32_t size); //页内读
	int readPage(uint16_t addr, Buffer &bs); //页内读
	int bufwr(uint16_t addr, byte *buf, uint32_t size, uint16_t bufpos, byte wr); //读写集中操作1写 0读
	int bufwr(uint16_t addr, Buffer &bs, byte wr); //读写集中操作1写 0读

	EW24XXType deviceType; //器件类型
	uint32_t pageSize; //存储页大小
	uint32_t writedelaynms; //写延时	  
};

#endif
