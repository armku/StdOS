#ifndef _ADS1246_H
#define _ADS1246_H
    #include "Spi.h"

	class ADS1246
    {
        public:
            ADS1246(Pin clk, Pin miso, Pin mosi,Pin nss,  InputPort &pinrd, Pin pinreset);
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

            SpiSoft pspi; //SPI�ӿ�
            InputPort *ppinrd; //����׼���ýӿ�
            OutputPort ppinreset; //��λ�ӿ�		
    };
#endif
