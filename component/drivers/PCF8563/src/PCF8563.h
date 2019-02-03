#ifndef _PCF8563_H
#define _PCF8563_H
#include "../../I2C.h"
#include "../../../../component/lib/DateTime.h"
class PCF8563
{
public:
	PCF8563();
	~PCF8563();
	void SetDateTime(DateTime & dt);
	void GetDateTime(DateTime& dt);
	uint8_t readaddr(uint8_t add);
public:
	SoftI2C IIC; // I2Cͨ�ſ�
	
private:	
	uint8_t Address; // �豸��ַ

};


#endif // _PCF8563_H
