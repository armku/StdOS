#ifndef _ADS1246_H
#define _ADS1246_H
    #include "Spi.h"

	class ADS1246
    {
        public:
            ADS1246(Pin clk, Pin miso, Pin mosi,Pin nss,  InputPort &pinrd, Pin pinreset);
            byte ReadReg(byte RegAddr);
            void WriteReg(byte RegAddr, byte da);
            int Read(void); //返回-1,表示转换未完成
            void Init(void);
            byte Check(void); //AD检查，正常返回0
            bool GetFlag(void); //读取AD转换是否正常
			void Reset(void);//复位
			int  CheckSpeed(int checkTimems=1000);//检查速度,传入检查周期
        private:
            int flagOK; //ad转换正常
            int decodead(byte *da);
			int readCnt;//读取次数
			int readCntCheck;//上次检查的读取周期

            SpiSoft pspi; //SPI接口
            InputPort *ppinrd; //数据准备好接口
            OutputPort ppinreset; //复位接口		
    };
#endif
