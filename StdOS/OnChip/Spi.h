#ifndef __SPI_H__
    #define __SPI_H__

    #include "OnChip\Port.h"

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
    // Spi��
    class Spi
    {
        public:
            CPOLTYPE CPOL; //ʱ�Ӽ���
            CPHATYPE CPHA; //ʱ����λ
        private:
            uint8_t _index;
            void *_SPI;
            //Pin Pins[4]; // NSS/CLK/MISO/MOSI
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
            Spi(SPI spi, CPOLTYPE cpol = CPOL_High, CPHATYPE cpha = CPHA_2Edge, uint32_t speedHz = 9000000);
            ~Spi();

            void Init(SPI spi, uint32_t speedHz = 9000000);

            void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0);
            void SetNss(Pin nss = P0);
            void Open();
            void Close();

            // ������д
            uint8_t Write(uint8_t data);
            uint16_t Write16(uint16_t data);

            // ������д�����ֽ����鳤��Ϊ׼
            void Write(void * buf, int len);
            void Read(void * buf, int len);

            void Start(); // ����NSS����ʼ����
            void Stop(); // ����NSS��ֹͣ����

        private:
            static int GetPre(int index, uint32_t &speedHz);
            void OnInit();
            void OnOpen();
            void OnClose();
    };
	    
    class SpiSoft
    {
        public:
            CPOLTYPE CPOL; //ʱ�Ӽ���
            CPHATYPE CPHA; //ʱ����λ
        public:
            SpiSoft(CPOLTYPE cpol = CPOL_Low, CPHATYPE cpha = CPHA_1Edge, uint32_t speedHz = 9000000); //ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�   
            void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0);
            void SetNss(Pin nss = P0);
            uint8_t Write(uint8_t data);
            void Open();
            void Close();
            void Start(); // ����NSS����ʼ����
            void Stop(); // ����NSS��ֹͣ����
        private:
            OutputPort _nss;
            OutputPort _clk;
            OutputPort _mosi;
            Port _miso;
            uint32_t delayus; //��ʱʱ��
        private:
            uint8_t WaitBusy();

    };
#endif
