#ifndef _PCF8563_H
#define _PCF8563_H
#include "I2C.h"
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
	SoftI2C IIC; // I2C通信口
	
private:	
	byte Address; // 设备地址

};


#endif // _PCF8563_H
