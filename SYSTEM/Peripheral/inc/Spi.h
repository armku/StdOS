#pragma once

#include "Sys.h"
#include "Port.h"

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
