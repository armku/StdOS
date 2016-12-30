#pragma once

#include "Type.h"

typedef enum
{
CHSPI1	=	1,
CHSPI2	=	2,
CHSPI3	=	3
}ESpiChannel;
//硬件SPI
class CHardSpi
{
	public:
		CHardSpi(ESpiChannel spichannel);		
	public:
		void Init(void);			 //初始化SPI口
		void SetSpeed(byte SpeedSet); //设置SPI速度   
		byte ReadByte();//SPI总线读一个字节
		byte WriteByte(byte TxData);//SPI总线写一个字节
		ushort SendHalfWord(ushort HalfWord);
	private:
		ESpiChannel spiChannel;//通道
};
