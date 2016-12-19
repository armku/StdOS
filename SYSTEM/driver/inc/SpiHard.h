#pragma once

#include <stdint.h>

typedef enum
{
CHSPI1	=	1,
CHSPI2	=	2,
CHSPI3	=	3
}ESpiChannel;
//Ӳ��SPI
class CSpiHard
{
	public:
		CSpiHard(ESpiChannel spichannel);
		~CSpiHard();
	public:
		void Init(void);			 //��ʼ��SPI��
		void SetSpeed(uint8_t SpeedSet); //����SPI�ٶ�   
		uint8_t ReadByte();//SPI���߶�һ���ֽ�
		uint8_t WriteByte(uint8_t TxData);//SPI����дһ���ֽ�
		uint16_t SendHalfWord(uint16_t HalfWord);
	private:
		ESpiChannel spiChannel;//ͨ��
};
