#ifndef _ADS1246_H
#define _ADS1246_H
    #include "../SpiSoft_ARMKU.h"
	
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
				ADS1246(SpiSoft_ARMKU *spi,Port &pinrd, Pin pinreset,ADSPEEDTYPE spd);
			#else
				ADS1246(Spi *spi,InputPort &pinrd, Pin pinreset,ADSPEEDTYPE spd);
			#endif
            byte ReadReg(byte RegAddr);
            void WriteReg(byte RegAddr, byte da);
            int Read(void); //返回-1,表示转换未完成
            void Init(void);
            byte Check(void); //AD检查，正常返回0
            bool GetFlag(void); //读取AD转换是否正常
			void Reset(void);//复位
			void SetSpeed(ADSPEEDTYPE spd);//设置AD转换速度
			int  CheckSpeed(int checkTimems=1000);//检查速度,传入检查周期
			bool Online();//在线
        private:
            int flagOK; //ad转换正常
            int decodead(byte *da);
			int readCnt;//读取次数
			int readCntCheck;//上次检查的读取周期
			bool online;//在线
			ADSPEEDTYPE speed;//AD转换速度

			#if ADS1246SPISOFT
				SpiSoft_ARMKU *pspi; //SPI接口
			#else
				Spi *pspi; //SPI接口
			#endif
            Port *ppinrd; //数据准备好接口
            Port ppinreset; //复位接口		
    };
#endif
