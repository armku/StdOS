#ifndef _PCF8563_H
#define _PCF8563_H
#include "../I2CSoft_ARMKU.h"
#include "DateTime.h"
class PCF8563
{
public:
	PCF8563();
	~PCF8563();
	void SetDateTime(DateTime & dt);
	void GetDateTime(DateTime& dt);
	byte readaddr(byte add);
public:
	I2CSoft_ARMKU IIC; // I2Cͨ�ſ�
	
private:	
	byte Address; // �豸��ַ

};


#endif // _PCF8563_H
