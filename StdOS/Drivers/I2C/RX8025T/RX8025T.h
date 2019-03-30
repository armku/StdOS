#ifndef _RX8025T_H
#define _RX8025T_H

#include "Kernel\Sys.h"
#include "../OnChip/I2C.h"
#include "Core\DateTime.h"

class RX8025T
{
	public:
		SoftI2C IIC; // I2C通信口
        uint8_t Address; // 设备地址
	public:
		RX8025T();
		RX8025T(Pin pinsck, Pin pinsda, uint8_t devaddr = 0xA0, uint32_t wnus = 0);
		void Init();
		uint8_t CheckDevice(void);//
		uint8_t Output1s_Start(void);
		uint8_t Output1s_Stop(void);
		int LoadTime(DateTime &dt);
		int SaveTime(DateTime &dt);		
	private:
		int RegWrite(uint8_t sadd, uint8_t  len);
		int RegRead(uint8_t sadd, uint8_t len);
	private:
		uint8_t buf[20];
		uint8_t devaddr;//设备地址
};

#endif
