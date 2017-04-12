#pragma once

#include "Sys.h"
#include "Port.h"
#include "stm32f10x.h"
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

        void Close(); //��Ҫ����

    public:
        SPI_TypeDef *SPI;
        int Speed; // �ٶ�
        int Retry; // �ȴ����Դ�����Ĭ��200
        int Error; // �������

        // ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
        Spi(int spiIndex, uint speedHz = 9000000, bool useNss = true);
        ~Spi();

        byte Write(byte data);
        ushort Write16(ushort data);

        void Start(); // ����NSS����ʼ����
        void Stop(); // ����NSS��ֹͣ����
};
class CSoftSpi
{
public:
    CSoftSpi(Pin pincs, Pin pinsck, Pin pindi, Pin pindo, uint nus = 0); //��ʱʱ��Ĭ��0us   
    byte Init();
    byte WaitBusy();
    byte Write(byte da);
    byte spi_readbyte(void);
public:
    OutputPort *pClk;
    OutputPort *pportdi;
    InputPort *pportdo;
    OutputPort *pportcs;
private:
    uint delayus;//��ʱʱ��
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
