#ifndef _AD7689_H
#define _AD7689_H

#include "OnChip\Port.h"
#include "OnChip/Spi.h"

#define MAXCH	8
#define CHDELAY	2	//ͨ���л���ʱʱ�䣬���������ȶ�

class CAD7689
{
public:
	uint8_t FlagEnable[MAXCH]; //�Ƿ�����ͨ��ת�� 0����ֹ��1������ ����
public:
	CAD7689();
	void SetSpi(SpiBase *spi);
	void Routin(void); //��ȡADֵ,���л�ͨ��
	void Init(void); //
	float getVolt(uint16_t ch); //��ȡͨ����ѹֵ    
private:
	SpiBase *pspi; //SPI�ӿ�  

	float volt[MAXCH]; //��ѹֵ
	uint16_t ADBuf[MAXCH]; //ԭʼADֵ
	uint8_t chCurrent; //��ǰͨ��
	uint8_t chDelayCnt; //ͨ����ʱʱ�䣬���������ȶ�        
private:
	uint16_t AD_Read(void);
	uint16_t AD_Write(uint16_t sdat);
	uint8_t getNextCH(); //��ȡ�´�ת��ͨ��		
};
#endif
