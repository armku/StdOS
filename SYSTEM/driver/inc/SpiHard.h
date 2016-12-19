#pragma once

#include <stdint.h>

typedef enum
{
CHSPI1	=	1,
CHSPI2	=	2,
CHSPI3	=	3
}ESpiChannel;
//硬件SPI
class CSpiHard
{
	public:
		CSpiHard(ESpiChannel spichannel);
		~CSpiHard();
	public:
		void Init(void);			 //初始化SPI口
		void SetSpeed(uint8_t SpeedSet); //设置SPI速度   
		uint8_t ReadByte();//SPI总线读一个字节
		uint8_t WriteByte(uint8_t TxData);//SPI总线写一个字节
		uint16_t SendHalfWord(uint16_t HalfWord);
	private:
		ESpiChannel spiChannel;//通道
};
