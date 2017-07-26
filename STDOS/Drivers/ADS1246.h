#ifndef _ADS1246_H
#define _ADS1246_H
    #include "Spi.h"
	
	#define ADS1246SPISOFT   1

	class ADS1246
    {
        public:
			#if ADS1246SPISOFT
				ADS1246(SpiSoft *spi,InputPort &pinrd, Pin pinreset);
			#else
				ADS1246(Spi *spi,InputPort &pinrd, Pin pinreset);
			#endif
            byte ReadReg(byte RegAddr);
            void WriteReg(byte RegAddr, byte da);
            int Read(void); //����-1,��ʾת��δ���
            void Init(void);
            byte Check(void); //AD��飬��������0
            bool GetFlag(void); //��ȡADת���Ƿ�����
			void Reset(void);//��λ
			int  CheckSpeed(int checkTimems=1000);//����ٶ�,����������
        private:
            int flagOK; //adת������
            int decodead(byte *da);
			int readCnt;//��ȡ����
			int readCntCheck;//�ϴμ��Ķ�ȡ����

			#if ADS1246SPISOFT
				SpiSoft *pspi; //SPI�ӿ�
			#else
				Spi *pspi; //SPI�ӿ�
			#endif
            InputPort *ppinrd; //����׼���ýӿ�
            OutputPort ppinreset; //��λ�ӿ�		
    };
#endif
