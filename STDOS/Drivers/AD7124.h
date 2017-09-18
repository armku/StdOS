#ifndef __AD7124_8_H
    #define __AD7124_8_H

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

            ushort spi16_run(ushort sendData);
            uint spi24_run(uint sendData);
            uint spi32_run(uint sendData);
            uint AD7124_Read_Reg(byte reg, byte bytes);
            uint AD7124_Read_Reg_NoCS(byte reg, byte bytes);
            void AD7124_Write_Reg(byte reg, byte bytes, uint data);
            void AD7124_Config(void);
            void AD7124_Init(void);
            void AD7124_Channel_Config(void);
            byte AD7124_Read_ID(void);
            byte AD7124_Read_Status(void);
            float AD7124_Get_Temp1(void);
            float AD7124_Get_Temp1_Res(void);
            void AD7124_Reset(void);
            float AD7124_Temp1_Test(void);
            
            uint ad7124Buf[AD7124_BUFSIZE];
            byte ad7124BufCnt = 0;
            byte ad7124Int = 0;

            int Read(void); //返回-1,表示转换未完成
            bool GetFlag(void); //读取AD转换是否正常
        private:
            int flagOK; //ad转换正常
            int readCnt; //读取次数
            int readCntCheck; //上次检查的读取周期

            #if AD7124SPISOFT
                SpiSoft *pspi; //SPI接口
            #else 
                Spi *pspi; //SPI接口
            #endif 
    };    
#endif
