#pragma once

#include "Sys.h"
#include "Port.h"

/*Spi定义*/
//SPI1..这种格式与st库冲突
#define SPI_1    0
#define SPI_2    1
#define SPI_3    2
#define SPI_NONE 0XFF
#if 0
    // Spi类
    class Spi
    {
        private:
            int _spi;
            OutputPort *_nss;

            AlternatePort *clk;
            AlternatePort *msio;
            AlternatePort *mosi;

        public:

            virtual ~Spi();
    };

    Spi ???????????????????覵pi spi(SPI1, 9000000, true)?????????疭PI1 ??MHz ?????胣ss ?????腟piScope ?????鯯pi ???????????
        ??????郤pi ???????????????????????蚽ss ?????????????????????????鶶piScope ?????????????????????
        鱏pi ????????SpiScope sc(&spi);
    ?????????????????????????????????????騭c ???????????????胹pi.Stop ???遪ss ??????????End. 
#endif 
// Spi类
class Spi
{
    private:
        byte _index;
        Pin Pins[4]; // NSS/CLK/MISO/MOSI
        OutputPort *pNss;

        AlternatePort *pClk;
        AlternatePort *pMiso;
        AlternatePort *pMosi;

        byte useNss; //使用nss

        void Init();

    public:
        SPI_TypeDef *SPI;
        int Speed; // 速度
        int Retry; // 等待重试次数，默认200
        int Error; // 错误次数
        bool Opened;

        Spi();
        Spi(int spi, int speedHz = 9000000, bool useNss = true);
        // 使用端口和最大速度初始化Spi，因为需要分频，实际速度小于等于该速度
        Spi(SPI_TypeDef *spi, uint speedHz = 9000000, bool useNss = true);
        ~Spi();

        void Init(SPI_TypeDef *spi, uint speedHz = 9000000, bool useNss = true);

        void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0, Pin nss = P0);
        void GetPin(Pin *clk = NULL, Pin *miso = NULL, Pin *mosi = NULL, Pin *nss = NULL);
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
