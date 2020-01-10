#ifndef __SPI_H__
#define __SPI_H__

#include "Device\Port.h"

// Spi��
class Spi
{
private:
    byte	_index;
    void*	_SPI;
	Pin		Pins[4];	// NSS/CLK/MISO/MOSI
    OutputPort	_nss;

    AlternatePort _clk;
    AlternatePort _miso;
    AlternatePort _mosi;

	void Init();

public:
    int		Speed;  // �ٶ�
    int		Retry;  // �ȴ����Դ�����Ĭ��200
    int		Error;  // �������
	bool	Opened;

	Spi();
	// ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
    Spi(SPI spi, uint speedHz = 9000000, bool useNss = true);
    ~Spi();

	void Init(SPI spi, uint speedHz = 9000000, bool useNss = true);

	void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0, Pin nss = P0);
	void GetPin(Pin* clk = nullptr, Pin* miso = nullptr, Pin* mosi = nullptr, Pin* nss = nullptr);
	void Open();
	void Close();

	// ������д
    byte Write(byte data);
    ushort Write16(ushort data);

	// ������д�����ֽ����鳤��Ϊ׼
	void Write(const Buffer& bs);
	void Read(Buffer& bs);

    void Start();   // ����NSS����ʼ����
    void Stop();    // ����NSS��ֹͣ����
	
private:
	static int GetPre(int index, uint& speedHz);
	void OnInit();
	void OnOpen();
	void OnClose();
};

// Spi�Ự�ࡣ��ʼ��ʱ��Spi����������������ʱ�ر�
class SpiScope
{
private:
	Spi* _spi;

public:
	SpiScope(Spi* spi)
	{
		_spi = spi;
		_spi->Start();
	}

	~SpiScope()
	{
		_spi->Stop();
	}
};

// Spi�豸����
class SpiConfig
{
public:
	SPI Spi;

	Pin	Irq;
	Pin Reset;
};

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
class SpiBase
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
// Spi��
class Spi11:public SpiBase
{
	public:
		CPOLTYPE CPOL; //ʱ�Ӽ���
		CPHATYPE CPHA; //ʱ����λ
	private:
		byte _index;
		void *_SPI;
		Port _nss;

		Port _clk;
		Port _miso;
		Port _mosi;			
	public:
		int Retry; // �ȴ����Դ�����Ĭ��200
		int Error; // �������
		bool Opened;

		Spi11(SPI spi);
		// ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�

		virtual void Init(CPOLTYPE cpol = CPOL_High, CPHATYPE cpha = CPHA_2Edge);

		virtual void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0);
		virtual void SetNss(Pin nss = P0);

		// ������д
		virtual byte Write(byte data);
		virtual ushort Write16(ushort data);

		virtual void Start(); // ����NSS����ʼ����
		virtual void Stop(); // ����NSS��ֹͣ����
};
	
class SpiSoft_ARMKU :public SpiBase
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
