#pragma once

#include "Sys.h"
#include "Port.h"

/*Spi����*/
//SPI1..���ָ�ʽ��st���ͻ
#define SPI_1    0
#define SPI_2    1
#define SPI_3    2
#define SPI_NONE 0XFF

// Spi��
class Spi
{
    private:
        byte _index;
        OutputPort *pNss;

        AlternatePort *pClk;
        AlternatePort *pMiso;
        AlternatePort *pMosi;

		void Init(SPI_TypeDef *spi, uint speedHz = 9000000, bool useNss = true);
		void Open(bool useNss);//��Ҫ����
        void Close();//��Ҫ����

    public:
        SPI_TypeDef *SPI;
        int Speed; // �ٶ�
        int Retry; // �ȴ����Դ�����Ĭ��200
        int Error; // �������
        
        // ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
        Spi(int spi, int speedHz = 9000000, bool useNss = true);
        ~Spi();
        
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
