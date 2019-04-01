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
	class SpiBase
	{
	public:
		virtual void Init(CPOLTYPE cpol = CPOL_High, CPHATYPE cpha = CPHA_2Edge)=0;

		virtual void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0)=0;
		virtual void SetNss(Pin nss = P0)=0;

		// ������д
		virtual uint8_t Write(uint8_t data)=0;
		uint8_t Read() { return this->Write(0XFF); }
		//virtual uint16_t Write16(uint16_t data)=0;

		virtual void Start()=0; // ����NSS����ʼ����
		virtual void Stop()=0; // ����NSS��ֹͣ����
	};
    // Spi��
    class Spi:public SpiBase
    {
        public:
            CPOLTYPE CPOL; //ʱ�Ӽ���
            CPHATYPE CPHA; //ʱ����λ
        private:
            uint8_t _index;
            void *_SPI;
            Port _nss;

            Port _clk;
            Port _miso;
            Port _mosi;			
        public:
            int Retry; // �ȴ����Դ�����Ĭ��200
            int Error; // �������
            bool Opened;

            Spi(SPI spi);
            // ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�

			virtual void Init(CPOLTYPE cpol = CPOL_High, CPHATYPE cpha = CPHA_2Edge);

			virtual void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0);
			virtual void SetNss(Pin nss = P0);

            // ������д
			virtual uint8_t Write(uint8_t data);
			virtual uint16_t Write16(uint16_t data);

			virtual void Start(); // ����NSS����ʼ����
			virtual void Stop(); // ����NSS��ֹͣ����
    };
	    
    class SpiSoft :public SpiBase
    {
        public:
            CPOLTYPE CPOL; //ʱ�Ӽ���
            CPHATYPE CPHA; //ʱ����λ
        public:
            SpiSoft(); //ʹ�ö˿ں�����ٶȳ�ʼ��Spi����Ϊ��Ҫ��Ƶ��ʵ���ٶ�С�ڵ��ڸ��ٶ�   
			virtual void Init(CPOLTYPE cpol = CPOL_High, CPHATYPE cpha = CPHA_2Edge);
			virtual void SetPin(Pin clk = P0, Pin miso = P0, Pin mosi = P0);
			virtual void SetNss(Pin nss = P0);
			virtual uint8_t Write(uint8_t data);
			//virtual uint16_t Write16(uint16_t data);
            void Open();
            void Close();
			virtual void Start(); // ����NSS����ʼ����
			virtual void Stop(); // ����NSS��ֹͣ����
        private:
            Port _nss;
            Port _clk;
            Port _mosi;
            Port _miso;
            uint32_t delayus; //��ʱʱ��
        private:
            uint8_t WaitBusy();
    };
#endif
