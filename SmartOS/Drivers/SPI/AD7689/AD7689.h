#ifndef _AD7689_H
#define _AD7689_H

#include "Device\Port.h"
#include "OnChip/Spi.h"

#define MAXCH	8
#define CHDELAY	2	//ͨ���л���ʱʱ�䣬���������ȶ�

class CAD7689
{
public:
	byte FlagEnable[MAXCH]; //�Ƿ�����ͨ��ת�� 0����ֹ��1������ ����
public:
	CAD7689();
	void SetSpi(SpiBase_ARMKU *spi);
	void Routin(void); //��ȡADֵ,���л�ͨ��
	void Init(void); //
	float getVolt(ushort ch); //��ȡͨ����ѹֵ    
private:
	SpiBase_ARMKU *pspi; //SPI�ӿ�  

	float volt[MAXCH]; //��ѹֵ
	ushort ADBuf[MAXCH]; //ԭʼADֵ
	byte chCurrent; //��ǰͨ��
	byte chDelayCnt; //ͨ����ʱʱ�䣬���������ȶ�        
private:
	ushort AD_Read(void);
	ushort AD_Write(ushort sdat);
	byte getNextCH(); //��ȡ�´�ת��ͨ��		
};
#endif
