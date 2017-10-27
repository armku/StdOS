#ifndef _DS18B20_H
    #define _DS18B20_H
    #include "Port.h"
    class DS18B20
    {
        public:
            byte Init();	
			void SetPin(Pin pin);
            float GetTemp();
		private:
			void Start();
			void WriteByte(byte dat);
			byte ReadByte();
			byte ReadBit();
			byte Presence();
			void Rest();
		private:
			OutputPort _dio;

    };
#endif
