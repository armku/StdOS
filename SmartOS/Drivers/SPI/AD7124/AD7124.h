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
            byte ReadID();		//��ȡID
            byte ReadStatus();	//��ȡ״̬
            void Reset();		//��λ
            uint ReadRlt();		//��ȡADֵ
			uint ReadRlt(byte& status);		//��ȡADֵ
			byte Status;		//����״̬
                        
            int Read(void); //����-1,��ʾת��δ���
            bool GetFlag(void); //��ȡADת���Ƿ�����
			void WriteReg(byte reg, byte bytes, uint data);
			void SetReadChannel(byte ch=0,byte chMax=7);//������Ҫ��ȡ��ͨ����Ĭ��ͨ��0
        private:
			uint16_t Write16(uint16_t sendData);
            uint Write24(uint sendData);
            uint Write32(uint sendData);
            uint ReadRegNoCS(byte reg, byte bytes);
            uint ReadReg(byte reg, byte bytes);			
            
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
            byte ad7124BufCnt;
            byte ad7124Int;
    };    
#endif
