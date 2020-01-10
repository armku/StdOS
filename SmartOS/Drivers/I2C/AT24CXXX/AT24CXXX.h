#ifndef _AT24CXX_2_H_
#define _AT24CXX_2_H_

#include "..\I2CSoft_ARMKU.h"

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
class AT24CXXX
{
public:
	I2CSoft_ARMKU* IIC; // I2Cͨ�ſ�
	byte Address; // �豸��ַ

	bool Write(ushort addr, byte data);
	byte Read(ushort addr);

	virtual bool Write(uint addr, void * buf, int len, int bufpos = 0);
	virtual bool Read(uint addr, void * buf, int len, int bufpos = 0);
public:
	AT24CXXX(EW24XXType devtype, byte devaddr = 0xA0, uint wnms = 5); //д��ʱʱ��
	void SetPin(Pin pinscl, Pin pinsda, Pin pinwriteprotect = P0);
	byte CheckOk();

	OutputPort pinWP; //��������
private:
	byte checkDevice();
	ushort jsPageSize(uint type); //����洢ҳ��С
private:
	int PageRead(ushort addr, void * buf, int len);//ҳ�ڶ������һҳ
	int PageWrite(ushort addr, void * buf, int len);//ҳ��д�����һҳ
	
	EW24XXType deviceType; //��������
	uint Block; //�洢ҳ��С
	uint writedelaynms; //д��ʱ	  
};

#endif
