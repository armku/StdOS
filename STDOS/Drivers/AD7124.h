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
			ushort CHCONFIG[16];//ͨ������

            void Init();		
            byte ReadID();		//��ȡID
            byte ReadStatus();	//��ȡ״̬
            void Reset();		//��λ
            uint ReadRlt();		//��ȡADֵ
			uint ReadRlt(byte& status);		//��ȡADֵ
			byte Status;		//����״̬
                        
            int Read(void); //����-1,��ʾת��δ���
            bool GetFlag(void); //��ȡADת���Ƿ�����
			void WriteReg(byte reg, byte bytes, uint data);
        private:
			ushort Write16(ushort sendData);
            uint Write24(uint sendData);
            uint Write32(uint sendData);
            uint ReadRegNoCS(byte reg, byte bytes);
            uint ReadReg(byte reg, byte bytes);			
            
		private:
            int flagOK; //adת������
            int readCnt; //��ȡ����
            int readCntCheck; //�ϴμ��Ķ�ȡ����

            #if AD7124SPISOFT
                SpiSoft *pspi; //SPI�ӿ�
            #else 
                Spi *pspi; //SPI�ӿ�
            #endif 
		private:		
		//����
			float GetTemp1();
            float GetTemp1Res();
            float Temp1Test();
            uint ad7124Buf[AD7124_BUFSIZE];
            byte ad7124BufCnt = 0;
            byte ad7124Int = 0;

    };    
#endif
