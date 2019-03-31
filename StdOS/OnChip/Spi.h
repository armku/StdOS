#ifndef __SPI_H__
    #define __SPI_H__

    #include "OnChip\Port.h"

    typedef enum
    {
        CPOL_Low = 0,  // 时钟极性，空闲时为低
        CPOL_High = 1,  // 时钟极性，空闲时为高
    } CPOLTYPE;
    typedef enum
    {
        CPHA_1Edge = 0,  //时钟相位 在串行同步时钟的第一个跳变沿（上升或下降）数据被采样
        CPHA_2Edge = 1,  //时钟相位 在串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    } CPHATYPE;
	class SpiBase
	{

	};
    // Spi类
    class Spi:public SpiBase
    {
        public:
            CPOLTYPE CPOL; //时钟极性
            CPHATYPE CPHA; //时钟相位
        private:
            uint8_t _index;
            void *_SPI;
            Port _nss;

            Port _clk;
            Port _miso;
            Port _mosi;			
        public:
            int Speed; // 速度
            int Retry; // 等待重试次数，默认200
            int Error; // 错误次数
            bool Opened;

            Spi();
            // 使用端口和最大速度初始化Spi，因为需要分频，实际速度小于等于该速度

            void Init(SPI spi, CPOLTYPE cpol = CPOL_High, CPHATYPE cpha = CPHA_2Edge, uint32_t speedHz = 9000000);

            void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0);
            void SetNss(Pin nss = P0);

            // 基础读写
            uint8_t Write(uint8_t data);
			uint8_t Read() { return this->Write(0XFF); }
            uint16_t Write16(uint16_t data);

            void Start(); // 拉低NSS，开始传输
            void Stop(); // 拉高NSS，停止传输

        private:
            static int GetPre(int index, uint32_t &speedHz);
    };
	    
    class SpiSoft
    {
        public:
            CPOLTYPE CPOL; //时钟极性
            CPHATYPE CPHA; //时钟相位
        public:
            SpiSoft(CPOLTYPE cpol = CPOL_Low, CPHATYPE cpha = CPHA_1Edge, uint32_t speedHz = 9000000); //使用端口和最大速度初始化Spi，因为需要分频，实际速度小于等于该速度   
            void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0);
            void SetNss(Pin nss = P0);
            uint8_t Write(uint8_t data);
            void Open();
            void Close();
            void Start(); // 拉低NSS，开始传输
            void Stop(); // 拉高NSS，停止传输
        private:
            Port _nss;
            Port _clk;
            Port _mosi;
            Port _miso;
            uint32_t delayus; //延时时间
        private:
            uint8_t WaitBusy();

    };
#endif
