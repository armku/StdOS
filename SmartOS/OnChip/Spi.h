#ifndef __SPI_H__
#define __SPI_H__

#include "Device\Port.h"

typedef enum
{
	CPOL_Low = 0,  // ʱ�Ӽ��ԣ�����ʱΪ��
	CPOL_High = 1,  // ʱ�Ӽ��ԣ�����ʱΪ��
} CPOLTYPE;
typedef enum
{
	CPHA_1Edge = 0,  //ʱ����λ �ڴ���ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
	CPHA_2Edge = 1,  //ʱ����λ �ڴ���ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
} CPHATYPE;
class SpiBase_ARMKU
{
public:
	virtual void Init(CPOLTYPE cpol = CPOL_High, CPHATYPE cpha = CPHA_2Edge)=0;

	virtual void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0)=0;
	virtual void SetNss(Pin nss = P0)=0;

	// ������д
	virtual byte Write(byte data)=0;
	byte Read() { return this->Write(0XFF); }
	virtual ushort Write16(ushort data) = 0;

	virtual void Start()=0; // ����NSS����ʼ����
	virtual void Stop()=0; // ����NSS��ֹͣ����
};
class SpiSoft_ARMKU :public SpiBase_ARMKU
{
	public:
		CPOLTYPE CPOL; //ʱ�Ӽ���
		CPHATYPE CPHA; //ʱ����λ
	public:
		SpiSoft_ARMKU(); //ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�   
		virtual void Init(CPOLTYPE cpol = CPOL_High, CPHATYPE cpha = CPHA_2Edge);
		virtual void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0);
		virtual void SetNss(Pin nss = P0);
		virtual byte Write(byte data);
		virtual ushort Write16(ushort data);
		void Open();
		void Close();
		virtual void Start(); // ����NSS����ʼ����
		virtual void Stop(); // ����NSS��ֹͣ����
	private:
		Port _nss;
		Port _clk;
		Port _mosi;
		Port _miso;
		uint delayus; //��ʱʱ��
	private:
		byte WaitBusy();
};
#endif
