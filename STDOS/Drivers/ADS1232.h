#ifndef _ADS1232_H
#define _ADS1232_H
    #include "Device\Port.h"

    class ADS1232
    {
        public:
            enum Speed  //AD转换速度
            {
                    SPEED10,  //10hZ
                    SPEED80  //80Hz
            };
            enum Gain  //放大倍数
            {
                    GAIN1, GAIN2, GAIN64, GAIN128
            };
        public:
            void Init();
            void SetPin(Pin pdout, Pin psclk, Pin ppwdn);
            void SetPinSpeed(Pin pin);
            void SetPinGain(Pin gain0, Pin gain1);
            void SetSpeed(Speed speed = SPEED10);
            void SetGain(Gain gain = GAIN1);
            int Read();
            void test();
        private:
            InputPort dout;
            OutputPort sclk;
            OutputPort pwdn;
            OutputPort pSpeed;
            OutputPort pGain0;
            OutputPort pGain1;
        private:
            int readCnt; //读取数量
            int oldval;
    };

#endif
