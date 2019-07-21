#ifndef __AD7124_H
    #define __AD7124_H

    #include "spi.h"

    #define AD7124_BUFSIZE 6

    class AD7124
    {
        public:
            uint16_t CHCONFIG[16];//ͨ������

			void SetSpi(SpiBase *spi); 
			void Init_8();		//8ͨ����ʼ��
			void Init_4();		//4ͨ����ʼ��
            uint8_t ReadID();		//��ȡID
            uint8_t ReadStatus();	//��ȡ״̬
            void Reset();		//��λ
            uint ReadRlt();		//��ȡADֵ
			uint ReadRlt(uint8_t& status);		//��ȡADֵ
			uint8_t Status;		//����״̬
                        
            int Read(void); //����-1,��ʾת��δ���
            bool GetFlag(void); //��ȡADת���Ƿ�����
			void WriteReg(uint8_t reg, uint8_t bytes, uint data);
			void SetReadChannel(uint8_t ch=0,uint8_t chMax=7);//������Ҫ��ȡ��ͨ����Ĭ��ͨ��0
        private:
			uint16_t Write16(uint16_t sendData);
            uint Write24(uint sendData);
            uint Write32(uint sendData);
            uint ReadRegNoCS(uint8_t reg, uint8_t bytes);
            uint ReadReg(uint8_t reg, uint8_t bytes);			
            
		private:
            int flagOK; //adת������
            int readCnt; //��ȡ����
            int readCntCheck; //�ϴμ��Ķ�ȡ����

            SpiBase *pspi; //SPI�ӿ�            
		private:		
		//����
			float GetTemp1();
            float GetTemp1Res();
            float Temp1Test();
            uint ad7124Buf[AD7124_BUFSIZE];
            uint8_t ad7124BufCnt;
            uint8_t ad7124Int;
    };    
#endif
