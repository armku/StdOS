#ifndef _ADS1232_H
    #define _ADS1232_H
    #include "Device\Port.h"

    class ADS1232
    {
        public:
            void Init();
            void SetPin(Pin pdout, Pin psclk, Pin ppwdn);
            int	Read();
        private:
            OutputPort dout;
            OutputPort sclk;
            OutputPort pwdn;
		private:
			int readCnt;//¶ÁÈ¡ÊýÁ¿
			int oldval;
    };

#endif
