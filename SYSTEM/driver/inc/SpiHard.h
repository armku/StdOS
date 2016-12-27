#pragma once

#include "Type.h"

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
		void SetSpeed(byte SpeedSet); //����SPI�ٶ�   
		byte ReadByte();//SPI���߶�һ���ֽ�
		byte WriteByte(byte TxData);//SPI����дһ���ֽ�
		ushort SendHalfWord(ushort HalfWord);
	private:
		ESpiChannel spiChannel;//ͨ��
};
