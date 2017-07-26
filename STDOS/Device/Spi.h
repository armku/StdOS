#ifndef __SPI_H__
    #define __SPI_H__

    #include "Device\Port.h"

    // Spi��
    class Spi
    {
        private:
            byte _index;
            void *_SPI;
            Pin Pins[4]; // NSS/CLK/MISO/MOSI
            OutputPort _nss;

            AlternatePort _clk;
            AlternatePort _miso;
            AlternatePort _mosi;

            void Init();

        public:
            int Speed; // �ٶ�
            int Retry; // �ȴ����Դ�����Ĭ��200
            int Error; // �������
            bool Opened;

            Spi();
            // ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�
            Spi(SPI spi, uint speedHz = 9000000, bool useNss = true);
            ~Spi();

            void Init(SPI spi, uint speedHz = 9000000, bool useNss = true);

            void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0, Pin nss = P0);
			void SetNss(Pin nss = P0);
            void GetPin(Pin *clk = NULL, Pin *miso = NULL, Pin *mosi = NULL, Pin *nss = NULL);
            void Open();
            void Close();

            // ������д
            byte Write(byte data);
            ushort Write16(ushort data);

            // ������д�����ֽ����鳤��Ϊ׼
            void Write(const Buffer &bs);
            void Read(Buffer &bs);

            void Start(); // ����NSS����ʼ����
            void Stop(); // ����NSS��ֹͣ����

        private:
            static int GetPre(int index, uint &speedHz);
            void OnInit();
            void OnOpen();
            void OnClose();
    };

    // Spi�Ự�ࡣ��ʼ��ʱ��Spi����������������ʱ�ر�
    class SpiScope
    {
        private:
            Spi *_spi;

        public:
            SpiScope(Spi *spi)
            {
                    _spi = spi;
                    _spi->Start();
            }

            ~SpiScope()
            {
                    _spi->Stop();
            }
    };

    class SpiSoft
    {
        public:
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
			CPOLTYPE CPOL;//ʱ�Ӽ���
			CPHATYPE CPHA;//ʱ����λ
        public:
            SpiSoft(uint speedHz = 9000000); //ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�   
            void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0, Pin nss = P0);
			void SetNss(Pin nss = P0);
            byte Write(byte data);
            void Open();
            void Close();
            void Start(); // ����NSS����ʼ����
            void Stop(); // ����NSS��ֹͣ����
        private:
            OutputPort _nss;
            OutputPort _clk;
            OutputPort _mosi;
            InputPort _miso;
            uint delayus; //��ʱʱ��
        private:
            byte WaitBusy();

    };
#endif
