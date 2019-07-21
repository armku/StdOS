#ifndef _ADS1246_H
#define _ADS1246_H
    #include "Spi.h"
	
	#define ADS1246SPISOFT   1

	typedef enum
	{
		ADC_SPS_5		=	0,
		ADC_SPS_10		=	1,
		ADC_SPS_20		=	2,
		ADC_SPS_40		=	3,
		ADC_SPS_80		=	4,
		ADC_SPS_160		=	5,
		ADC_SPS_320		=	6,
		ADC_SPS_640		=	7,
		ADC_SPS_1000	=	8,
		ADC_SPS_2000	=	9,
	}ADSPEEDTYPE;
	
	class ADS1246
    {
        public:
			#if ADS1246SPISOFT
				ADS1246(SpiSoft *spi,Port &pinrd, Pin pinreset,ADSPEEDTYPE spd);
			#else
				ADS1246(Spi *spi,InputPort &pinrd, Pin pinreset,ADSPEEDTYPE spd);
			#endif
            byte ReadReg(byte RegAddr);
            void WriteReg(byte RegAddr, byte da);
            int Read(void); //����-1,��ʾת��δ���
            void Init(void);
            byte Check(void); //AD��飬��������0
            bool GetFlag(void); //��ȡADת���Ƿ�����
			void Reset(void);//��λ
			void SetSpeed(ADSPEEDTYPE spd);//����ADת���ٶ�
			int  CheckSpeed(int checkTimems=1000);//����ٶ�,����������
			bool Online();//����
        private:
            int flagOK; //adת������
            int decodead(byte *da);
			int readCnt;//��ȡ����
			int readCntCheck;//�ϴμ��Ķ�ȡ����
			bool online;//����
			ADSPEEDTYPE speed;//ADת���ٶ�

			#if ADS1246SPISOFT
				SpiSoft *pspi; //SPI�ӿ�
			#else
				Spi *pspi; //SPI�ӿ�
			#endif
            Port *ppinrd; //����׼���ýӿ�
            Port ppinreset; //��λ�ӿ�		
    };
#endif
