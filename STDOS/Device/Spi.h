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

    class SoftSpi
    {
        public:
            SoftSpi(uint speedHz = 9000000); //ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�   
            void SetPin(Pin pincs, Pin pinsck, Pin pindi, Pin pindo);
            byte Write(byte data);
            void Open();
            void Close();
            void Start(); // ����NSS����ʼ����
            void Stop(); // ����NSS��ֹͣ����
        private:
            OutputPort pportcs;
            OutputPort pClk;
            OutputPort pportdi;
            InputPort pportdo;
            uint delayus; //��ʱʱ��
        private:
            byte WaitBusy();

    };
#endif
