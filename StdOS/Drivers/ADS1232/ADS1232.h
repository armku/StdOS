#ifndef _ADS1232_H
#define _ADS1232_H
    #include "OnChip\Port.h"

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
			uint32_t Status;//״̬
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
			mcuGpio dout;
			mcuGpio sclk;
			mcuGpio pwdn;
			mcuGpio pSpeed;
			mcuGpio pGain0;
			mcuGpio pGain1;
        private:            
            int oldval;
    };

#endif
