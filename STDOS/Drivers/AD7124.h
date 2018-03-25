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
			ushort CHCONFIG[16];//ͨ������

            void Init_8();		//8ͨ����ʼ��
			void Init_4();		//4ͨ����ʼ��
            byte ReadID();		//��ȡID
            byte ReadStatus();	//��ȡ״̬
            void Reset();		//��λ
            uint32_t ReadRlt();		//��ȡADֵ
			uint32_t ReadRlt(byte& status);		//��ȡADֵ
			byte Status;		//����״̬
                        
            int Read(void); //����-1,��ʾת��δ���
            bool GetFlag(void); //��ȡADת���Ƿ�����
			void WriteReg(byte reg, byte bytes, uint32_t data);
			void SetReadChannel(byte ch=0,byte chMax=7);//������Ҫ��ȡ��ͨ����Ĭ��ͨ��0
        private:
			ushort Write16(ushort sendData);
            uint32_t Write24(uint32_t sendData);
            uint32_t Write32(uint32_t sendData);
            uint32_t ReadRegNoCS(byte reg, byte bytes);
            uint32_t ReadReg(byte reg, byte bytes);			
            
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
            uint32_t ad7124Buf[AD7124_BUFSIZE];
            byte ad7124BufCnt = 0;
            byte ad7124Int = 0;

    };    
#endif
