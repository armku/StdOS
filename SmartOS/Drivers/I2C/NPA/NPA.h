#ifndef _NPA_H
#define _NPA_H

    #include "../OnChip/I2C.h"

    class CNPA
    {
        public:
			CNPA(byte devAddr = 0X51);
			void SetPin(Pin pinsck, Pin pinsda);
            int Read();
			float ReadP();//读取大气压值
        private:
            I2CSoft pi2c;
            byte DEVER_ADDR; //设备地址
    };
#endif
