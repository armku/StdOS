#ifndef _RX8025T_H
#define _RX8025T_H

#include "Kernel\Sys.h"
#include "Device\I2C.h"

class RX8025T
{
	public:
		SoftI2C IIC; // I2Cͨ�ſ�
        byte Address; // �豸��ַ
	public:
		RX8025T();
		RX8025T(Pin pinsck, Pin pinsda, byte devaddr = 0xA0, uint wnus = 0);
		void Init();
		byte CheckDevice(void);//
		byte Output1s_Start(void);
		byte Output1s_Stop(void);
		int LoadTime(DateTime &dt);
		int SaveTime(DateTime &dt);		
	private:
		int RegWrite(byte sadd, byte  len);
		int RegRead(byte sadd, byte len);
	private:
		byte buf[20];
		byte devaddr;//�豸��ַ
};

#endif
