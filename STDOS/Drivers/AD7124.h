#ifndef __AD7124_H
    #define __AD7124_H

    #include "spi.h"

    #define AD7124SPISOFT 1	
	#define AD7124_BUFSIZE 6

    class AD7124
    {
        public:
            #if AD7124SPISOFT
                AD7124(SpiSoft *spi);
            #else 
                AD7124(Spi *spi);
            #endif 
			ushort CHCONFIG[16];//通道配置

            void Init_8();		//8通道初始化
			void Init_4();		//4通道初始化
            byte ReadID();		//读取ID
            byte ReadStatus();	//读取状态
            void Reset();		//复位
            uint ReadRlt();		//读取AD值
			uint ReadRlt(byte& status);		//读取AD值
			byte Status;		//器件状态
                        
            int Read(void); //返回-1,表示转换未完成
            bool GetFlag(void); //读取AD转换是否正常
			void WriteReg(byte reg, byte bytes, uint data);
			void SetReadChannel(byte ch=0,byte chMax=7);//设置需要读取的通道，默认通道0
        private:
			ushort Write16(ushort sendData);
            uint Write24(uint sendData);
            uint Write32(uint sendData);
            uint ReadRegNoCS(byte reg, byte bytes);
            uint ReadReg(byte reg, byte bytes);			
            
		private:
            int flagOK; //ad转换正常
            int readCnt; //读取次数
            int readCntCheck; //上次检查的读取周期

            #if AD7124SPISOFT
                SpiSoft *pspi; //SPI接口
            #else 
                Spi *pspi; //SPI接口
            #endif 
		private:		
		//测试
			float GetTemp1();
            float GetTemp1Res();
            float Temp1Test();
            uint ad7124Buf[AD7124_BUFSIZE];
            byte ad7124BufCnt = 0;
            byte ad7124Int = 0;

    };    
#endif
