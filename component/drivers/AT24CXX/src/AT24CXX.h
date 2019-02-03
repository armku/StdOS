#ifndef _AT24CXX_H_
#define _AT24CXX_H_

#include "../../I2C.h"

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
	uint8_t Address; // 设备地址

	bool Write(uint16_t addr, uint8_t data);
	uint8_t Read(uint16_t addr);

	virtual bool Write(uint32_t addr, void * buf, int len, int bufpos = 0);
	virtual bool Read(uint32_t addr, void * buf, int len, int bufpos = 0);
public:
	AT24CXX(EW24XXType devtype, uint8_t devaddr = 0xA0, uint32_t wnms = 5); //写延时时间
	void SetPin(Pin pinscl, Pin pinsda, Pin pinwriteprotect = P0);
	uint8_t CheckOk();

	mcuGpio pinWP; //保护引脚
private:
	uint8_t checkDevice();
	uint16_t jsPageSize(uint32_t type); //计算存储页大小
private:
	int PageRead(uint16_t addr, void * buf, int len);//页内读，最多一页
	int PageWrite(uint16_t addr, void * buf, int len);//页内写，最多一页
	
	EW24XXType deviceType; //器件类型
	uint32_t Block; //存储页大小
	uint32_t writedelaynms; //写延时	  
};

#endif
