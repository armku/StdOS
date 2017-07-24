#ifndef _ADS1246_H
#define _ADS1246_H
    #include "Spi.h"

	class ADS1246
    {
        public:
            ADS1246(Pin pincs, Pin pinsck, Pin pindin, Pin pindout, InputPort &pinrd, Pin pinreset);
            byte ReadReg(byte RegAddr);
            void WriteReg(byte RegAddr, byte da);
            int Read(void); //����-1,��ʾת��δ���
            void Init(void);
            byte Check(void); //AD��飬��������0
            bool GetFlag(void); //��ȡADת���Ƿ�����
        private:
            int flagOK; //adת������
            int decodead(byte *da);

            SpiSoft pspi; //SPI�ӿ�
            InputPort *ppinrd; //����׼���ýӿ�
            OutputPort ppinreset; //��λ�ӿ�		
    };
#endif
