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
        OutputPort *pNss;

        AlternatePort *pClk;
        AlternatePort *pMiso;
        AlternatePort *pMosi;

        void Close(); //需要清理

    public:
        SPI_TypeDef *SPI;
        int Speed; // 速度
        int Retry; // 等待重试次数，默认200
        int Error; // 错误次数

        // 使用端口和最大速度初始化Spi，因为需要分频，实际速度小于等于该速度
        Spi(int spiIndex, uint speedHz = 9000000, bool useNss = true);
        ~Spi();

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
class CSoftSpi
{
public:
    CSoftSpi(Pin pincs, Pin pinsck, Pin pindi, Pin pindo, uint nus = 0); //延时时间默认0us   
    byte Init();
    byte WaitBusy();
    byte spi_writebyte(byte da);
    byte spi_readbyte(void);
public:
    OutputPort *pportsck;
    OutputPort *pportdi;
    InputPort *pportdo;
    OutputPort *pportcs;
private:
    uint delayus;//延时时间
};
typedef enum
{
CHSPI1	=	1,
CHSPI2	=	2,
CHSPI3	=	3
}ESpiChannel;
//硬件SPI
class CHardSpi
{
	public:
		CHardSpi(ESpiChannel spichannel);		
	public:
		void Init(void);			 //初始化SPI口
		void SetSpeed(byte SpeedSet); //设置SPI速度   
		byte ReadByte();//SPI总线读一个字节
		byte WriteByte(byte TxData);//SPI总线写一个字节
		ushort SendHalfWord(ushort HalfWord);
	private:
		ESpiChannel spiChannel;//通道
};
