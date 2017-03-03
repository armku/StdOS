#pragma once

#include "Sys.h"
#include "Port.h"

/*Spi����*/
//SPI1..���ָ�ʽ��st���ͻ
#define SPI_1    0
#define SPI_2    1
#define SPI_3    2
#define SPI_NONE 0XFF
#if 0
// Spi��
class Spi
{
private:
    int _spi;
    OutputPort* _nss;

    AlternatePort* clk;
    AlternatePort* msio;
    AlternatePort* mosi;

public:
    
    virtual ~Spi();
};

Spi���÷�Ҳ�ܼ򵥣�ֱ��Spi spi(SPI1, 9000000, true)���ɳ�ʼ��SPI1Ϊ9MHz��ʹ��nss
�����SpiScope��һ��Spi�������������ںܶ�Spi�����������棬��Ҫ������nss��Ȼ�������������ߡ�
����SpiScope������ֻ��Ҫ�ں�������Spiǰ��һ��
SpiScope sc(&spi);
Ȼ���ľͲ��ù��ˣ��뿪���������ʱ��sc���������Զ�����spi.Stop����nss������ɣ�
End.
#endif
// Spi��
class Spi
{
    private:
        byte _index;
        Pin Pins[4]; // NSS/CLK/MISO/MOSI
        OutputPort _nss;

        AlternatePort _clk;
        AlternatePort _miso;
        AlternatePort _mosi;

        void Init();

    public:
        SPI_TypeDef *SPI;
        int Speed; // �ٶ�
        int Retry; // �ȴ����Դ�����Ĭ��200
        int Error; // �������
        bool Opened;

        Spi();
		Spi(int spi, int speedHz = 9000000, bool useNss = true);
        // ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
        Spi(SPI_TypeDef *spi, uint speedHz = 9000000, bool useNss = true);
        ~Spi();

        void Init(SPI_TypeDef *spi, uint speedHz = 9000000, bool useNss = true);

        void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0, Pin nss = P0);
        void GetPin(Pin *clk = NULL, Pin *miso = NULL, Pin *mosi = NULL, Pin *nss = NULL);
        void Open();
        void Close();

        byte Write(byte data);
        ushort Write16(ushort data);

        void Start(); // ����NSS����ʼ����
        void Stop(); // ����NSS��ֹͣ����
};

// Spi�Ự�ࡣ��ʼ��ʱ��Spi����������������ʱ�ر�
class SpiScope
{
    private:
        Spi *_spi;

    public:
        _force_inline SpiScope(Spi *spi)
        {
                _spi = spi;
                _spi->Start();
        }

        _force_inline ~SpiScope()
        {
                _spi->Stop();
        }
};
