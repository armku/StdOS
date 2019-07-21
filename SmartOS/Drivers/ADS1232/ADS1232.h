#ifndef _ADS1232_H
#define _ADS1232_H
    #include "Device\Port.h"

    class ADS1232
    {
        public:
            enum Speed  //ADת���ٶ�
            {
                    SPEED10,  //10hZ
                    SPEED80  //80Hz
            };
            enum Gain  //�Ŵ���
            {
                    GAIN1, GAIN2, GAIN64, GAIN128
            };
			uint Status;//״̬
			int CalCnt;//У׼����
			int readCnt; //��ȡ����
        public:
            void Init();
            void SetPin(Pin pdout, Pin psclk, Pin ppwdn);
            void SetPinSpeed(Pin pin);
            void SetPinGain(Pin gain0, Pin gain1);
            void SetSpeed(Speed speed = SPEED10);
            void SetGain(Gain gain = GAIN1);
            int Read(bool cal=false);
        private:
            Port dout;
            Port sclk;
            Port pwdn;
            Port pSpeed;
            Port pGain0;
            Port pGain1;
        private:            
            int oldval;
    };

#endif
