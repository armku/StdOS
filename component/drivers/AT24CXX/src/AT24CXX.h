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
	SoftI2C IIC; // I2Cͨ�ſ�
	uint8_t Address; // �豸��ַ

	bool Write(uint16_t addr, uint8_t data);
	uint8_t Read(uint16_t addr);

	virtual bool Write(uint32_t addr, void * buf, int len, int bufpos = 0);
	virtual bool Read(uint32_t addr, void * buf, int len, int bufpos = 0);
public:
	AT24CXX(EW24XXType devtype, uint8_t devaddr = 0xA0, uint32_t wnms = 5); //д��ʱʱ��
	void SetPin(Pin pinscl, Pin pinsda, Pin pinwriteprotect = P0);
	uint8_t CheckOk();

	mcuGpio pinWP; //��������
private:
	uint8_t checkDevice();
	uint16_t jsPageSize(uint32_t type); //����洢ҳ��С
private:
	int PageRead(uint16_t addr, void * buf, int len);//ҳ�ڶ������һҳ
	int PageWrite(uint16_t addr, void * buf, int len);//ҳ��д�����һҳ
	
	EW24XXType deviceType; //��������
	uint32_t Block; //�洢ҳ��С
	uint32_t writedelaynms; //д��ʱ	  
};

#endif
