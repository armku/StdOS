#ifndef _NPA_H
#define _NPA_H

    #include "../Device/I2C.h"

    class CNPA
    {
        public:
			CNPA(byte devAddr = 0X51);
			void SetPin(Pin pinsck, Pin pinsda);
            int Read();
			float ReadP();//��ȡ����ѹֵ
        private:
            I2CSoft_ARMKU pi2c;
            byte DEVER_ADDR; //�豸��ַ
    };
#endif
