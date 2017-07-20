#ifndef _RX8025T_H
#define _RX8025T_H

#include "Kernel\Sys.h"
#include "Device\I2C.h"

class RX8025T
{
	public:
		SoftI2C IIC; // I2C通信口
        byte Address; // 设备地址
	public:
		RX8025T();
		RX8025T(Pin pinsck, Pin pinsda, byte devaddr = 0xA0, uint wnms = 4);
		void Init();
		void LoadTime(DateTime &dt);
		void SaveTime(DateTime &dt);
		void RX8025_Write(byte sadd,byte  *buf, byte  len);
		void RX8025_Read(byte sadd, byte *buf, byte len);
	private:
		byte buf[20];
		byte devaddr;//设备地址
		
};

#endif
