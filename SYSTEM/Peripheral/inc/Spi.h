#pragma once

#include "Sys.h"
#include "Port.h"

/*Spi定义*/
//SPI1..这种格式与st库冲突
#define SPI_1    0
#define SPI_2    1
#define SPI_3    2
#define SPI_NONE 0XFF

// Spi类
class Spi
{
    private:
        byte _index;
        Pin Pins[4]; // NSS/CLK/MISO/MOSI 需要消除
        OutputPort *pNss;

        AlternatePort *pClk;
        AlternatePort *pMiso;
        AlternatePort *pMosi;

        void Init();
    public:
        SPI_TypeDef *SPI;
        int Speed; // 速度
        int Retry; // 等待重试次数，默认200
        int Error; // 错误次数
        bool Opened;

        // 使用端口和最大速度初始化Spi，因为需要分频，实际速度小于等于该速度
        Spi(int spi, int speedHz = 9000000, bool useNss = true);
        ~Spi();

        void Init(SPI_TypeDef *spi, uint speedHz = 9000000, bool useNss = true);

        void Open();
        void Close();

        byte Write(byte data);
        ushort Write16(ushort data);

        void Start(); // 拉低NSS，开始传输
        void Stop(); // 拉高NSS，停止传输
};

// Spi会话类。初始化时打开Spi，超出作用域析构时关闭
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
