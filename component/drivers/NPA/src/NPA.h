#ifndef _NPA_H
#define _NPA_H

    #include "../../I2C.h"

    class CNPA
    {
        public:
			CNPA(uint8_t devAddr = 0X51);
			void SetPin(Pin pinsck, Pin pinsda);
            int Read();
			float ReadP();//��ȡ����ѹֵ
        private:
            SoftI2C pi2c;
            uint8_t DEVER_ADDR; //�豸��ַ
    };
#endif
